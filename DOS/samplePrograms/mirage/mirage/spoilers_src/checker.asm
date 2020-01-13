[bits 16]
[org 0]

; assume
; ds, es - set properly
; cs - set to this
int3
push si
push fs

; Get address of password to SI from params.
mov si, sp
mov ax, [si + 8]
mov si, ax

; Reset counter.
xor cx, cx

%macro step1 2
  ; Get character.
  mov ah, [si]
  inc si

  ; Step 1.
  ; Set the DAC register index and port number.
  mov al, %1  ; Color number
  mov dx, 0x3c7
  out dx, al
  add dx, 2

  ; Get RGB and XOR them with shift.
  in al, dx
  xor ah, al
 
  in al, dx
  shl al, 1
  xor ah, al

  in al, dx
  shl al, 2
  xor ah, al

  ; Step 2.
  mov al, %2  ; Color number
  mov dx, 0x3c7
  out dx, al
  add dx, 2
%endmacro

%macro step2odd 0
  in al, dx
  in al, dx
  shl al, 6
  mov bl, al
  in al, dx
  or bl, al
%endmacro

%macro step2even 0
  in al, dx
  shl al, 4
  mov bl, al
  in al, dx
  shr al, 2
  or bl, al
  in al, dx
%endmacro

%macro step3 0
  ; Merge.
  xor ah, bl
  movzx ax, ah
  add cx, ax
%endmacro

%macro nextodd 2
  step1 %1, %2
  step2odd
  step3
%endmacro

%macro nextevn 2
  step1 %1, %2
  step2even
  step3
%endmacro

nextodd 0, 192
nextevn 1, 192

nextodd 2, 193
nextevn 3, 193

nextodd 4, 194
nextevn 5, 194

nextodd 6, 195
nextevn 7, 195

nextodd 8, 196
nextevn 9, 196

nextodd 10, 197
nextevn 11, 197

nextodd 12, 198
nextevn 13, 198

nextodd 14, 199
nextevn 15, 199

nextodd 16, 200
nextevn 17, 200

nextodd 18, 201
nextevn 19, 201

nextodd 20, 202
nextevn 21, 202

nextodd 22, 203
nextevn 23, 203

nextodd 24, 204
nextevn 25, 204

; Done.
test cx, cx     ;  good(H/L)    bad(H/L)
setz bl         ;  BH BL        BH BL
setnz bh        ;  00 01        01 00      

dec bl          ;
dec bh          ;  FF 00        00 FF
not bx          ;  00 FF        FF 00

mov cl, 205
mov ch, 16

mov dx, 0x3c8
mov al, cl
out dx, al
inc dx

l:
  mov al, ch
  and al, bh
  out dx, al
  
  mov al, ch
  and al, bl
  out dx, al

  xor al, al
  out dx, al

  inc cl
  inc ch
  cmp cl, 237
  jne l

pop fs
pop si
retf

