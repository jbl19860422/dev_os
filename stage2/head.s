.code16
.section .text
.global _start
_start:
	PUSHW $_s_operate_msg
        CALL DisplayMsg
        ADD $2,%SP
	JMP .
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
.section .data
_s_operate_msg:
.ascii "IN KERNEL.\r\n"
.byte 0


