# The `jmp` (jump) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `jmp`                 |
| Name                 | jump                  |
| `zed.h` denomination | `ZED_INSTRUCTION_JMP` |
| Opcode               | 0x3                   |
| Operands             | 1                     |

Transfers program control to the position denoted by the first operand (`position`) in the instruction stream.

`position` can only be a position index.

## ZASM syntax

```zasm
jmp position
```

## Flags and registers affected

`position` is usually copied to the instruction pointer register (`ip`), although the specific implementation may choose to handle jumps differently and thus not affect `ip`.

## Pseudocode

### C-like

```c
ip = position; // set 'ip' to 'position' (not necessarily the case on all implementations)
goto position; // jump to 'position'
```

## History

`jmp` was introduced in ZEDv1.
