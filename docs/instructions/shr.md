# The `shr` (bitwise shift right) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `shr`                 |
| Name                 | bitwise shift right   |
| `zed.h` denomination | `ZED_INSTRUCTION_SHR` |
| Opcode               | 0xf                   |
| Operands             | 3                     |

Copies the second operand (`source`) shifted right by the amount of bits denoted by the third operand (`shift`) to the first operand (`destination`).

If `source` is an 8-bit address, the value it points to will be padded to 64 bits with zeros before being shifted.

`source` and `shift` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
shr destination source shift
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

uint64_t padded_source = 0;

if (source.type == ADDRESS_8) {
	padded_source |= source_a & 0xFF; // make sure 'source' is padded to 64 bits with zeros
}

else {
	padded_source = source;
}

destination = (uint64_t) padded_source >> shift; // copy the 'padded_source' shifted right by 'shift' bits to 'destination'
                                                 // note the cast to 'uint64_t' to make sure this is *shifted* right, and not *rotated* right
```

## History

`shr` was introduced in ZEDv1, although it took only 2 operands before ZEDv5. In ZEDv4, the `ror` (rotate right) instruction was introduced, which was very similar to `shr`, but was ultimately removed for ZEDv5 due to lack of use.