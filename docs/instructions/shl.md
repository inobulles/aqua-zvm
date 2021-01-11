# The `shl` (bitwise shift left) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `shl`                 |
| Name                 | bitwise shift left    |
| `zed.h` denomination | `ZED_INSTRUCTION_SHL` |
| Opcode               | 0xe                   |
| Operands             | 3                     |

Copies the second operand (`source`) shifted left by the amount of bits denoted by the third operand (`shift`) to the first operand (`destination`).

If `source` or `shift` is an 8-bit address, the value it points to will be zero-extended to 64 bits before shifting.
If `destination` is an 8-bit address, it'll truncate the result of the shift to its 8 lowest bits before copying.

`source` and `shift` can each be a register, a constant, an address (8 or 64-bit), a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
shl destination source shift
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

if (source.type == ADDRESS_8) {
	source = (uint8_t) source; // zero-extend 'source'
}

if (shift.type == ADDRESS_8) {
	shift = (uint8_t) shift; // zero-extend 'shift'
}

destination = source << shift; // copy the 'source' shifted left by 'shift' bits to 'destination'
```

## History

`shl` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.