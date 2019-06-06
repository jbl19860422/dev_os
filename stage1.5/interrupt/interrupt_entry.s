GLOBAL KeyBoard
GLOBAL common_int
GLOBAL TimerInt
GLOBAL MouseInt
[SECTION .text]
[BITS 32]
KeyBoard:
	MOV AL,020h
	OUT 0A0h,AL
	MOV AL,020h
	OUT 020h,AL
	IN AL, 060h
	MOV AH, 0Eh
	MOV AL, [KEY]
	MOV [0B8000h+140], AX
	ADD AL,1
	MOV [KEY],AL
	IRETD
KEY:	db 'b'

TimerInt:
	MOV AH, 0Ch
	MOV AL, [KEY1]
	MOV [0B8000h+60], AX
	ADD AL,1
	MOV [KEY1],AL
	MOV AL,020h
	OUT 020h,AL
	IRETD
KEY1:	db 'b'

MouseInt:
	CLI
	MOV AL,020h
	OUT 0A0h,AL
	MOV AL,020h
	OUT 020h,AL
	IN AL, 060h
	IN AL, 060h
	IN AL, 060h
	IN AL, 060h
	MOV AH, 06h
	MOV AL, [KEY2]
	MOV [0B8000h+80], AX
	ADD AL,1
	MOV [KEY2],AL
	MOV BL,AL
	STI
	IRETD
KEY2:	db 'd'

common_int:
	IRETD