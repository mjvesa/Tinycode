; Brain
; y0bi / wAMMA
; first shown: beginning of 2002
; released: 2024-2-18
cpu 386

org 100h

  COSTAB  equ     4096

start:
  mov al,13h
  int 10h

  mov dx,3d4h   ; Set 256 pixel width
  mov ax,2013h
  out dx,ax

;-----------------------------------------------------------------------
  mov di,COSTAB
  mov cx,128
CosGenLoop:
  mov ax,192                              
  sub ax,cx
  imul cx
  imul cx
  mov bx,2048
  div bx
  dec ah
  sar ax,1
  stosb
  mov [di+128-1],al 

  loop CosGenLoop

;-----------------------------------------------------------------------
  mov ax,ds                               ;allocate mem !
  add ax,4096
  mov ds,ax
  mov es,ax
  add ax,4096
  mov gs,ax
  add ax,4096
  mov fs,ax
;-----------------------------------------------------------------------
;full loop  -> di can be whatever, causes a weird pixel tho...
;fill value doesn't matter
  dec cx           
  rep stosb       

  mov cl,32
DrawBobzOuterLoop:
          
  xchg bx,dx        ;bp -> seed
  mul dx
  inc ax
  mov bl,al
  xchg ax,dx

  xor di,di
DrawPatternLoop:
  mov ax,di
  add al,bl
  
  imul al
  xchg si,ax
  mov ax,di
  xchg ah,al
  add al,bh
  imul al
  add ax,si

  cmp ah,31
  jbe ok
  mov ah,31
ok:
  add [di],ah
  dec di
  jnz DrawPatternLoop

  loop DrawBobzOuterLoop
;-----------------------------------------------------------------------
MainLoop:

  push gs
  pop es

  dec bh
  inc bp

  xor si,si
Loop:
  mov al,ds:[si+bp]
  add al,[si+bx]
  imul al
  mov fs:[di],ah
  shr ax,11
  add al,64
  stosb
  dec si
  jnz Loop

  push bx
  push bp

;-----------------------------------------------------------------------------
;dx on vapaa
;

  xor bx,bx
DrawSlimuYLoop:
  xor bp,bp

  mov si,255
DrawSlimuLineLoop:

 
  mov ah,cs:[COSTAB+si]
  mov al,fs:[bx+si]
  shr al,1
  add al,20
  imul ah
  sar ax,8
  xchg di,ax

  mov al,fs:[bx+si]                             ;Bump
  sar al,4
  add al,64+14 

  cmp si,128
  ja Draw
  cmp byte es:[bx+di],32
 jb Nope
Draw:
  mov ah,al
  mov es:[bx+di+32],ax
Nope:

  dec si
  jnz DrawSlimuLineLoop
  inc bh
  jnz DrawSlimuYLoop
;-----------------------------------------------------------------------
  push 0a000h
  pop es

  push ds
  push gs
  pop ds

  xor di,di
  mov ch,110
  rep movsw       ;si==0
  pop ds

  pop bp
  pop bx

  in al,60h
  cmp al,1
  jne MainLoop

  mov ax,3h                    
  int 10h

  ret
