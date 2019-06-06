ENTRY(_start)
SECTIONS
{
	iDriverNum = 2044+0x7c00;
	. = 0x8400;
	.text : {*(.text)}
	.data : {*(.data)}
	.rodata : {*(.rodata)}
	.bss : {*(.bss)}
	iSystemSize = .;
}
