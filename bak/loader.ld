ENTRY(_start)
SECTIONS
{
	. = 0x800;
	.text : {*(.text)}
	.data : {*(.data)}
}
