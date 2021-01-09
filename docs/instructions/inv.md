# The `inv` (invert) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `inv`                 |
| Name                 | invert                |
| `zed.h` denomination | `ZED_INSTRUCTION_INV` |
| Opcode               | 0x2                   |
| Operands             | 2                     |

Copies the inverse of each of the second operand's (`source`) bits to the first operand (`destination`).

`source` can be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
inv destination source
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c
destination = ~source; // copy the inverse of 'source' to 'destination' (equivalent to taking it's ones' complement)
```

## History

`inv` was introduced in ZEDv1 as the `not` (bitwise not) instruction.
It is known as the `inv` (invert) instruction as of ZEDv5.
