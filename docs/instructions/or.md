# The `or` (bitwise or) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `or`                  |
| Name                 | bitwise or            |
| `zed.h` denomination | `ZED_INSTRUCTION_OR`  |
| Opcode               | 0xc                   |
| Operands             | 3                     |

Copies the bitwise or of the second operand (`source_a`) with the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be zero-extended to 64 bits before or'ing.
If `destination` is an 8-bit address, it'll truncate the result of the bitwise or to its 8 lowest bits before copying.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
or  destination source_a source_b
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

if (source_a.type == ADDRESS_8) {
	source_a = (uint8_t) source_a; // zero-extend 'source_a'
}

if (source_b.type == ADDRESS_8) {
	source_b = (uint8_t) source_b; // zero-extend 'source_b'
}

destination = padded_source_a | padded_source_b; // copy the bitwise or of 'source_a' and 'source_b' to 'destination'
```

## History

`or` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.
As of ZEDv4, it's the only option who's mnemonic isn't spelled with 3 letters.