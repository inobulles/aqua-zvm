#define ZVM_SIZE (sizeof(uint64_t) / sizeof(uint16_t))

typedef struct {
	uint64_t length, version, invalidated;
	uint64_t prereserved_count, data_section_element_count;
	uint64_t reserved_positions_count, main_reserved_position, text_section_start;
	
} zvm_meta_section_t;

typedef struct {
	uint8_t* data;
	
	uint64_t  element_count, element_count_bytes;
	uint64_t* element_element_count; void** start_positions;
	
	uint8_t* contiguous_data;
	
} zvm_data_section_t;

typedef struct {
	int64_t registers[REGISTER_COUNT];
	
	uint64_t  stack_size;
	uint64_t* stack;
	
	uint8_t next_skip;
	int64_t nest;
	
} zvm_state_t;

typedef struct {
	#define ZVM_BDA_SIGNATURE 0xBDA5BDA5BDA5BDA5
	uint64_t signature;
	
	#ifdef KOS_BDA
		uint64_t kos_bda[128];
	#endif
} zvm_bda_t;

typedef struct {
	void* pointer;
	int64_t error_code;
	
	zvm_meta_section_t* meta;
	zvm_data_section_t data_section;
	uint16_t* text_section_pointer;
	
	uint64_t* reserved_positions;
	uint64_t reserved_count;
	void** reserved;
	
	zvm_state_t state;
	zvm_bda_t* bda;
	
} zvm_program_t;
