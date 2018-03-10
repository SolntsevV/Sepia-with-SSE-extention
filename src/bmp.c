#include <bmp.h>

#include <stdlib.h>

image *from_bmp(FILE *const inptr) {
  bmp_t bf;
  int read_error = fread(&bf, sizeof(bmp_t), 1, inptr);
  if (!read_error && (feof(inptr) || ferror(inptr))) //определение ошибок потока данных
	  return NULL;

  header_t bi;
  read_error = fread(&bi, sizeof(header_t), 1, inptr);

  if (!read_error && (feof(inptr) || ferror(inptr)))
	  return NULL;

  image *img = img_alloc(bi.height, bi.width);

  int padding = (4 - (bi.width * sizeof(pixel)) % 4) % 4;

  for (int i = 0; i < bi.height; i++) {
    for (int j = 0; j < bi.width; j++) {
      read_error = fread(&img->pixels[i * bi.width + j], sizeof(pixel), 1, inptr);
      if (!read_error && ferror(inptr)) 
		  return NULL;
    }
    fseek(inptr, padding, SEEK_CUR);
  }
  return img;
}

void to_bmp(FILE *const outptr, const image img) {
  header_t bi;
  bi.bit_count = 24;
  bi.compression = 0;
  bi.size = 40;
  bi.planes = 1;
  bi.height = img.height;
  bi.width = img.width;
  bi.size_image = bi.height * bi.width * sizeof(pixel);
  bi.x_pels_per_meter = 0;
  bi.y_pels_per_meter = 0;
  bi.clr_used = 0;
  bi.clr_important = 0;

  bmp_t bf;
  bf.type = 0x4d42;
  bf.off_bits = 54;
  bf.size = bi.size + bf.off_bits;
  bf.reserved1 = 0;
  bf.reserved2 = 0;

  fwrite(&bf, sizeof(bmp_t), 1, outptr);
  fwrite(&bi, sizeof(header_t), 1, outptr);

  int padding = (4 - (img.width * sizeof(pixel)) % 4) % 4;

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      fwrite(&(img.pixels[i * img.width + j]), sizeof(pixel), 1, outptr);
    }
    for (int k = 0; k < padding; k++) fputc(0x00, outptr);
  }
}
