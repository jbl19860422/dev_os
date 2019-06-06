ENTRY(_start)
SECTIONS
{
	. = 0x7c00;
	DriverNum = 444;
	.text : {*(.text)}
	.data	: {*(.data)}
}
