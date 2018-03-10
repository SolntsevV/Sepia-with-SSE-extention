%define pixel_array rdi
%define array_size rsi
%define result rdx
%define max_vals xmm7

%macro do_sepia 0
  mulps xmm0, xmm3
  mulps xmm1, xmm4
  mulps xmm2, xmm5

  addps xmm0, xmm1
  addps xmm0, xmm2

  cvtps2dq xmm0, xmm0

  pminsd xmm0, max_vals

  pextrb [result],      xmm0, 0
  pextrb [result + 1],  xmm0, 4
  pextrb [result + 2],  xmm0, 8
  pextrb [result + 3],  xmm0, 12

  add pixel_array, 3
  add result, 4

  pxor xmm0, xmm0
  pxor xmm1, xmm1
  pxor xmm2, xmm2
%endmacro

%macro mov_first 0
  pinsrb xmm0, byte[pixel_array], 0
  pinsrb xmm0, byte[pixel_array + 3], 12
  shufps xmm0, xmm0, 00000000
  cvtdq2ps xmm0, xmm0

  pinsrb xmm1, byte[pixel_array+ 1], 0
  pinsrb xmm1, byte[pixel_array + 4], 12
  shufps xmm1, xmm1, 00000000
  cvtdq2ps xmm1, xmm1

  pinsrb xmm2, byte[pixel_array + 2], 0
  pinsrb xmm2, byte[pixel_array + 5], 12
  shufps xmm2, xmm2, 00000000
  cvtdq2ps xmm2, xmm2

  movaps xmm3, [c1_1]
  movaps xmm4, [c2_1]
  movaps xmm5, [c3_1]
%endmacro

%macro mov_second 0
  pinsrb xmm0, [pixel_array], 0
  pinsrb xmm0, [pixel_array + 3], 8
  shufps xmm0, xmm0, 0b00001010
  cvtdq2ps xmm0, xmm0

  pinsrb xmm1, [pixel_array + 1], 0
  pinsrb xmm1, [pixel_array + 4], 8
  shufps xmm1, xmm1, 0b00001010
  cvtdq2ps xmm1, xmm1

  pinsrb xmm2, [pixel_array + 2], 0
  pinsrb xmm2, [pixel_array + 5], 8
  shufps xmm2, xmm2, 0b00001010
  cvtdq2ps xmm2, xmm2

  movaps xmm3, [c1_2]
  movaps xmm4, [c2_2]
  movaps xmm5, [c3_2]
%endmacro

%macro mov_third 0
  pinsrb xmm0, byte[pixel_array], 0
  pinsrb xmm0, byte[pixel_array+3], 4
  shufps xmm0, xmm0, 0b00010101
  cvtdq2ps xmm0, xmm0

  pinsrb xmm1, byte[pixel_array + 1], 0
  pinsrb xmm1, byte[pixel_array + 4], 4
  shufps xmm1, xmm1, 0b00010101
  cvtdq2ps xmm1, xmm1

  pinsrb xmm2, byte[pixel_array + 2], 0
  pinsrb xmm2, byte[pixel_array + 5], 4
  shufps xmm2, xmm2, 0b00010101
  cvtdq2ps xmm2, xmm2

  movaps xmm3, [c1_3]
  movaps xmm4, [c2_3]
  movaps xmm5, [c3_3]
%endmacro

DEFAULT REL

section .rodata
align 16
c1_1: dd 0.272, 0.349, 0.393, 0.272
align 16
c2_1: dd 0.534, 0.686, 0.769, 0.534
align 16
c3_1: dd 0.131, 0.168, 0.189, 0.131

align 16
c1_2: dd 0.349, 0.393, 0.272, 0.349
align 16
c2_2: dd 0.686, 0.769, 0.534, 0.686
align 16
c3_2: dd 0.168, 0.189, 0.131, 0.168

align 16
c1_3: dd 0.393, 0.272, 0.349, 0.393
align 16
c2_3: dd 0.769, 0.534, 0.686, 0.769
align 16
c3_3: dd 0.189, 0.131, 0.168, 0.189



align 16
max_values: dd 255, 255, 255, 255

section .text
global sepia_asm:function

; first in rdi 
; second in rsi 
; third in rdx
sepia_asm:
  test array_size, array_size
  jz .end

  movaps max_vals, [max_values]

  pxor xmm0, xmm0
  pxor xmm1, xmm1
  pxor xmm2, xmm2

.loop:
  mov_first
  do_sepia
  mov_second
  do_sepia
  mov_third
  do_sepia

  add pixel_array, 3
  sub array_size, 4
  ja .loop


.end:
  ret
