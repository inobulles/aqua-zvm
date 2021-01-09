# The `cal` (kfunc call) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `cal`                 |
| Name                 | kfunc call            |
| `zed.h` denomination | `ZED_INSTRUCTION_CAL` |
| Opcode               | 0x4                   |
| Operands             | 1                     |

Calls the kfunc (KOS function) denoted by the first operand (`kfunc`).

`kfunc` can only be a kfunc index.

## ZASM syntax

```zasm
cal kfunc
```

## Flags and registers affected

The first general purpose register (`g0`) is set to the value returned by the kfunc.

## Pseudocode

### C-like

```c
g0 = kfunc_pointers[kfunc](a0, a1, a2, a3); // call the kfunc at index 'kfunc' and store the return value in 'g0'
                                            // the argument registers ('a0', 'a1', 'a2', and 'a3') are set depending on the kfunc in question
```

## History

`cal` was introduced in ZEDv1, although it was also used as a call routine instruction before ZEDv5.
