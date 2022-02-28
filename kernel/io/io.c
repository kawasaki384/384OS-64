#include <stdio.h>
#include <stdint.h>

void outb(unsigned short port, unsigned char val)
{
    /* バイト(8bit)サイズでI/Oポートに値を出力	*/
    __asm__ __volatile__(
        "outb %1, %0"
        :
        : "d"(port), "a"(val));
}

unsigned char inb(unsigned short port)
{
    /* バイト(8bit)サイズでI/Oポートから値を得る
    ** ここではインラインアセンブラ(AT&T記法)を用いた
    */
    unsigned char val;
    __asm__ __volatile__(
        "inb %1, %0\n\t"
        : "=a"(val)
        : "d"(port));
    return val;
}

