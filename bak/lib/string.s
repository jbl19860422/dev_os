DisplayString:
	PUSHW %BP
	MOVW %SP,%BP
	MOVW %BP,%SP
	POPW %BP
	RET
