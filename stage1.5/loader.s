%include "pm.inc"
;ORG 0800h
;EXTERN lmain
EXTERN lmain1_5
EXTERN iSystemSize
EXTERN switch_real_to_protect
EXTERN switch_protect_to_real
EXTERN _end
EXTERN pEnd
EXTERN ReadMemConf
EXTERN InitPageConf
EXTERN g_iMemConfCount
EXTERN g_memConf
EXTERN g_pMemMap
EXTERN PrintMsg16
EXTERN PrintMsg
EXTERN PrintWord
EXTERN g_iDriverNum
EXTERN InitCDRom
GLOBAL _start
[SECTION .text]
[BITS 16]
_start:
JMP LabelStart16
[SECTION .text]
[BITS 16]
LabelStart16:
	PUSH CS		;由于nasm不支持远程调用使用寄存器作为段（只能是立即数，所以先push了）
	CALL switch_real_to_protect
[BITS 32]
	MOV EAX,[08400h-4]
	MOV [g_iDriverNum],EAX
	MOV ESP,08400h
	CALL lmain1_5
	JMP $

