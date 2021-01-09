# The `sub` (arithmetic subtract) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `sub`                 |
| Name                 | arithmetic subtract   |
| `zed.h` denomination | `ZED_INSTRUCTION_SUB` |
| Opcode               | 0x7                   |
| Operands             | 3                     |

Copies the subtraction of the second operand (`source_a`) by the third operand (`source_b`) to the first operand (`destination`).

If `source_a` or `source_b` is an 8-bit address, the value it points to will be sign-extended to 64 bits before subtraction.

`source_a` and `source_b` can each be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
sub destination source_a source_b
```

## Flags and registers affected

None

## Pseudocode

### C-like

```c
destination = source_a - source_b; // copy the subtraction of 'source_a' by 'source_b' to 'destination'
```

## History

`sub` was introduced in ZEDv1, although it took only 2 operands before ZEDv5.