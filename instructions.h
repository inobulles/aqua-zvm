
// instruction function helpers

static inline void zvm_set_value(zvm_program_t* self, uint64_t type, uint64_t data, uint64_t value) { // set object (based on type and data) to value
	switch (type) {
		case ZED_OPERAND_16_TYPE_REGISTER:                           self->state.registers[data] = value; break;
			
		case ZED_OPERAND_16_TYPE_ADDRESS_64: *(uint64_t*) (intptr_t) self->state.registers[data] = value; break;
		case ZED_OPERAND_16_TYPE_ADDRESS_8:  *(uint8_t *) (intptr_t) self->state.registers[data] = value; break;
	}
}

static inline uint64_t zvm_get_value(zvm_program_t* self, uint64_t type, uint64_t data) { // get object (based on type and data) and return value
	switch (type) {
		case ZED_OPERAND_16_TYPE_REGISTER:       return                                    self->state.registers[data];
		case ZED_OPERAND_16_TYPE_CONSTANT:       return                                                          data;

		case ZED_OPERAND_16_TYPE_ADDRESS_64:     return            *(uint64_t*) (intptr_t) self->state.registers[data];
		case ZED_OPERAND_16_TYPE_ADDRESS_8:      return (uint64_t) *(uint8_t *) (intptr_t) self->state.registers[data]; // zero extended by default

		case ZED_OPERAND_16_TYPE_KFUNC_INDEX:    return (uint64_t)  (data < ZVM_KFUNC_COUNT ? zvm_kfunc_pointers[data] : 0);
		case ZED_OPERAND_16_TYPE_POSITION_INDEX: return (uint64_t)                               self->positions[data];

		case ZED_OPERAND_16_TYPE_DATA_INDEX:     return (uint64_t)      (self->rom + self->data_section_elements[data].data_offset);

		default: return 0;
	}
}

static inline uint64_t zvm_get_value_sext(zvm_program_t* self, uint64_t type, uint64_t data) { // sign-extend if operand type is 8-bit address
	if (type == ZED_OPERAND_16_TYPE_ADDRESS_8) {
		return (uint64_t) *(int8_t*) (intptr_t) self->state.registers[data];
	}

	return zvm_get_value(self, type, data);
}

// instruction functions

static void zvm_mov(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source);
}

static void zvm_red(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, 1 - !source);
}

static void zvm_inv(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, ~source);
}

static void zvm_jmp(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t position = zvm_get_value(self, instruction->operand1_type, instruction->operand1_data);
	self->state.registers[ZED_REGISTER_IP] = position;
}

static void zvm_cal(zvm_program_t* self, zvm_instruction_t* instruction) {
	zvm_kfunc_t kfunc = (zvm_kfunc_t) (intptr_t) zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);

	uint64_t retval = kfunc(self,
		self->state.registers[ZED_REGISTER_A0],
		self->state.registers[ZED_REGISTER_A1],
		self->state.registers[ZED_REGISTER_A2],
		self->state.registers[ZED_REGISTER_A3]
	);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, retval);
}

static void zvm_cpe(zvm_program_t* self, zvm_instruction_t* instruction) {
	int64_t source_a = zvm_get_value_sext(self, instruction->operand1_type, instruction->operand1_data);
	int64_t source_b = zvm_get_value_sext(self, instruction->operand2_type, instruction->operand2_data);

	self->state.registers[ZED_REGISTER_F2] = (uint64_t) source_a > (uint64_t) source_b;

	uint64_t comparison = source_a - source_b; // shouldn't really matter if this stuff is signed or not
	uint8_t sign = comparison >> 63;

	self->state.registers[ZED_REGISTER_F1] = sign;

	self->state.registers[ZED_REGISTER_F3] = (uint64_t) source_a >> 63 != sign;
	self->state.registers[ZED_REGISTER_F0] = !comparison;

	uint64_t source_e = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data); // important that this go AFTER comparison
	
	if (!source_e) { // skip next instruction
		zvm_instruction_t dummy_instruction;
		zvm_next_instruction(self, &dummy_instruction);
	}
}

static void zvm_add(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source_a = zvm_get_value_sext(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t source_b = zvm_get_value_sext(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a + source_b);
}

static void zvm_sub(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source_a = zvm_get_value_sext(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t source_b = zvm_get_value_sext(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a - source_b);
}

static void zvm_mul(zvm_program_t* self, zvm_instruction_t* instruction) {
	int64_t source_a = (int64_t) zvm_get_value_sext(self, instruction->operand2_type, instruction->operand2_data);
	int64_t source_b = (int64_t) zvm_get_value_sext(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a * source_b);
}

static void zvm_div(zvm_program_t* self, zvm_instruction_t* instruction) {
	int64_t source_a = (int64_t) zvm_get_value_sext(self, instruction->operand2_type, instruction->operand2_data);
	int64_t source_b = (int64_t) zvm_get_value_sext(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a / source_b);
	self->state.registers[ZED_REGISTER_F3] = source_a % source_b;
}

static void zvm_sel(zvm_program_t* self, zvm_instruction_t* instruction) {
	// ignore for now, not exactly sure i really want this instruction
}

static void zvm_and(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source_a = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t source_b = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a & source_b);
}

static void zvm_or(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source_a = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t source_b = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a | source_b);
}

static void zvm_xor(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source_a = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t source_b = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source_a ^ source_b);
}

static void zvm_shl(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t shift  = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source << shift);
}

static void zvm_shr(zvm_program_t* self, zvm_instruction_t* instruction) {
	uint64_t source = zvm_get_value(self, instruction->operand2_type, instruction->operand2_data);
	uint64_t shift  = zvm_get_value(self, instruction->operand3_type, instruction->operand3_data);

	zvm_set_value(self, instruction->operand1_type, instruction->operand1_data, source >> shift);
}

typedef void (*zvm_ofunc_t) (zvm_program_t* self, zvm_instruction_t* instruction); // technically should be called 'zvm_ofunc_pointer_t' but whatever

zvm_ofunc_t zvm_operations[ZED_OPERATION_COUNT] = { // list of all operation function pointers for fast indexing
	(zvm_ofunc_t) zvm_mov,
	(zvm_ofunc_t) zvm_red, (zvm_ofunc_t) zvm_inv,
	(zvm_ofunc_t) zvm_jmp, (zvm_ofunc_t) zvm_cal,
	(zvm_ofunc_t) zvm_cpe,
	(zvm_ofunc_t) zvm_add, (zvm_ofunc_t) zvm_sub, (zvm_ofunc_t) zvm_mul, (zvm_ofunc_t) zvm_div,
	(zvm_ofunc_t) zvm_sel,
	(zvm_ofunc_t) zvm_and, (zvm_ofunc_t) zvm_or,  (zvm_ofunc_t) zvm_xor,
	(zvm_ofunc_t) zvm_shl, (zvm_ofunc_t) zvm_shr,
};
