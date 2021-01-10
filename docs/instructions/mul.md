# The `mul` (arithmetic multiply) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `mul`                 |
| Name                 | arithmetic multiply   |
| `zed.h` denomination | `ZED_INSTRUCTION_MUL` |
| Opcode               | 0x8                   |
| Operands             | 3                     |

Copies the signed integer product of the second operand (`source_a`) with the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be sign-extended to 64 bits before multiplication.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
mul destination source_a source_b
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

destination = (int64_t) source_a * (int64_t) source_b; // copy the product of 'source_a' with 'source_b' to 'destination'
```

## History

`mul` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.