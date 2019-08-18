
#include <stdint.h>
#include "defs.h"

typedef struct {
	uint64_t length, version, invalidated;
	uint64_t preserved_count, reserved_positions_count, main_reserved_position;
	uint64_t data_section_element_count;
	
} zed_meta_section_t;

typedef struct {
	uint8_t* data;
	
	uint64_t  element_count, element_count_bytes;
	uint64_t* element_element_count; void** start_positions;
	
	uint8_t* contiguous_data;
	
} zed_data_section_t;

typedef struct {
	int64_t registers[REGISTER_COUNT];
	
} zed_state_t;

#define ZED_BDA_SIGNATURE 0xBDA5BDA5BDA5BDA5
typedef struct {
	uint64_t signature;
	
	#ifdef KOS_BDA_EXTENSION
		kos_bda_extension_t extension;
	#endif
} zed_bda_t;

typedef struct {
	void* pointer;
	int64_t warning_count;
	
	zed_meta_section_t* meta;
	zed_data_section_t data_section;
	
	uint64_t reserved_count;
	void** reserved;
	
	zed_state_t state;
	zed_bda_t* bda;
	
} zed_program_t;

void zed_program_run_setup_phase(zed_program_t* self) {
	self->warning_count = 0;
	uint8_t* pointer = (uint8_t*) self->pointer;
	
	// parse meta section
	
	self->meta = (zed_meta_section_t*) pointer;
	pointer += sizeof(*self->meta);
	
	// allocate bda
	
	self->bda = (zed_bda_t*) heap_malloc(sizeof(*self->bda));
	self->bda->signature = ZED_BDA_SIGNATURE;
	
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
	
	// build the reserved list (with the prereserved pointers and the data section element pointers)
	
	self->reserved_count = self->meta->preserved_count + self->data_section.element_count;
	self->reserved = (void**) malloc(self->reserved_count * sizeof(void*));
	
	for (uint64_t i = 0; i < self->meta->preserved_count;      i++) self->reserved[i]                               = preserved                         [i];
	for (uint64_t i = 0; i < self->data_section.element_count; i++) self->reserved[i + self->meta->preserved_count] = self->data_section.start_positions[i];
	
	// parse the reserved positions section
	
	pointer = (uint64_t*) temp_contiguous_data;
	self->reserved_positions = (uint64_t*) malloc(self->meta->reserved_positions_count * sizeof(uint64_t));
	
	for (uint64_t i = 0; i < self->meta->reserved_positions_count; i++) {
		self->reserved_positions[i] = *pointer++;
		
	}
	
	// get ready for parsing the text section
	
	self->state.registers[REGISTER_IP] = self->meta->main_reserved_position / sizeof(uint16_t);
	zed_push(self, TOKEN_REGISTER, REGISTER_IP);
	
}
