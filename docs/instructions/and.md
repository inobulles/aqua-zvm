# The `and` (bitwise and) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `and`                 |
| Name                 | bitwise and           |
| `zed.h` denomination | `ZED_INSTRUCTION_AND` |
| Opcode               | 0xb                   |
| Operands             | 3                     |

Copies the bitwise and of the second operand (`source_a`) with the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be padded to 64 bits with zeros before being and'.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
and destination source_a source_b
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

uint64_t padded_source_a = 0;

if (source_a.type == ADDRESS_8) {
	padded_source_a |= source_a & 0xFF; // make sure 'source_a' is padded to 64 bits with zeros
}

else {
	padded_source_a = source_a;
}

uint64_t padded_source_b = 0;

if (source_b.type == ADDRESS_8) {
	padded_source_b |= source_b & 0xFF; // make sure 'source_b' is padded to 64 bits with zeros
}

else {
	padded_source_b = source_b;
}

destination = padded_source_a & padded_source_b; // copy the bitwise and of 'padded_source_a' and 'padded_source_b' to 'destination'
```

## History

`and` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.