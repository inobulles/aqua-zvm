
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "structs.h"
#include "kos.h"

void zvm_program_run_setup_phase(zvm_program_t* self) {
	uint8_t* pointer = (uint8_t*) self->pointer;
	memset(&self->state, 0, sizeof(self->state));
	
	// parse meta section
	
	self->meta = (zvm_meta_section_t*) pointer;
	pointer += sizeof(*self->meta);
	
	// allocate bda
	
	self->bda = (zvm_bda_t*) zvm_malloc((uint64_t) self, sizeof(*self->bda));
	self->bda->signature = ZVM_BDA_SIGNATURE;
	
	// parse data section
	
	self->data_section.element_count       = self->meta->data_section_element_count;
	self->data_section.element_count_bytes = self->meta->data_section_element_count * sizeof(uint64_t);
	
	self->data_section.element_element_count = (uint64_t*) malloc(self->data_section.element_count_bytes);
	self->data_section.start_positions       = (void**)    malloc(self->data_section.element_count * sizeof(void*));
	
	// get the size of each data section element
	
	uint64_t* data_section_pointer = (uint64_t*) pointer;
	for (uint64_t i = 0; i < self->data_section.element_count; i++) {
		self->data_section.element_element_count[i] = *data_section_pointer++;
		
	}
	
	// get the pointers to the contents of each data section element
	
	self->data_section.contiguous_data = (uint8_t*) data_section_pointer;
	uint8_t* temp_contiguous_data = self->data_section.contiguous_data;
	
	for (uint64_t i = 0; i < self->data_section.element_count; i++) {
		self->data_section.start_positions[i] = temp_contiguous_data;
		temp_contiguous_data += self->data_section.element_element_count[i] + 1;
		
	}
	
	// parse the reserved positions section
	
	uint64_t* pointer64 = (uint64_t*) temp_contiguous_data;
	self->reserved_positions = (uint64_t*) malloc(self->meta->reserved_positions_count * sizeof(uint64_t));
	
	for (uint64_t i = 0; i < self->meta->reserved_positions_count; i++) {
		self->reserved_positions[i] = *pointer64++;
		
	}
	
	// build the reserved list (with the prereserved pointers and the data section element pointers)
	
	self->reserved_count = self->meta->prereserved_count + self->data_section.element_count;
	self->reserved = (void**) malloc(self->reserved_count * sizeof(void*));
	
	for (uint64_t i = 0; i < self->meta->prereserved_count;    i++) self->reserved[i]                                 = prereserved                       [i];
	for (uint64_t i = 0; i < self->data_section.element_count; i++) self->reserved[i + self->meta->prereserved_count] = self->data_section.start_positions[i];
	
	// get ready for parsing the text section
	
	self->state.stack_size = 1ll << 16;
	self->state.stack = (uint64_t*) zvm_malloc((uint64_t) self, self->state.stack_size * sizeof(*self->state.stack));
	self->state.registers[REGISTER_SP] = (int64_t) (self->state.stack + self->state.stack_size * sizeof(*self->state.stack));
	
	*((int64_t*) (self->state.registers[REGISTER_SP] -= sizeof(int64_t))) = self->state.registers[REGISTER_IP] = self->meta->main_reserved_position / sizeof(uint16_t); // push main label position to stack
	self->text_section_pointer = (uint16_t*) (self->pointer + self->meta->text_section_start);
	
}

uint64_t zvm_program_get_next_token(zvm_program_t* self, uint64_t* type, uint64_t* data) { // get the next token and advance all pointers and all
	uint64_t token = (uint64_t) *(self->text_section_pointer + self->state.registers[REGISTER_IP]++);
	*type = token & 0x00FF;
	
	if (*type == TOKEN_NUMBER || *type == TOKEN_RES_POS || *type == TOKEN_RESERVED) { // is the data size of the token 8 bytes (64 bit)?
		if (self->state.registers[REGISTER_IP] % ZVM_SIZE) { // 32 bit aligned?
			*data = *(((uint64_t*) self->text_section_pointer) + self->state.registers[REGISTER_IP] / ZVM_SIZE + 1);
			self->state.registers[REGISTER_IP] = (self->state.registers[REGISTER_IP] / ZVM_SIZE + 2) * ZVM_SIZE;
			
		} else { // not 32 bit aligned?
			*data = *(((uint64_t*) self->text_section_pointer) + self->state.registers[REGISTER_IP] / ZVM_SIZE + 1);
			self->state.registers[REGISTER_IP] = (self->state.registers[REGISTER_IP] / ZVM_SIZE + 2) * ZVM_SIZE;
			
		}
		
	} else { // is the data size a single byte?
		*data = (token & 0xFF00) >> 8;
		
	}
	
	*type = *type == TOKEN_BYTE ? TOKEN_NUMBER : *type; // from now on, consider a byte token as a number token
	return token;
	
}

#include "instructions.h"

int64_t zvm_program_run_loop_phase(zvm_program_t* self) {
	uint64_t type, data;
	zvm_program_get_next_token(self, &type, &data); // get next token
	
	// am assuming type == TOKEN_INSTRUCTION
	((void (*)(zvm_program_t* self)) zvm_instructions[data])(self);
	
	if (self->state.registers[REGISTER_IP] > (self->meta->length - self->meta->text_section_start) * ZVM_SIZE) {
		self->error_code = self->state.registers[REGISTER_G0]; // get error code
		
		// parse signature
		
		uint64_t signature = *(((uint64_t*) self->text_section_pointer) + (self->state.registers[REGISTER_IP] += ZVM_SIZE) / ZVM_SIZE); 
		char signature_string[9] = {0};
		memcpy(signature_string, &signature, sizeof(signature));
		
		if (strcmp(signature_string, "AQUA-ZED") != 0) {
			// program signature does not match the default signature
			
		}
		
		return 1; // stop execution
		
	} else {
		return 0; // continue execution
		
	}
	
}
