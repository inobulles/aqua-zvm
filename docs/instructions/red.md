# The `red` (reduce) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `red`                 |
| Name                 | reduce                |
| `zed.h` denomination | `ZED_INSTRUCTION_RED` |
| Opcode               | 0x1                   |
| Operands             | 2                     |

Reduces (bitwise or's together) all the bits within the second operand (`source`) to a single bit, and fills the first operand (`destination`) with that bit.

`source` can be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
red destination source
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c
#include <stdint.h>

uint8_t reduced = !!source; // reduce 'source' to a single bit
destination = 0 - reduced; // fill 'destination' with that bit
```

## History

`red` was introduced in ZEDv5.
