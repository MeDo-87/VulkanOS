/* Link.ld -- Linker script for the kernel - ensure everything goes in the */
/*            Correct place.  */
/*            Original file taken from Bran's Kernel Development */
/*            tutorials: http://www.osdever.net/bkerndev/index.php. */

ENTRY(start)
SECTIONS
{
  . = 1M;
  .text BLOCK(4K) : ALIGN(4K)
  {
    code = .; _code = .; __code = .;
    *(.text)
  }

  .data :
  {
      __CTOR_LIST__ = .; LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2)*
                                          (.ctors)   LONG(0) __CTOR_END__ = .;
      __DTOR_LIST__ = .; LONG((__DTOR_END__ - __DTOR_LIST__) / 4 - 2)*
                                         (.dtors) LONG(0) __DTOR_END__ = .;
     data = .; _data = .; __data = .;
     *(.data)
     *(.rodata)
     . = ALIGN(4096);
  }

  .bss :
  {
    bss = .; _bss = .; __bss = .;
    *(.bss)
    *(.bootstrap_stack)
    . = ALIGN(4096);
  }

  end = .; _end = .; __end = .;
} 
