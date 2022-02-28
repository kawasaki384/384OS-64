#include <stdint.h>

/*BearMetal_KernelArg.h*/

/*Memo
それっぽくしたいのとカーネルの引数を
もしも見たくなった時用に置いて置いた。
使うかは分からん、というか多分使わん。((ifndefのかっこよさ
*/

#define BEARMETAL_ARGMENTS
#define DEBUG_ONLY_KERNEL_ARGMENT
#define PROTOTYPES_KERNEL_ARGMENT

#ifndef BEARMETAL_ARGMENTS

#ifndef DEBUG_ONLY_KERNEL_ARGMENT 
/*
ベアメタルなUEFIのGOP環境((再現しきってはいない。))
*/

#ifndef PROTOTYPES_KERNEL_ARGMENT
#define TARGET_BITS uint64_t
typedef uint64_t EFI_PHYSICAL_ADDRESS;
typedef EFI_STATUS UINTN;

typedef enum KEFI_GRAPHICS_PIXEL_FORMAT;
typedef enum KEFI_GRAPHICS_OUTPUT_BLT_OPERATION;

typedef struct KEFI_PIXEL_BITMASK;
typedef struct KEFI_GRAPHICS_OUTPUT_MODE_INFORMATION;
typedef struct KEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;
#endif /*PROTOTYPES_KERNEL_ARGMENT*/

typedef enum
{
    K_PixelRedGreenBlueReserved8BitPerColor,
    K_PixelBlueGreenRedReserved8BitPerColor,
    K_PixelBitMask,
    K_PixelBltOnly,
    K_PixelFormatMax,
} KEFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    uint32_t RedMask;
    uint32_t GreenMask;
    uint32_t BlueMask;
    uint32_t ReservedMask;
} KEFI_PIXEL_BITMASK;

typedef enum
{
    K_EfiBltVideoFill,
    K_EfiBltVideoToBltBuffer,
    K_EfiBltBufferToVideo,
    K_EfiBltVideoToVideo,
    K_EfiGraphicsOutputBltOperationMax
} KEFI_GRAPHICS_OUTPUT_BLT_OPERATION;

typedef struct
{
    uint32_t Version;
    uint32_t HorizontalResolution;
    uint32_t VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    EFI_PIXEL_BITMASK PixelInformation;
    uint32_t PixelsPerScanLine;
} KEFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct
{
    uint32_t MaxMode;
    uint32_t Mode;
    KEFI_GRAPHICS_OUTPUT_MODE_INFORMATION Info;
    UINTN SizeOfInfo;
    KEFI_PHYSICAL_ADDRESS FrameBufferBase;
    UINTN FrameBufferSize;
} KEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef struct KernelArg_Graphic_GOP {
    /*GOP:EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode*/
    /*GOP:EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode*/
    KEFI_GRAPHICS_OUTPUT_PROTOCOL_MODE Mode;
    /*GOP:EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt*/
} GraphicArg;
#endif /*DEBUG_ONLY_KERNEL_ARGMENT */

#endif /*BEARMETAL_ARGMENTS*/