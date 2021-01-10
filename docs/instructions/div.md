# The `div` (arithmetic divide) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `div`                 |
| Name                 | arithmetic divide     |
| `zed.h` denomination | `ZED_INSTRUCTION_DIV` |
| Opcode               | 0x9                   |
| Operands             | 3                     |

Copies the signed integer division of the second operand (`source_a`) by the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be sign-extended to 64 bits before division.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
div destination source_a source_b
```

## Flags and registers affected

The fourth flag register (`f3`) is set to the remainder of the division (i.e. the modulo of `source_a` by `source_b`).

## Pseudocode

### C-like

```c++
#include <stdint.h>

destination = (int64_t) source_a / (int64_t) source_b; // copy the division of 'source_a' by 'source_b' to 'destination'
f3 = source_a % source_b; // copy the modulo of 'source_a' by 'source_b' to 'f3'
```

## History

`div` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.