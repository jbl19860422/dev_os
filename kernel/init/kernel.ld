ENTRY(_start)
SECTIONS
{
	. = 0x100000;
	.text : {*(.text)}
	.data : {*(.data)}
	.gdt : {*(.gdt)}
	.rodata : {*(.rodata)}
	.bss : {*(.bss)}
	vVGA = 0xB8000;
	_end = .;
}
