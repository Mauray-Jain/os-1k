ENTRY(boot) /* declare boot as the entry point */

SECTIONS {
	. = 0x80200000; /* base address */

	/* contains code */
	.text :{
		KEEP (*(.text.boot)); /* place .text.boot at beginning */
		*(.text .text.*); /* places the .text section and any sections starting with .text. from all files (*) at that location.*/
	}

	/* Read only const data */
	.rodata : ALIGN(4) {
		*(.rodata .rodata.*);
	}

	/* RW data */
	.data : ALIGN(4) {
		*(.data .data.*);
	}

	/* RW data with initial value of 0 */
	.bss : ALIGN(4) {
		__bss = .;
		*(.bss .bss.* .sbss .sbss.*);
		__bss_end = .;
	}

	/* kernel stack of 128kb */
	. = ALIGN(4);
	. += 128 * 1024; /* 128KB */
	__stack_top = .;
}
