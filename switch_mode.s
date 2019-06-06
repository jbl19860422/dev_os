%include "pm_def.inc"
GLOBAL switch_real_to_protect
GLOBAL switch_protect_to_real
[SECTION .text]
[BITS 16]
	switch_real_to_protect:
		/*save the ip first*/
		MOV AX,[ESP]
		MOV AX,[SavedIP]
		ADD ESP,2h
		LGDT [GDT_PTR]
		CLI
		IN AL, 092h
		OR AL, 0x2
		OUT 092h, AL
		MOV EAX,CR0
		OR EAX,01h
		MOV CR0,EAX
		JMP DWORD SelectorCode32:LabelCode32Start
[BITS 32]
	LabelCode32Start:
		MOV AX,SelectorData32
		MOV SS,AX
		MOV DS,AX
		MOV ES,AX
		/*restore the ip in stack,but use eip to replace*/
		SUB ESP,04h
		XOR EAX,EAX
		MOV AX,[SavedIP]
		MOV [ESP],EAX
		RET
		
[BITS 32]
	switch_protect_to_real:
		MOV AX,0h
		MOV SS,AX
		MOV DS,AX
		MOV ES,AX
		MOV EAX,CR0
		AND EAX,~01h
		MOV CR0,EAX
		JMP 0:LabelCode16Start
	LabelCode16Start:
		RET
		
[SECTION .data]
SavedIP:
	DW 0
SavedEIP:
	DD 0
	
[SECTION .gdt]
LABEL_GDT:
	DESCRIPTOR 0h,0h,0h
LABEL_DESC_CODE32:
	DESCRIPTOR 0h,FFFFFh,DA_TYPE_EXECUTE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
LABEL_DESC_DATA32:
	DESCRIPTOR 0h,FFFFFh,DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
LABEL_DESC_VIDEO:
	DESCRIPTOR B8000h,FFFFFh,DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMITBYTE
SelectorCode32 	EQU LABEL_DESC_CODE32-LABEL_GDT
SelectorData32 	EQU LABEL_DESC_DATA32-LABEL_GDT
SelectorVideo	EQU LABEL_DESC_VIDEO-LABEL_GDT
GtdLen EQU $-LABEL_GDT
GDT_PTR:
	DW 0x800
	DD LABEL_GDT

	
