#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#include <bmp.h>

int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, " ./main 1.bmp outC.bmp outASM.bmp count\n");
    return 1;
  }

  const char *const infile = argv[1];
  const char *const outfile_C = argv[2];
  const char *const outfile_ASM = argv[3];
  const int n = atoi(argv[4]);

  if (n <= 0) {
    puts("ERROR: count <= 0");
    return 0;
  }

  FILE *const inptr = fopen(infile, "r");
  if (inptr == NULL) {
    fprintf(stderr, "Could not open %s.\n", infile);
    return 2;
  }

  FILE *const outptr_C = fopen(outfile_C, "w");
  if (outptr_C == NULL) {
    fclose(inptr);
    fprintf(stderr, "Could not create %s.\n", outfile_C);
    return 3;
  }

  FILE *const outptr_ASM = fopen(outfile_ASM, "w");
  if (outptr_ASM == NULL) {
    fclose(inptr);
    fclose(outptr_C);
    fprintf(stderr, "Could not create %s.\n", outfile_ASM);
    return 4;
  }

  image *const img = from_bmp(inptr);
  if (!img) {
    fprintf(stderr, "Could not create %s\n", infile);
    fclose(inptr);
    fclose(outptr_C);
    fclose(outptr_ASM);
    return 5;
  }

  struct rusage r;
  struct timeval start;
  struct timeval end;
  
  image *const result_c = make_copy(img);
  image *const result_asm = make_copy(img);

  unsigned long time_of_c = 0, time_of_asm = 0;
  for (int i = 0; i < n; i++) {
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;
    sepia_c_inplace(result_c);
    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;

    time_of_c += ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;

    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;
    if (!i)
		sepia_asm(img->pixels, img->length, result_asm->pixels);
    else 
		sepia_asm(result_asm->pixels, img->length, result_asm->pixels);
    getrusage(RUSAGE_SELF, &r);
    end = r.ru_utime;

    time_of_asm += ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
  }

  printf("Time C:   %lu\n", time_of_c);
  printf("Time ASM: %lu\n", time_of_asm);

  to_bmp(outptr_C, *result_c);
  to_bmp(outptr_ASM, *result_asm);
  img_free(result_c);
  img_free(result_asm);
  img_free(img);

  fclose(inptr);
  fclose(outptr_C);
  fclose(outptr_ASM);
  return 0;
}
