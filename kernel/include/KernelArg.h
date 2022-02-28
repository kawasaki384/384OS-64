#pragma once
#include <stdint.h>

#define UINTN uint64_t

typedef struct psf_header
{
    unsigned char magic[2];
    unsigned char mode;
    unsigned char charsize;
} psf1_header;

typedef struct PSF_FONT
{
    psf1_header *psf1_Header;
    void *glyphBuffer;
} PSF1_FONT;

typedef enum
{
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax,
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct KernelArgGraphic
{
    uint32_t version;
    uint32_t HorizontalResolution;
    uint32_t VerticalResolution;
    uint32_t PixelsPerScanLine;

    uint8_t* FrameBufferBase;
    UINTN FrameBufferSize;

    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
} KAGraphic;