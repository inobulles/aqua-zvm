# The `add` (arithmetic add) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `add`                 |
| Name                 | arithmetic add        |
| `zed.h` denomination | `ZED_INSTRUCTION_ADD` |
| Opcode               | 0x6                   |
| Operands             | 3                     |

Copies the sum of the second operand (`source_a`) with the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be sign-extended to 64 bits before addition.
If `destination` is an 8-bit address, it'll truncate the sum to its 8 lowest bits before copying.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
add destination source_a source_b
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

if (source_a.type == ADDRESS_8) {
	source_a = (int8_t) source_a; // sign-extend 'source_a'
}

if (source_b.type == ADDRESS_8) {
	source_b = (int8_t) source_b; // sign-extend 'source_b'
}

destination = source_a + source_b; // copy the sum of 'source_a' and 'source_b' to 'destination'
```

## History

`add` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.