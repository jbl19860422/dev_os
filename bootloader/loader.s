%include "pm.inc"
;ORG 0800h
EXTERN lmain
EXTERN iSystemSize
GLOBAL _start
[SECTION .text]
[BITS 16]
_start:
JMP LabelStart16
[SECTION .GDT]
LABEL_GDT:
	DESCRIPTOR 0h,0h,0h
LABEL_DESC_CODE32:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_EXECUTE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
LABEL_DESC_DATA32:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
LABEL_DESC_VIDEO:
	DESCRIPTOR 0B8000h, 0FFFFFh, DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
SelectorCode32		EQU	(LABEL_DESC_CODE32-LABEL_GDT)
SelectorVideo		EQU 	(LABEL_DESC_VIDEO-LABEL_GDT)
GtdLen			EQU	$-LABEL_GDT
GDT_PTR:
	DW 800h
	DD LABEL_GDT
LabelStart16:
	CALL lmain
	MOV DX,[iSystemSize]
	JMP DX
	LGDT [GDT_PTR]
	CLI
	IN AL,92h
	OR AL,02h
	OUT 92h,AL
	MOV EAX,CR0
	OR EAX,01h
	MOV CR0,EAX
	JMP dword SelectorCode32:(07c00h+LabelStart32)
[SECTION .S32]
[BITS 32]
LabelStart32:
	MOV AX,SelectorVideo
	MOV GS,AX
	MOV EDI,(80*10+0)*2
	MOV AH,0ch
	MOV AL,'P'
	MOV [GS:EDI],AX
	JMP $ 
