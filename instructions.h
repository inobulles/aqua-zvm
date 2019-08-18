
static void zvm_cla(zvm_program_t* self) {
	uint64_t left_type, left_data;
	uint64_t operation_type, operation_data;
	uint64_t right_type, right_data;
	
	zvm_program_get_next_token(self, &left_type, &left_data);
	zvm_program_get_next_token(self, &operation_type, &operation_data);
	zvm_program_get_next_token(self, &right_type, &right_data);
	
	int64_t right = right_type == TOKEN_NUMBER ? right_data : self->state.registers[right_data];
	int64_t left  = self->state.registers[left_data]; // assuming left_type == TOKEN_REGISTER
	
	switch (operation_data) { // assuming operation_type == TOKEN_INSTRUCTION
		case INSTRUCTION_SUB: self->state.registers[REGISTER_AD] = left - right; break;
		case INSTRUCTION_ADD: self->state.registers[REGISTER_AD] = left + right; break;
		case INSTRUCTION_MUL: self->state.registers[REGISTER_AD] = left * right; break;
		
	}
	
} static void zvm_mov(zvm_program_t* self) {
	
	
}

static void (*zvm_instructions)(zvm_program_t* self)[INSTRUCTION_COUNT] = { // list of all instruction function pointers for fast indexing
	(void*) zvm_cla,
	(void*) zvm_mov,
};
