
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <zed.h>

typedef struct {
	int64_t registers[ZED_REGISTER_COUNT];
	
	uint64_t  stack_size;
	uint64_t* stack;
} zvm_state_t;

typedef struct {
	#define ZVM_BDA_SIGNATURE 0xBDA5BDA5BDA5BDA5
	uint64_t signature;
	
	#ifdef KOS_BDA
		uint64_t kos_bda[128];
	#endif
} zvm_bda_t;

typedef struct {
	void* rom;
	int64_t error_code;

	zed_meta_section_t* meta_section;
	zed_data_section_element_t* data_section_elements;

	uint64_t* positions;
	uint64_t* logic_section;

	zvm_state_t state;
	zvm_bda_t* bda;
} zvm_program_t;

#include "kos.h"

void zvm_program_free(zvm_program_t* self) {
	/// TODO free
}

int zvm_program_run_setup_phase(zvm_program_t* self) {
	// read rom sections

	self->meta_section = (zed_meta_section_t*) self->rom;

	if (self->meta_section->magic != ZED_MAGIC) {
		fprintf(stderr, "[AQUA ZVM] ERROR ROM is not a valid ZED ROM (magic = 0x%lx)\n", (long) self->meta_section->magic);
		return 1;
	}

	self->data_section_elements = (zed_data_section_element_t*) (self->rom + self->meta_section->data_section_offset);
	self->positions = (void*) (self->rom + self->meta_section->position_section_offset);
	self->logic_section = (void*) (self->rom + self->meta_section->logic_section_offset);

	// allocate bda
	
	self->bda = (void*) (intptr_t) zvm_allocate(self, sizeof(*self->bda));
	self->bda->signature = ZVM_BDA_SIGNATURE;
	
	#ifdef KOS_BDA
		extern uint64_t* kos_bda;
		extern uint64_t  kos_bda_bytes;
		
		kos_bda = self->bda->kos_bda;
		kos_bda_bytes = sizeof(self->bda->kos_bda);
		
		zvm_zero(self, (uint64_t) (intptr_t) kos_bda, kos_bda_bytes);
	#endif
	
	// get ready for parsing the logic section
	
	self->state.stack_size = 1ll << 16;
	self->state.stack = (uint64_t*) (intptr_t) zvm_allocate(self, self->state.stack_size * sizeof(*self->state.stack));
	self->state.registers[ZED_REGISTER_SP] = (int64_t) (intptr_t) (self->state.stack + self->state.stack_size);
	
	// push main label position (index 0) to the stack

	int64_t* stack_pointer = (int64_t*) (intptr_t) (self->state.registers[ZED_REGISTER_SP] -= sizeof(int64_t));
	self->state.registers[ZED_REGISTER_IP] = self->positions[0];
	*stack_pointer = self->state.registers[ZED_REGISTER_IP];

	return 0;
}

typedef struct {
	zed_instruction_t* instruction;

	uint8_t operation;

	uint8_t operand1_type;
	uint8_t operand2_type;
	uint8_t operand3_type;

	uint64_t operand1_data;
	uint64_t operand2_data;
	uint64_t operand3_data;
} zvm_instruction_t; // not to be confused with zed_instruction_t

static inline void zvm_next_instruction(zvm_program_t* self, zvm_instruction_t* instruction) {
	instruction->instruction = (zed_instruction_t*) (self->logic_section + self->state.registers[ZED_REGISTER_IP]++);
	
	instruction->operation = instruction->instruction->operation;

	instruction->operand1_type = instruction->instruction->operand1_type;
	instruction->operand2_type = instruction->instruction->operand2_type;
	instruction->operand3_type = instruction->instruction->operand3_type;

	if (instruction->instruction->operand1_64_bit) instruction->operand1_data = self->logic_section[self->state.registers[ZED_REGISTER_IP]++];
	else instruction->operand1_data = instruction->instruction->operand1_data;

	if (instruction->instruction->operand2_64_bit) instruction->operand2_data = self->logic_section[self->state.registers[ZED_REGISTER_IP]++];
	else instruction->operand2_data = instruction->instruction->operand2_data;

	if (instruction->instruction->operand3_64_bit) instruction->operand3_data = self->logic_section[self->state.registers[ZED_REGISTER_IP]++];
	else instruction->operand3_data = instruction->instruction->operand3_data;
}

#include "instructions.h"

int zvm_program_run_loop_phase(zvm_program_t* self) {
	zvm_instruction_t instruction;
	zvm_next_instruction(self, &instruction);
	
	zvm_operations[instruction.operation](self, &instruction);
	
	if (self->state.registers[ZED_REGISTER_IP] > self->meta_section->logic_section_words) {
		self->error_code = self->state.registers[ZED_REGISTER_G0]; // get error code
		return 1; // stop execution
		
	} else {
		return 0; // continue execution
	}
}
