ENTRY (_start)

SECTIONS
{
  . = 0x60010000;
_KERNEL_START_ = .;

  .text : {
_TEXT_START_ = .;
      *(.text)
_TEXT_END_ = .;
   }

  .data : {
_DATA_START_ = .;
      *(.data*)
_DATA_END_ = .;
  }


  .bss : {
_BSS_START_ = .;
      *(.bss*)
_BSS_END_ = .;
   }

_KERNEL_END_ = .;
}
