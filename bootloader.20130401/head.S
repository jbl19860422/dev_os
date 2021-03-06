.global _start
.code16
.section .text
.section .data
_start: 
	MOVW %CS,%AX
	MOVW %AX,%DS
	MOVW %AX,%ES
	MOVW %AX,%FS
	MOVW $0,%AX
	MOVW %AX,%SS
	MOVW $0x7c00,%SP
	MOVW $DriverNum,%DI
	MOVW %DX,(%DI)
	PUSHW $_s_operate_msg
#	PUSHW %DX
#	CALL DisplayWord
#	ADD $2,%SP
#	JMP .
	CALL DisplayMsg
	ADD $2,%SP
	PUSHW %DX
	CALL checkInt13h
	ADD $2,%SP
	CMPW $1,%AX
	JZ Support13H
	JNZ NotSupport13H
Support13H:
	PUSHW $_driver_msg
	CALL DisplayMsg
	ADD $2,%SP
	MOVB $0,%DH
	PUSHW %DX
	CALL DisplayWord
	ADD $2,%SP
	PUSHW $_13h_msg
	CALL DisplayMsg
	ADD $2,%SP
	PUSHW $16  #sectornum
	PUSHW $1   #sectorcount
	PUSHW $0x800   #data offset
	PUSHW $0x07C0  #data segment
	CALL ReadSector
	ADD $8,%SP
	PUSHW $RootDirEntry
	PUSHW $0x800
	CALL ReadRootDirEntry
	ADD $4,%SP

	#PUSHW $RootDirEntry
	#PUSHW $17
	#CALL DisplayMem
	#ADD $4,%SP
	MOVW $RootDirEntry,%SI
	ADD $2,%SI
	MOVL (%SI),%EAX
	MOVL %EAX,(RootSectorNo)
	PUSHW (RootSectorNo)
	CALL DisplayWord
	ADD $2,%SP
	PUSHW (RootSectorNo)
        PUSHW $1   #sectorcount
        PUSHW $0x800   #data offset
        PUSHW $0x07C0  #data segment
        CALL ReadSector
        ADD $8,%SP
	PUSHW $0x800
        PUSHW $0x40
        CALL DisplayMem
        ADD $4,%SP
	PUSHW $_pre_find
	CALL DisplayMsg	
	ADD $2,%SP
	MOVB $0,%AH
	INT $0x16
	PUSHW $_return 
	CALL DisplayMsg	
	ADD $2,%SP
	PUSHW $0x800
	PUSHW $DirEntry
	CALL FindLoader
	ADD $4,%SP
	CMP $1,%AX
	JZ 1f
	JNZ 2f
1:
	PUSHW $_find_loader_msg
	JMP 3f
2:
	PUSHW $_not_find_loader_msg
3:
	CALL DisplayMsg
	ADD $2,%SP
	#PUSHW $DirEntry
	#PUSHW $25
	#CALL DisplayMem
	MOVW $DirEntry,%SI
	ADD $2,%SI
	MOVL (%SI),%EAX
	PUSHW %AX
	CALL DisplayWord
	ADD $2,%SP
	PUSHW $_return
	CALL DisplayMsg
	ADD $2,%SP
	MOVW $DriverNum,%DI
        MOVW (%DI),%DX
	PUSHW $_driver_msg
        CALL DisplayMsg
        ADD $2,%SP
	PUSHW %DX
	CALL DisplayWord
	ADD $2,%SP
	PUSHW %AX
	CALL DisplayWord
	ADD $2,%SP
	PUSHW %AX
	PUSHW $2
	PUSHW $0x800
	PUSHW $0x07c0
	CALL ReadSector
	ADD $8,%SP 
	PUSHW $_return
	CALL DisplayMsg
	ADD $2,%SP
	PUSHW $0x800
	PUSHW $40
	CALL DisplayMem
	ADD $4,%SP
	ADD $0x5678,%AX
	PUSHW _return
	CALL DisplayMsg
	ADD $2,%SP
	CALL GetMem
	PUSHW (MemConfCount)
	CALL DisplayWord
	ADD $2,%SP
	PUSHW $_return
	CALL DisplayMsg
	ADD $2,%SP
	#PUSHW (MemConfCount) 
	#PUSHW $MemConfBuff
	#CALL DisplayMemConf
	#ADD $4,%SP	
	#MOVW %AX,(MemSize)
	#PUSHW $_return
	#CALL DisplayMsg
	#ADD $2,%SP
	#PUSHW $_mem_size_msg
	#CALL DisplayMsg
	#ADD $2,%SP
	#PUSHW $MemSize
	#CALL DisplayWord
	#ADD $2,%SP
	LJMP $0x7c0,$0x800
NotSupport13H:
	PUSHW $_driver_msg
        CALL DisplayMsg
        ADD $2,%SP
        MOVB $0,%DH
        PUSHW %DX
        CALL DisplayWord
        ADD $2,%SP
	PUSHW $_no_13h_msg
	CALL DisplayMsg
	ADD $2,%SP
	JMP .
_pre_find:
	.ascii "going to find...\r\n"
	.byte 0
_find_loader_msg:
	.ascii "find loader haha\r\n"
	.byte 0
_not_find_loader_msg:
	.ascii "not find loader\r\n"
	.byte 0
_driver_msg:
	.ascii "driver no:"
	.byte 0
_13h_msg:
	.ascii " support 13h\r\n"
	.byte 0
_no_13h_msg:
	.ascii " not support 13h\r\n"
	.byte 0
_mem_size_msg:
	.ascii "our system's mem size is:"
	.byte 0
checkInt13h:
	PUSHW %BP
	MOVW %SP,%BP
	MOVW 4(%BP),%DX
	MOVB $0x41,%AH
	MOVW $0x55AA,%BX
	INT $0x13
	CMPW $0xAA55,%BX
	JNZ 1f   #no support
	JZ 2f    #support
1:
	MOVW $0,%AX
	JMP 3f
2:
	MOVW $1,%AX
	JMP 3f
3:
	MOVW %BP,%SP
	POPW %BP
	RET
DisplayWord:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %CX
	PUSHW %DX
	PUSHW %AX
	MOVW 4(%BP),%CX
	MOVB %CH,%AL
	SHR $4,%AL
	CALL DisplayAL
	MOVB %CH,%AL
	ANDB $0x0f,%AL
	CALL DisplayAL
	MOVB %CL,%AL
	SHR $4,%AL
	CALL DisplayAL
	MOVB %CL,%AL
	ANDB $0x0f,%AL
	CALL DisplayAL
	POPW %AX
	POPW %DX
	POPW %CX
	MOVW %BP,%SP
	POPW %BP
	RET
DisplayMem:
        PUSHW %BP
        MOVW %SP,%BP
        MOVW 4(%BP),%CX
        MOVW 6(%BP),%SI
1:
        PUSHW (%SI)
        CALL DisplayWord
        ADD $2,%SP
        ADD $2,%SI
        DEC %CX
        JNZ 1b
	MOVW %BP,%SP
        POPW %BP
        RET
DisplayAL:
	PUSHW %BX
	PUSHW %AX
	CMP $10,%AL
	JL 1f
	SUBB $10,%AL
	ADDB $'A',%AL
	JMP 2f
1:
	ADDB $'0',%AL
2:
	MOVW $7,%BX
	MOVB $0xe,%AH
	INT $0x10	
	POPW %ax
	POPW %bx	
	RET

DisplayMemConf:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %DX
	PUSHW %CX
	PUSHW %BX
	PUSHW %AX
	MOVW 4(%BP),%SI
	MOVW 6(%BP),%CX
	PUSHW $_mem_conf_head
	CALL DisplayMsg
	ADD $2,%SP
1:
	MOVW $5,%DX
2:
	PUSHW %SI
	CALL DisplayLong
	ADD $2,%SP
	PUSHW $TabMsg
	CALL DisplayMsg
	ADD $2,%SP
	ADD $4,%SI
	DEC %DX
	JNZ 2b
	PUSHW $_return
	CALL DisplayMsg	
	ADD $2,%SP
	DEC %CX
	CMP $0,%CX
	JZ 3f
	JMP 1b
3:
	POPW %AX
	POPW %BX
	POPW %CX
	POPW %DX
	MOVW %BP,%SP
	POPW %BP
	RET
TabMsg:
	.ascii "  "
	.byte 0
DisplayMsg:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %SI
	PUSHW %DX
	PUSHW %AX
	MOVW 4(%BP),%SI
	STI
	CLD
1:
        LODSB
        ANDB %AL,%AL
        JZ 2f
        MOVB $0xE,%AH
        MOVW $7,%BX
        INT $0x10
        JMP 1b
2:
	POPW %AX
	POPW %DX
	POPW %SI
	MOVW %BP,%SP
	POPW %BP
	RET

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
ReadRootDirEntry:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %SI
	PUSHW %DI
	PUSHW %DX
	PUSHW %CX
	PUSHW %BX
	PUSHW %AX
	MOVW $17,%CX
	MOVW 4(%BP),%SI
	MOVW 6(%BP),%DI
	ADDW $156,%SI
1:
	MOVW (%SI),%AX
	MOVW %AX,(%DI)
	ADD $2,%SI
	ADD $2,%DI
	DEC %CX
	JNZ 1b
	POPW %AX
	POPW %BX
	POPW %CX
	POPW %DX
	POPW %DI
	POPW %SI
	MOVW %BP,%SP
	POPW %BP
	RET 
CopyMem:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %SI
	PUSHW %DI
	PUSHW %CX
	PUSHW %AX
	MOVW 8(%BP),%SI
	MOVW 6(%BP),%DI
	MOVW 4(%BP),%CX
1:
	MOVB (%SI),%AL
	MOVB %AL,(%DI)
	INC %SI
	INC %DI
	DEC %CX
	JNZ 1b
	POPW %AX
	POPW %CX
	POPW %DI
	POPW %SI
	MOVW %BP,%SP
	POPW %BP
	RET
CmpStr:
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %SI
	PUSHW %DI
	PUSHW %BX
	MOVW $0,%AX
	MOVW 6(%BP),%SI
	MOVW 4(%BP),%DI
1:
	MOVB (%SI),%BL
	MOVB (%DI),%BH
	INC %SI
	INC %DI
	CMP $0,%BL
	JZ 2f
	JNZ 4f
2:
	CMP $0,%BH
	JZ 3f
	JNZ 4f
3:
	MOVW $1,%AX
	JMP 7f
	RET
4:
	CMP %BL,%BH
	JNZ 5f
	JZ 6f
5:
	MOVW $0,%AX
	JMP 7f
6:
	JMP 1b
7:
	POPW %BX
	POPW %DI
	POPW %SI
	MOVW %BP,%SP
	POPW %BP
	RET		
FindLoader:
	PUSHW %BP
	MOVW %SP,%BP
	MOVW $0,%AX
	MOVW 6(%BP),%SI    #address of address
	MOVW 4(%BP),%DI    #address of
	PUSHW %DX 
	SUB $2,%SP
	MOVW %SI,-2(%BP)
1:
	MOVB $0,%DH
	MOVB (%SI),%DL
	CMP $0,%DL
	JZ 2f
	PUSHW %SI
	PUSHW $DirEntry
	PUSHW %DX
	CALL CopyMem
	ADD $6,%SP
	PUSHW %AX
	PUSHW $LoaderName
	MOVW %SI,%BX
	ADDW $33,%BX
	PUSHW %BX
	CALL CmpStr
	ADD $4,%SP
	MOVW %AX,%BX
	POPW %AX
	CMP $1,%BX
	JZ 3f
	ADDW %DX,%SI
	JMP 1b
3:
	MOVW $1,%AX
2:
	ADD $2,%SP
	POPW %DX
	MOVW %BP,%SP
	POPW %BP
	RET
DisplayLong:	
	PUSHW %BP
	MOVW %SP,%BP
	PUSHW %SI
	MOVW 4(%BP),%SI
	ADD $2,%SI
	PUSHW (%SI)
	CALL DisplayWord
	ADD $2,%SP
	SUB $2,%SI
	PUSHW (%SI)
	CALL DisplayWord
	ADD $2,%SP
	POPW %SI
	MOVW %BP,%SP
	POPW %BP
	RET
GetMem:
	PUSHW %BP
	MOVW %SP,%BP
	MOVL $0xe820,%EAX
	MOVW $MemConfBuff,%DI
	MOVL $0x20,%ECX
	MOVL $0,%EBX
	MOVL $0x534d4150,%EDX
1:
	MOVL $0xe820,%EAX
	MOVL $0x534d4150,%EDX
	MOVL $20,%ECX
	INT $0x15
	JC ReadMemError
	MOVW $MemConfCount,%SI
	MOVW (%SI),%DX
	INC %DX
	MOVW %DX,(%SI)
	ADD $20,%DI
	CMP $0,%EBX
	JE ReadMemOK
	JMP 1b
ReadMemError:
	PUSHW $ReadMemErrMsg
	CALL DisplayMsg
	ADD $2,%SP
	JMP 2f	
ReadMemOK:
	PUSHW $ReadMemOkMsg
	CALL DisplayMsg
	ADD $2,%SP
	PUSHW $_return
	CALL DisplayMsg
	ADD $2,%SP
2:
	MOVW %BP,%SP
	POPW %BP
	RET
#.include "lib/string.s"
ReadMemOkMsg:
	.ascii "Read Mem Ok"
	.byte 0
ReadMemErrMsg:
	.ascii "Read Mem Err"
	.byte 0
ReadErrMsg:
	.ascii "Read Err"
	.byte 0
ReadOkMsg:
	.ascii "Read Ok haha \r\n"
	.byte 0
LoaderName:
        .ascii "LOADER.BIN"
        .byte 0
DiskAddressPacket:
PacketSize=.-DiskAddressPacket
	.BYTE 0x10
Reserved=.-DiskAddressPacket
	.BYTE 0 
BlockCount=.-DiskAddressPacket
	.WORD 1
BufferOff=.-DiskAddressPacket
	.WORD 0x0400
BufferSeg=.-DiskAddressPacket
	.WORD 0x07c0 
BlockNum0=.-DiskAddressPacket
	.LONG 16
BlockNum1=.-DiskAddressPacket
	.LONG 0
MemConfCount:
        .WORD 0
MemConfBuff:
.rept 300
.byte 0
.endr
_return:
.ascii "\r\n"
.byte 0
_s_operate_msg:
.ascii "Wellcome to our Operating System v9\r\n"
.byte 0
_mem_conf_head:
.ascii "AddLow    AddHigh   LenLow    LenHigh   Type\r\n"
.byte 0
MemSize:
        .WORD 0
RootSectorNo:
        .WORD 0
RootDirEntry:
.rept 34
.byte 0
.endr
DirEntry:
.rept 50
.byte 0
.endr
.org 2044
DriverNum:
	.WORD 0
.word 0xAA55
