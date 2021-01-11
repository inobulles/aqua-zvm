# The `mov` (move) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `mov`                 |
| Name                 | move                  |
| `zed.h` denomination | `ZED_INSTRUCTION_MOV` |
| Opcode               | 0x0                   |
| Operands             | 2                     |

Copies the second operand (`source`) to the first operand (`destination`).

If `source` is an 8-bit address, the value it points to will be zero-extended before copying.
If `destination` is an 8-bit address, it'll truncate `source` to its 8 lowest bits before copying.

`source` can be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
mov destination source
```

## Flags and registers affected

None.

## Pseudocode

### C-like

```c++
#include <stdint.h>

if (source.type == ADDRESS_8) {
	source = (uint8_t) source; // zero extend 'source'
}

destination = source; // copy 'source' to 'destination'
```

## History

`mov` was introduced in ZEDv1.
