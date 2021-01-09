# The `cpe` (compare and execute) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `cpe`                 |
| Name                 | compare and execute   |
| `zed.h` denomination | `ZED_INSTRUCTION_CPE` |
| Opcode               | 0x5                   |
| Operands             | 3                     |

Compares the first operand (`source_a`) with the second operand (`source_b`) first, and then only moves on to the next position in the instruction stream if the third operand (`source_e`) is set (i.e. it skips the next instruction if `source_e` is **not** set). The comparison is performed by subtracting `source_b` from `source_a`, and setting flags according to the result.

If `source_a` or `source_b` is an 8-bit address, the value it points to will be sign-extended to 64 bits before comparison.

`source_a`, `source_b`, and `source_e` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.

## ZASM syntax

```zasm
cpe source_a source_b source_e
```

## Flags and registers affected

The instruction pointer register (`ip`) is usually set to point to the next instruction in the instruction stream, or the one after that (depending on if `source_e` is set or not), although, as with the `jmp` (jump) instruction, the specific implementation may choose to handle this differently.

The carry flag (`cf`) is set if the unsigned representation of `source_a` is strictly greater than `source_b`.

The sign flag (`sf`) is set if the result of the comparison is strictly less than 0 (negative).

The offset flag (`of`) is set if the sign of the result of the comparison is **not** equal to that of `source_a`.

The zero flag (`zf`) is set if the result of the comparison is equal to 0 (i.e. `source_a` is equal to `source_b`)

## Pseudocode

### C-like

```c
#include <stdint.h>

cf = (uint64_t) source_a > (uint64_t) source_b;

int64_t comparison = source_a - source_b;
uint8_t sign = (uint64_t) comparison >> 63;

sf = sign;

of = (uint64_t) source_a >> 63 != sign;
zf = comparison == 0; // i.e. source_a == source_b

if (!source_e) { // note that 'source_e' could very well be one of the flags just set
	goto next_instruction; // skip next instruction
}
```

## History

`cpe` was introduced in ZEDv5.
It was preceeded by the `cmp` (compare) and `cnd` (condition) instructions in ZEDv4, and by the `cmp` (compare), `test` (test), and `cnd` (condition) instructions in ZEDv1, ZEDv2, and ZEDv3.