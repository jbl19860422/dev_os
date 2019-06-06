[SECTION .text]
[BITS 16]
ReadSector:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %CX
	MOVB $0x42,%AH
	MOVW $DiskAddressPacket,%BX
	MOVW $PacketSize,%SI
	MOVB $0x10,(%BX,%SI,1)
	MOVW $BlockCount,%SI
	MOVW 8(%BP),%CX
	MOVW %CX,(%BX,%SI,1)
	MOVW $BufferOff,%SI
	MOVW 6(%BP),%CX
	MOVW %CX,(%BX,%SI,1)
	MOVW $BufferSeg,%SI
	MOVW 4(%BP),%CX
	MOVW %CX,(%BX,%SI,1)
	MOVW $BlockNum0,%SI
	MOVW 10(%BP),%CX
	movw %CX,(%BX,%SI,1)
	MOVW $BlockNum1,%SI
	MOVL $0,(%BX,%SI,1)
	MOVW $DiskAddressPacket,%SI
	INT $0x13
	#read error
	JC 1f
	#read ok     
	JNC 2f
1:
	MOVW $0,%AX
	PUSHW $ReadErrMsg
	CALL DisplayMsg
	ADD $2,%SP
	PUSHW $_return
	CALL DisplayMsg
	ADD $2,%SP
	JMP 3f
2:
	MOVW $1,%AX
	PUSHW $ReadOkMsg
	CALL DisplayMsg
	ADD $2,%SP
3:
	POPW %CX
	MOVW %BP,%SP
	POPW %BP
	RET