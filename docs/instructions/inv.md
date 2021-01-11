# The `inv` (bit invert) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `inv`                 |
| Name                 | bit invert            |
| `zed.h` denomination | `ZED_INSTRUCTION_INV` |
| Opcode               | 0x2                   |
| Operands             | 2                     |

Copies the inverse of each of the second operand's (`source`) bits to the first operand (`destination`).

If `source` is an 8-bit address, the value it points to will be zero-extended to 64 bits before inversion.
If `destination` is an 8-bit address, it'll truncate the result of the inversion to its 8 lowest bits before copying.

`source` can be a register, a constant, an address (8 or 64-bit), a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
inv destination source
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

destination = ~source; // copy the bit inverse of 'source' to 'destination' (equivalent to taking it's ones' complement)
```

## History

`inv` was introduced in ZEDv1 as the `not` (bitwise not) instruction.
It is known as the `inv` (bit invert) instruction as of ZEDv5.
