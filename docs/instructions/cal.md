# The `cal` (kfunc call) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `cal`                 |
| Name                 | kfunc call            |
| `zed.h` denomination | `ZED_INSTRUCTION_CAL` |
| Opcode               | 0x4                   |
| Operands             | 2                     |

Calls the kfunc (KOS function) denoted by the second operand (`kfunc`) and copies the return value to the first operand (`retval`).

If `kfunc` is an 8-bit address, the value it points to will be zero-extended to 64 bits before calling.
If `retval` is an 8-bit address, it'll truncate the return value of the kfunc to its 8 lowest bits before copying.

`kfunc` can be a register, an address (8 or 64-bit), or a kfunc index.
`retval` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
cal retval kfunc
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c++
#include <stdint.h>

if (kfunc.type == ADDRESS_8) { // on most platforms this isn't possible, which is why it isn't included in the default ZVM
	kfunc = (uint8_t) kfunc; // zero extend 'kfunc'
}

retval = kfunc(a0, a1, a2, a3); // call the kfunc at index 'kfunc' and copy the return value to 'retval'
                            // the argument registers ('a0', 'a1', 'a2', and 'a3') are set by the user before calling depending on the kfunc in question
```

## History

`cal` was introduced in ZEDv1, although it was also used as a call routine instruction before ZEDv5.
