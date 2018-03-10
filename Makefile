cc=gcc
out=main
include=include/
obj=image.o bmp.o main.o sepia.o
flags=-Ofast -march=native -Wall -Werror -std=gnu11 -I $(include) -c

all: clean $(out)

$(out): $(obj)
	$(cc) $(obj) -o $(out)

image.o: src/image.c
	$(cc) $(flags) src/image.c

bmp.o: src/bmp.c
	$(cc) $(flags) src/bmp.c

main.o: src/main.c
	$(cc) $(flags) src/main.c

sepia.o: src/sepia.asm
	nasm -f elf64 -o sepia.o src/sepia.asm

clean:
	rm -f  $(out) $(obj)
