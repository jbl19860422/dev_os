%include "pm.inc"
GLOBAL switch_real_to_protect
GLOBAL switch_protect_to_real
[SECTION .text]
[BITS 16]
	switch_real_to_protect:
		;save the ip first
		MOV AX,[ESP]
		MOV [RealIP],AX
		ADD ESP,2h
		MOV AX,[ESP]
		MOV [RealCS], AX
		ADD ESP,2h
		LGDT [GDT_PTR]
		CLI
		IN AL, 092h
		OR AL, 02h
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
		MOV FS,AX
		MOV GS,AX
		;restore the ip in stack,but use eip to replace
		XOR EAX,EAX
		MOV AX,[RealCS]
		SHL EAX, 16
		ADD AX,[RealIP]
		PUSH EAX		;��eippush����ջ��
		RET
[BITS 32]
	;ע����switch_protect_to_real�󣬳�������ò��ã��������
	switch_protect_to_real:
		;save the eip first
		MOV EAX,[ESP]
		MOV [ProtectEIP],EAX
		ADD ESP,04h
		;load the 16bits code selector
		MOV AX, SelectorData16
		MOV SS,AX
		MOV DS,AX
		MOV ES,AX
		MOV FS,AX
		MOV GS,AX
		JMP SelectorCode16:LabelCode16StartTmp
[BITS 16]
	LabelCode16StartTmp:
		MOV EAX,CR0
		AND AL,~01h
		MOV CR0,EAX
		;shutdown a20
		IN AL, 092h
		AND AL,~02h
		OUT 092h,AL
		JMP 0h:LabelCode16Start
	LabelCode16Start:
		;restore the stack(put ip into the stack)
		MOV AX, 0
		MOV SS,AX ;ssһֱ��0�����ܱ仯
		MOV DS,AX
		MOV ES,AX
		MOV FS,AX
		MOV GS,AX
		MOV EAX,[ProtectEIP]
		AND EAX,0f0000h      ;eip ���ᳬ��1m��û����
		SHR EAX,04h			  ;�õ�cs
		PUSH AX				  ;CS PUSH����ջ��
		MOV EAX,[ProtectEIP]
		PUSH AX				  ;IP PUSH����ջ��	
		RETF				  ;Զ�̷��� 
		
RealIP:
	DW 0
RealCS:
	DW 0
ProtectEIP:
	DD 0
LABEL_GDT:
	DESCRIPTOR 0h,0h,0h
LABEL_DESC_CODE32:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_EXECUTE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMIT4K
LABEL_DESC_DATA32:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMIT4K
LABEL_DESC_CODE16:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_EXECUTE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_STACK_SP16|DA_G_LIMIT4K
LABEL_DESC_DATA16:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_DATA_WRITE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_STACK_SP16|DA_G_LIMIT4K
LABEL_DESC_INTERRUPT:
	DESCRIPTOR 0h,0FFFFFh,DA_TYPE_EXECUTE|DA_S_DC|DA_DPL_0|DA_P_1|DA_DB_SEGLIMIT_4G|DA_G_LIMIT4K
SelectorCode32 	EQU LABEL_DESC_CODE32-LABEL_GDT
SelectorData32 	EQU LABEL_DESC_DATA32-LABEL_GDT
SelectorCode16	EQU LABEL_DESC_CODE16-LABEL_GDT
SelectorData16 	EQU LABEL_DESC_DATA16-LABEL_GDT
SelectorInt32	EQU LABEL_DESC_INTERRUPT-LABEL_GDT		;�жϳ����������
GtdLen EQU $-LABEL_GDT
GDT_PTR:
	DW 0x800
	DD LABEL_GDT
