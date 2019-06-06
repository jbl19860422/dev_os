ENTRY(_start)
SECTIONS
{
	iDriverNum = 2044;
	. = 0x800;
	.text : {*(.text)}
	.data : {*(.data)}
	.rodata : {*(.rodata)}
	.bss : {*(.bss)}
	iSystemSize = .;
}
