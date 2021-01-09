# The `mov` (move) instruction

## Description

| Property             | Value                 |
|----------------------|-----------------------|
| Mnemonic             | `mov`                 |
| Name                 | move                  |
| `zed.h` denomination | `ZED_INSTRUCTION_MOV` |
| Opcode               | 0x0                   |
| Operands             | 2                     |

Copies the second operand (`source`) to the first operand (`destination`).

If `source` is an 8-bit address, the value it points to will be copied to the 8 lowest bits of `destination`.
If `destination` is an 8-bit address, it'll copy the 8 lowest bits of `source` to the value it points to.

`source` can be a register, a constant, an address (8 or 64-bit), a kfunc index, a position index, or a data index.
`destination` can be a register or an address (8 or 64-bit).

## ZASM syntax

```zasm
mov destination source
```

## Flags and registers affected

None.

## Pseudocode

### C-like

```c
if (destination.type == ADDRESS_8) {
	destination = source & 0xFF; // truncate to 8 lowest bits of 'source' and copy that to 'destination'
}

else if (source.type == ADDRESS_8) {
	destination ^= 0xFF; // clear 8 lowest bits
	destination |= source; // copy 'source' to the 8 lowest bits
}

else {
	destination = source; // copy 'source' to 'destination'
}
```

## History

`mov` was introduced in ZEDv1.
