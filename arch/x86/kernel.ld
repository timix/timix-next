ENTRY (_start)

SECTIONS
{
  . = VOFF;
  .text : { *(.text*) *(.rodata*) }
  .data : { *(.data*) }
  .bss : { *(.bss*) *(COMMON*) }
}
