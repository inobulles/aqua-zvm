
// instruction function helpers

static inline void zvm_set_value(zvm_program_t* self, uint64_t type, uint64_t data, int64_t value) { // set object (based on type and data) to value
	if      (type == TOKEN_REGISTER) self->state.registers[data] = value;
	
	else if (type == TOKEN_ADDRESS)   *((int64_t*) self->state.registers[data]) = value;
	else if (type == TOKEN_BYTE_ADDR) *((int8_t*)  self->state.registers[data]) = value;
	
} static inline int64_t zvm_get_value(zvm_program_t* self, uint64_t type, uint64_t data) { // get object (based on type and data) and return value
	if      (type == TOKEN_REGISTER)   return self->state.registers[data];
	
	else if (type == TOKEN_ADDRESS)   return *((int64_t*) self->state.registers[data]);
	else if (type == TOKEN_BYTE_ADDR) return *((int8_t*)  self->state.registers[data]);
	
	else if (type == TOKEN_NUMBER)    return data; // we don't need to handle TOKEN_BYTE, as bytes are anyway converted to numbers by zvm_program_get_next_token
	
	else if (type == TOKEN_RES_POS)   return self->reserved_positions[data];
	else if (type == TOKEN_RESERVED)  return self->reserved[data];
	
	else return data; // if we don't know what to return, just give back data
	
}

// instruction functions

static void zvm_cla(zvm_program_t* self) {
	uint64_t left_type, left_data;           zvm_program_get_next_token(self, &left_type, &left_data);
	uint64_t operation_type, operation_data; zvm_program_get_next_token(self, &operation_type, &operation_data);
	uint64_t right_type, right_data;         zvm_program_get_next_token(self, &right_type, &right_data);
	
	int64_t right = right_type == TOKEN_NUMBER ? right_data : self->state.registers[right_data];
	int64_t left  = self->state.registers[left_data]; // assuming left_type == TOKEN_REGISTER
	
	switch (operation_data) { // assuming operation_type == TOKEN_INSTRUCTION
		case INSTRUCTION_SUB: self->state.registers[REGISTER_AD] = left - right; break;
		case INSTRUCTION_ADD: self->state.registers[REGISTER_AD] = left + right; break;
		case INSTRUCTION_MUL: self->state.registers[REGISTER_AD] = left * right; break;
		
	}
	
} static void zvm_mov(zvm_program_t* self) {
	uint64_t dst_type, dst_data; zvm_program_get_next_token(self, &dst_type, &dst_data);
	uint64_t src_type, src_data; zvm_program_get_next_token(self, &src_type, &src_data);
	
	zvm_set_value(self, dst_type, dst_data, zvm_get_value(self, src_type, src_data));
	
}

static void zvm_cnd(zvm_program_t* self) {
	uint64_t condition_type, condition_data; zvm_program_get_next_token(self, &condition_type, &condition_data);
	self->state.next_skip = self->state.registers[condition_data]; // assuming condition_type == TOKEN_REGISTER
	
} static void zvm_cmp(zvm_program_t* self) {
	uint64_t left_type, left_data;   zvm_program_get_next_token(self, &left_type, &left_data);   int64_t left  = zvm_get_value(self, left_type, left_data);
	uint64_t right_type, right_data; zvm_program_get_next_token(self, &right_type, &right_data); int64_t right = zvm_get_value(self, right_type, right_data);
	
	int64_t result = left - right;
	
	self->state.registers[REGISTER_OF] = (result < 0) == (left < 0);
	self->state.registers[REGISTER_SF] = result < 0;
	self->state.registers[REGISTER_CF] = abs(right) < abs(left);
	self->state.registers[REGISTER_ZF] = !result;
	
}

static void zvm_jmp(zvm_program_t* self) {
	uint64_t type, data; zvm_program_get_next_token(self, &type, &data);
	self->state.registers[REGISTER_IP] = zvm_get_value(self, type, data) / sizeof(uint16_t);
	
} static void zvm_cal(zvm_program_t* self) {
	uint64_t type, data;
	zvm_program_get_next_token(self, &type, &data);
	
	if (type == TOKEN_RESERVED) { // handle reserved token types differently
		self->state.registers[REGISTER_G0] = ((int64_t (*) (int64_t, int64_t, int64_t, int64_t)) self->reserved[data])( \
			self->state.registers[REGISTER_A0], \
			self->state.registers[REGISTER_A1], \
			self->state.registers[REGISTER_A2], \
			self->state.registers[REGISTER_A3]);
		
	} else {
		self->state.nest++;
		
		*((int64_t*) (self->state.registers[REGISTER_SP] -= sizeof(int64_t))) = self->state.registers[REGISTER_IP]; // push the current IP to the stack
		self->state.registers[REGISTER_IP] = zvm_get_value(self, type, data) / sizeof(uint16_t); // jump
		
	}
	
} static void zvm_ret(zvm_program_t* self) {
	if (--self->state.nest < 0) { // exit program if returning to nothing
		self->state.registers[REGISTER_IP] = self->meta->length * ZVM_SIZE;
		
	}
	
	self->state.registers[REGISTER_IP] = *((int64_t*) self->state.registers[REGISTER_SP]); // return to call position
	self->state.registers[REGISTER_SP] += sizeof(int64_t); // pop off IP from stack
	
}

static void zvm_psh(zvm_program_t* self) {
	uint64_t type, data; zvm_program_get_next_token(self, &type, &data);
	*((int64_t*) (self->state.registers[REGISTER_SP] -= sizeof(int64_t))) = zvm_get_value(self, type, data);
	
} static void zvm_pop(zvm_program_t* self) {
	uint64_t type, data; zvm_program_get_next_token(self, &type, &data);
	zvm_set_value(self, type, data, *((int64_t*) self->state.registers[REGISTER_SP]));
	self->state.registers[REGISTER_SP] += sizeof(int64_t);
	
}

static void (*zvm_instructions)(zvm_program_t* self)[INSTRUCTION_COUNT] = { // list of all instruction function pointers for fast indexing
	(void*) zvm_cla,
	(void*) zvm_mov,
	
	(void*) zvm_cnd,
	(void*) zvm_cmp,
	
	(void*) zvm_jmp,
	(void*) zvm_cal,
	(void*) zvm_ret,
	
	(void*) zvm_psh,
	(void*) zvm_pop,
};
