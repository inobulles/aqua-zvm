# The `sel` (bit select) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `sel`                 |
| Name                 | bit select            |
| `zed.h` denomination | `ZED_INSTRUCTION_SEL` |
| Opcode               | 0xa                   |
| Operands             | 3                     |

Selects all the bits within the second operand (`source`), starting at the lowest bit, at the positions where the third operand's (`mask`) bits are set, and lines them up in the first operand (`destination`), also starting from the lowest bit.

If `source` or `mask` is an 8-bit address, the value it points to will be padded to 64 bits with zeros before being masked.

`source` and `mask` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
sel destination source mask
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

uint64_t padded_source = 0;

if (source.type == ADDRESS_8) {
	padded_source |= source & 0xFF; // make sure 'source' is padded to 64 bits with zeros
}

else {
	padded_source = source;
}

uint64_t padded_mask = 0;

if (mask.type == ADDRESS_8) {
	padded_mask |= mask & 0xFF; // make sure 'mask' is padded to 64 bits with zeros
}

for (int i, j = 0; i < 64 && j < destination.length; i++) {
	if (padded_mask.bits[i]) {
		destination.bits[j] = padded_source.bits[i];
		j++;
	}
}
```

## History

`sel` was introduced in ZEDv5.
