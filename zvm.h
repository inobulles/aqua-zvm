
#include <stdint.h>
#include "defs.h"

typedef unsigned long long val;

typedef struct {
	uint64_t length, version, invalidated;
	uint64_t base_reserved_count, main_reserved_position;
	uint64_t debugging_mode, verbose_mode;
	uint64_t data_section_element_count;
	
} meta_section_t;

typedef struct {
	uint8_t* data;
	
	val element_count, element_count_bytes;
	val* element_element_count; void** start_position;
	
	uint8_t* contiguous_data;
	
} data_section_t;

typedef struct {
	int64_t registers[REGISTER_COUNT];
	
} state_t;

typedef struct {
	void* pointer;
	signed long long warning_count;
	
	meta_section_t* meta;
	data_section_t data_section;
	
	val reserved_count;
	void** reserved;
	
	state_t state;
	
} program_t;

void program_run_setup_phase(program_t* self) {
	
	
}
