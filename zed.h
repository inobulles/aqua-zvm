#if !defined(__AQUA_ZED__ZED_H)
#define __AQUA_ZED__ZED_H
// zed and zpk filetype related macros

#define ZPK_UNIQUE_PATH "unique"
#define ZPK_ROM_PATH "rom.zed"

#define ZPK_META_NAME_PATH "meta/name"
#define ZPK_META_AUTHOR_PATH "meta/author"
#define ZPK_META_VERSION_PATH "meta/version"
#define ZPK_META_ORGANIZATION_PATH "meta/organization"
#define ZPK_META_DESCRIPTION_PATH "meta/description"

#define ZED_MAGIC 0x7A647A647A647A64
#define ZED_VERSION 5

typedef struct {
	uint64_t magic, version;
	uint64_t data_section_offset, data_section_element_count;
	uint64_t position_section_offset, position_section_element_count;
	uint64_t logic_section_offset, logic_section_words;
} zed_meta_section_t;

typedef struct {
	uint64_t bytes;
	uint64_t data_offset;
} zed_data_section_element_t;

// note that these following macros aren't really here to make changing the ZED standard easier
// rather, they're for improving readability inside of dependant source files
// so it's really not a big deal if you don't use them

#define ZED_OPERATION_COUNT 16
#define ZED_REGISTER_COUNT 16

typedef struct {
	unsigned operation       : 4;

	unsigned operand1_64_bit : 1;
	unsigned operand1_type   : 3;

	unsigned operand2_64_bit : 1;
	unsigned operand2_type   : 3;

	unsigned operand3_64_bit : 1;
	unsigned operand3_type   : 3;

	unsigned operand1_data   : 16;
	unsigned operand2_data   : 16;
	unsigned operand3_data   : 16;
} zed_instruction_t; // 64 bits (size of a logic section word)

#define ZED_OPERAND_16_TYPE_NONE           (0b0000 | 0)
#define ZED_OPERAND_16_TYPE_REGISTER       (0b0000 | 1)
#define ZED_OPERAND_16_TYPE_CONSTANT       (0b0000 | 2)
#define ZED_OPERAND_16_TYPE_ADDRESS_64     (0b0000 | 3)
#define ZED_OPERAND_16_TYPE_ADDRESS_8      (0b0000 | 4)
#define ZED_OPERAND_16_TYPE_POSITION_INDEX (0b0000 | 5)
#define ZED_OPERAND_16_TYPE_KFUNC_INDEX    (0b0000 | 6)
#define ZED_OPERAND_16_TYPE_DATA_INDEX     (0b0000 | 7)

#define ZED_OPERAND_64_TYPE_CONSTANT       (0b1000 | 2)
#define ZED_OPERAND_64_TYPE_POSITION_INDEX (0b1000 | 5)
#define ZED_OPERAND_64_TYPE_DATA_INDEX     (0b1000 | 7)

// instruction set architecture related macros

typedef struct {
	char* name; // must be null-terminated

	union { // only if token is data or position label
		struct {
			uint64_t data_bytes;
			uint8_t* data;
		};

		uint64_t position;
	};
} zed_token_t;

typedef enum {
	ZED_OPCODE_MOV =  0,
	ZED_OPCODE_RED =  1, ZED_OPCODE_INV =  2,
	ZED_OPCODE_JMP =  3, ZED_OPCODE_CAL =  4,
	ZED_OPCODE_CPE =  5,
	ZED_OPCODE_ADD =  6, ZED_OPCODE_SUB =  7, ZED_OPCODE_MUL =  8, ZED_OPCODE_DIV =  9,
	ZED_OPCODE_SEL = 10,
	ZED_OPCODE_AND = 11, ZED_OPCODE_OR  = 12, ZED_OPCODE_XOR = 13,
	ZED_OPCODE_SHL = 14, ZED_OPCODE_SHR = 15,
} zed_opcode_t;

static zed_token_t zed_opcodes[] = {
	{"mov"},
	{"red"}, {"inv"},
	{"jmp"}, {"cal"},
	{"cpe"},
	{"add"}, {"sub"}, {"mul"}, {"div"},
	{"sel"},
	{"and"}, {"or" }, {"xor"},
	{"shl"}, {"shr"},
};

typedef enum {
	ZED_REGISTER_IP =  0, ZED_REGISTER_SP =  1, ZED_REGISTER_BP =  2, ZED_REGISTER_AD =  3,
	ZED_REGISTER_G0 =  4, ZED_REGISTER_G1 =  5, ZED_REGISTER_G2 =  6, ZED_REGISTER_G3 =  7,
	ZED_REGISTER_A0 =  8, ZED_REGISTER_A1 =  9, ZED_REGISTER_A2 = 10, ZED_REGISTER_A3 = 11,
	ZED_REGISTER_F0 = 12, ZED_REGISTER_F1 = 13, ZED_REGISTER_F2 = 14, ZED_REGISTER_F3 = 15,
} zed_register_t;

static zed_token_t zed_registers[] = {
	{"ip"}, {"sp"}, {"bp"}, {"ad"},
	{"g0"}, {"g1"}, {"g2"}, {"g3"},
	{"a0"}, {"a1"}, {"a2"}, {"a3"},
	{"f0"}, {"f1"}, {"f2"}, {"f3"},
};

typedef enum {
	ZED_KFUNC_NOOP         =  0, ZED_KFUNC_PRINT            = 1, ZED_KFUNC_EXIT         = 2,
	ZED_KFUNC_ALLOCATE     =  3, ZED_KFUNC_FREE             = 4, ZED_KFUNC_COPY         = 5, ZED_KFUNC_ZERO        =  6,
	ZED_KFUNC_GET_PLATFORM =  7, ZED_KFUNC_PLATFORM_COMMAND = 8, ZED_KFUNC_QUERY_DEVICE = 9, ZED_KFUNC_SEND_DEVICE = 10, ZED_KFUNC_NATIVE = 11,
	ZED_KFUNC_GET_REQUESTS = 12,
} zed_kfunc_t;

static zed_token_t zed_kfuncs[] = {
	{"noop"},         {"print"},            {"exit"},
	{"allocate"},     {"free"},             {"copy"},   {"zero"},
	{"get_platform"}, {"platform_command"}, {"native"}, {"get_requests"},
	{"query_device"}, {"send_device"}
};

#endif
