
Links: 

- https://wiki.osdev.org/Inline_Assembly

Inline assembler examples:

Reading values only:

```
    void write(uint8 value) {
      asm volatile(
        "mov al, %0\n"
        "mov dx, %1\n"
        "outb dx, al\n"
        :
        : "r"(value), "r"(_number)
        : "rax", "rdx"
      );
    }
```

Writing values back:

```
     uint8 read() {
      uint8 result;
      asm volatile(
        "mov dx, %1\n"
        "inb al, dx\n"
        "mov %0, al\n"
        : "=r"(result)
        : "r"(_number)
        : "rax", "rdx"
      );
      return result;
    }
```

Working pure assembler PIC remap code:

```
    void remap() {
      asm volatile(
        "movb al, 0x11\n"
        "outb 0x20, al\n"
        "outb 0xa0, al\n"

        "movb al, 0x20\n"
        "outb 0x21, al\n"
        "movb al, 0x28\n"
        "outb 0xa1, al\n"

        "movb al, 0x4\n"
        "outb 0x21, al\n"
        "movb al, 0x2\n"
        "outb 0xa1, al\n"

        "movb al, 0x1\n"
        "outb 0x21, al\n"
        "movb al, 0x1\n"
        "outb 0xa1, al\n"

        "movb al, 0xfd\n"
        "outb 0x21, al\n"
        "movb al, 0xff\n"
        "outb 0xa1, al\n"
        ::
      );
    }
```
