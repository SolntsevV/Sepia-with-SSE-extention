#include <math.h>
#include <stdlib.h>

#include <image.h>

static BYTE sat(QWORD x) {
  if (x < 256) return x;
  return 255;
}

static void sepia_one(pixel* const pxl) {
  static const float c[3][3] = {
      {.393f, .769f, .189f}, {.349f, .686f, .168f}, {.272f, .534f, .131f}};
  pixel const old = *pxl;
  pxl->red =
      sat(old.red * c[0][0] + old.green * c[0][1] + old.blue * c[0][2]);
  pxl->green =
      sat(old.red * c[1][0] + old.green * c[1][1] + old.blue * c[1][2]);
  pxl->blue =
      sat(old.red * c[2][0] + old.green * c[2][1] + old.blue * c[2][2]);
}

static pixel* pixel_of(image img, DWORD x, DWORD y) {
  return &img.pixels[y * img.width + x];
}

void sepia_c_inplace(image* img) {
  DWORD x, y;
  for (y = 0; y < img->height; y++)
    for (x = 0; x < img->width; x++) sepia_one(pixel_of(*img, x, y));
}

image* img_alloc(const DWORD height, const DWORD width) {
  image* img = malloc(sizeof(image));
  img->width = width;
  img->height = height;
  img->length = width * height;
  img->pixels = calloc(img->length, sizeof(pixel));
  return img;
}

image* make_copy(const image* src) {
  const DWORD length = src->length;
  image* copy = img_alloc(src->height, src->width);
  for (int i = 0; i < length; i++) copy->pixels[i] = src->pixels[i];
  return copy;
}

void img_free(image* img) {
  free(img->pixels);
  free(img);
}
