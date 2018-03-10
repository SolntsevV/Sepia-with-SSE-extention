#pragma once
#include <image.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
  WORD type;
  DWORD size;
  WORD reserved1;
  WORD reserved2;
  DWORD off_bits;
} bmp_t;

typedef struct {
  DWORD size;
  LONG width;
  LONG height;
  WORD planes;
  WORD bit_count;
  DWORD compression;
  DWORD size_image;
  LONG x_pels_per_meter;
  LONG y_pels_per_meter;
  DWORD clr_used;
  DWORD clr_important;
} header_t;
#pragma pack(pop)

image *from_bmp(FILE *);
void to_bmp(FILE *, image);
