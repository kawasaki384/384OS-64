#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <stdalign.h>
#include <KernelArg.h>
#include <test_header.h>
#include <stdlib.h>

#include "io/io.h"
#include "driver/serialport/serialport.h"
#include "driver/define_use.h" //ドライバ関連の条件コンパイルの設定。
#include "driver/localapic/timer/l_apic_timer.h"
#include "driver/localapic/local_apic.h"

#include "gdt/gdt_func.h"
#include "gdt/segment.h" 
#include "paging/paging.h"
#include "mem/memmgr.h"

#include "idt/interrupt.h"

#include "args/memmap.h"
#include "args/memory.h"

//https://debimate.jp/2019/02/02/linux-kernel-prinkprint-kernel%E3%81%AB%E3%82%88%E3%82%8B%E3%83%A1%E3%83%83%E3%82%BB%E3%83%BC%E3%82%B8%E5%87%BA%E5%8A%9B/

int CUI_X, CUI_Y;
KAGraphic *KGraphicArgs;
PSF1_FONT *psffont;
MemoryMap *memmap;

typedef enum FONT_COLOR
{
    emerg = 0xff2222,
    alert = 0xcc2222,
    crit = 0xffa333,
    err = 0xaf7f33,
    warning = 0x873788,
    notice = 0x89bdde,
    info = 0x67ad67,
    debug = 0x22aaaa,
    Default = 0xdfdfdf,
    cont = 0xdddddd,
} fcolor;

typedef enum tarKERN_LOG
{
    KERN_EMERG,   //システム停止前の緊急メッセージ
    KERN_ALERT,   //早急な対応が必要なエラー
    KERN_CRIT,    //HardWareもしくはSoftWareの致命的なエラー
    KERN_ERR,     //ドライバ共通のエラー
    KERN_WARNING, //警告(エラーとなる可能性がある)
    KERN_NOTICE,  //注意(エラーではない)
    KERN_INFO,    //情報通知
    KERN_DEBUG,   //デバッグメッセージ専用
    KERN_DEFAULT, //デフォルトのログレベル
    KERN_CONT     //ログの継続(タイムスタンプの更新を回避、ブート中専用)
} KERN_LOG;

void WriteHexc(int x, int y, unsigned int color);
void DrawRectangle(int x, int y, int x1, int y1, unsigned int color);
int printk(const char *fmt, ...);
int printLog(KERN_LOG kernlog_type, const char *fmt, ...);
#define MEMORY_CODE //メモリのコードを使う。

void test_printk(){
    printk("Inithandler!!\n");
}

alignas(16) uint8_t kernel_main_stack[1024 * 1024];

void KernelMain_NewStack(KAGraphic *KernelGraphicArgs, PSF1_FONT *finishedFont,MemoryMap *kmemmap)
{
    InitializeInterrupt();
    InitializeAPICTimer();
    CUI_X = 0;
    CUI_Y = 0;
    KGraphicArgs = KernelGraphicArgs;
    psffont = finishedFont;
    memmap = kmemmap;
    SetupSegments();
    const uint16_t kernel_cs = 1 << 3;
    const uint16_t kernel_ss = 2 << 3;
    SetDSAll(0);
    SetCSSS(kernel_cs, kernel_ss);
    SetupIdentityPagetable();
    int Bool_Initmem = InitializeMemoryManager(memmap);

    switch (KernelGraphicArgs->PixelFormat)
    {
    case PixelRedGreenBlueReserved8BitPerColor:
        break;
    case PixelBlueGreenRedReserved8BitPerColor:
        break;
    case PixelBitMask:
        break;
    case PixelBltOnly:
        break;
    case PixelFormatMax:
        break;

    default:
        printk("default");
        break;
    }

    /********************************MEMORY_PROGRAM***********************************************/
    #ifdef MEMORY_CODE

    printLog(KERN_INFO, "InitMemory:%d", Bool_Initmem);

    printLog(KERN_INFO, "%08lx %08lx", kmemmap->buffer,kmemmap->map_size); 
    //ここのフォーマットの文字列が%dだったら少なくともkmemmap->bufferはオーバーフローする。(実機メモリ8GB環境)
    
    //init_serialport();
    uint64_t memorysize = 0;
    for (uint64_t i = 0; i < kmemmap->map_size / kmemmap->descriptor_size;i++)
    {
        EFI_MEMORY_DESCRIPTOR *desc = (EFI_MEMORY_DESCRIPTOR *)((uint64_t)kmemmap->buffer + (i * kmemmap->descriptor_size));
        memorysize += desc->NumberOfPages * 4096;
    }
    //printLogのpは小文字
    printLog(KERN_INFO,"RAM_SIZE:%dMB", memorysize / 1048576);
    #endif //MEMORY_CODE
    /******************************************************************************************/

/*
    printLog(KERN_EMERG, "suioasu");
    printLog(KERN_ALERT, "suioasu");
    printLog(KERN_CRIT, "suioasu");
    printLog(KERN_ERR, "suioasu");
    printLog(KERN_WARNING, "suioasu");
    printLog(KERN_NOTICE, "suioasu");
    printLog(KERN_INFO, "suioasu");
    printLog(KERN_DEBUG, "suioasu");
    printLog(KERN_DEFAULT, "suioasu");
*/

    capsella_function();
}

/*
KernelArgs

*/

// WriteHexColor
void WriteHexc(int x, int y, unsigned int color)
{
    *(unsigned int *)((unsigned int *)KGraphicArgs->FrameBufferBase + x +
                      (y * KGraphicArgs->PixelsPerScanLine)) = color;
    /*
        PixelsPerScanLine = 一ラインごとの横のピクセル
        HorizontalResolution,VerticalResolutionとは違う。
        unsigned intはビットはuint8_t*の4倍だけどこれは
        一つのピクセルの大きさがそれぞれ
        R(8bit),G(8bit),B(8bit),S(8bit) or
        B(8bit),G(8bit),R(8bit),S(8bit) で4つだから SはSpaceの意味。
        |_______|_______|_______|____全て合わせて整数32bit->unsigned intだ!!
        という訳。

        詳しくはゼロからのOS自作100p図4.1参照
    */
}

#define CHECK_GRAPHICFN_ARGS definefalse // GraphicFunctionが引数をチェックしない。

void DrawRectangle(int x, int y, int x1, int y1, unsigned int color)
{
#if CHECK_GRAPHICFN_ARGS == definetrue
    // x,y,x1,y1は正の数でなければならず、x<=x1,y<=y1でなければならない。
    if ((x < 0 || y < 0 || x1 < 0 || y1 < 0) || (x <= x1 || y <= y1))
        return;
#endif
    for (int y_0 = y; y_0 < y1; y_0++)
        for (int x_0 = x; x_0 < x1; x_0++)
            WriteHexc(x_0, y_0, color);
}

void new_line()
{
    //#define CLEAR_Y_0_16
    //最適化することで処理速度がだいぶ変わりそうな部分、よく考える。
    int CUI_Y_OF_FONT = KGraphicArgs->VerticalResolution - KGraphicArgs->VerticalResolution % 16;
    #ifdef CLEAR_Y_0_16
    int CUI_X_OF_FONT = KGraphicArgs->HorizontalResolution - KGraphicArgs->HorizontalResolution % 8;
    for (int y = 0; y < 16; y++)
        for (int x = 0; x < CUI_X_OF_FONT; x++)
            *(unsigned int *)((unsigned int *)KGraphicArgs->FrameBufferBase + x + (y * KGraphicArgs->PixelsPerScanLine)) = 0x000000;
    #endif /*CLEAR_Y_0_16*/
    for (int y = 16; y < CUI_Y_OF_FONT; y++)
        for (int x = 0; x < KGraphicArgs->HorizontalResolution; x++)
            *(unsigned int *)((unsigned int *)KGraphicArgs->FrameBufferBase + x +
                              ((y - 16) * KGraphicArgs->PixelsPerScanLine)) = *(unsigned int *)((unsigned int *)KGraphicArgs->FrameBufferBase + x +
                                                                                                (y * KGraphicArgs->PixelsPerScanLine));
    
    for (int y = CUI_Y_OF_FONT - 16; y < KGraphicArgs->VerticalResolution; y++)
        for (int x = 0; x < KGraphicArgs->HorizontalResolution; x++)
            *(unsigned int *)((unsigned int *)KGraphicArgs->FrameBufferBase + x +
                              (y * KGraphicArgs->PixelsPerScanLine)) = 0x000000;
    
}

void kputc(char chr, unsigned int color)
{
    #if SERIALPORT_SOURCE_USE_GRAPHIC_FN == definetrue
    write_serial(chr);
    #endif /*SERIALPORT_SOURCE_USE_GRAPHIC_FN*/

    if (CUI_X + 8 > KGraphicArgs->HorizontalResolution)
        CUI_Y += 16, CUI_X = 0;

    char *fontPtr = psffont->glyphBuffer + (chr * psffont->psf1_Header->charsize);

    if (chr == '\n')
    {
        CUI_Y += 16;
        CUI_X = 0;
        return;
    }

    if (CUI_Y + 16 > KGraphicArgs->VerticalResolution)
    {
        new_line();
        CUI_Y -= 16;
        CUI_X = 0;
    }

    for (int y = CUI_Y; y < CUI_Y + 16; y++)
    {
        for (int x = CUI_X; x < CUI_X + 8; x++)
        {
            if ((*fontPtr & (0b10000000 >> (x - CUI_X))) > 0)
            {
                WriteHexc(x, y, color);
            }
        }
        fontPtr++;
    }
    CUI_X += 8;
}

void printcharpointer_setcolor(char *fmt, unsigned int color)
{
    while (*fmt != '\0')
    {
        kputc(*fmt++, color);
    }
}

int printk(const char *fmt, ...)
{
    va_list ap;
    int result;
    char s[1024];
    va_start(ap, fmt);
    //見たところ%fが使えなかった。
    result = vsprintf(s, fmt, ap);
    printcharpointer_setcolor(s, Default);
    return result;
}

int printLog(KERN_LOG kernlog_type, const char *fmt, ...)
{
    va_list ap;
    int result;
    char s[1024];
    char r[1024];
    va_start(ap, fmt);
    result = vsprintf(s, fmt, ap);
    
    if (kernlog_type == KERN_EMERG)
    {
        sprintf(r, "[KERN_EMERG][TIMEROG]%s", s);
        printcharpointer_setcolor(r, emerg);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_ALERT)
    {
        sprintf(r, "[KERN_ALERT][TIMEROG]%s", s);
        printcharpointer_setcolor(r, alert);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_CRIT)
    {
        sprintf(r, "[KERN_CRIT][TIMEROG]%s", s);
        printcharpointer_setcolor(r, crit);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_ERR)
    {
        sprintf(r, "[KERN_ERR][TIMEROG]%s", s);
        printcharpointer_setcolor(r, err);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_WARNING)
    {
        sprintf(r, "[KERN_WARNING][TIMEROG]%s", s);
        printcharpointer_setcolor(r, warning);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_NOTICE)
    {
        sprintf(r, "[KERN_NOTICE][TIMEROG]%s", s);
        printcharpointer_setcolor(r, notice);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_INFO)
    {
        sprintf(r, "[KERN_INFO][TIMEROG]%s", s);
        printcharpointer_setcolor(r, info);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_DEBUG)
    {
        sprintf(r, "[KERN_DEBUG][TIMEROG]%s", s);
        printcharpointer_setcolor(r, debug);
        CUI_Y += 16, CUI_X = 0;
    }
    else if (kernlog_type == KERN_DEFAULT)
    {
        sprintf(r, "[KERN_DEFAULT][TIMEROG]%s", s);
        printcharpointer_setcolor(r, Default);
        CUI_Y += 16, CUI_X = 0;
    }
    else if(kernlog_type == KERN_CONT)
    {
        sprintf(r, "[KERN_CONT][TIMEROG]%s", s);
        printcharpointer_setcolor(r, cont);
        CUI_Y += 16, CUI_X = 0;
    }
    return result;
}
