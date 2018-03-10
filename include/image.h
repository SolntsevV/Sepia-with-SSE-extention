#pragma once
#include <stdint.h>

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef int32_t LONG;
typedef uint16_t WORD;

#pragma pack(push, 1)
typedef struct pixel {
  BYTE blue;
  BYTE green;
  BYTE red;
} pixel;
#pragma pack(pop)

typedef struct image {
  DWORD length;
  DWORD height;
  DWORD width;
  pixel *pixels;
} image;


void sepia_c_inplace(image*);
void sepia_asm(const pixel*, DWORD, pixel*);

image *img_alloc(DWORD, DWORD);
image *make_copy(const image *src);
void img_free(image *);
