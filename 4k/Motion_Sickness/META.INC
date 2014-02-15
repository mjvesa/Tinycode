;±°ÄÍ                                      
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DATA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

                MetaY1          dw      8
                MetaY2          dw      64-8

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  CalcMeta1:
;
;    This calculates meta part 1: Two simple metaballs colliding into
;    eachother.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcMeta        MACRO

                mov es,Kube2

                xor cx,cx
@@CalcMetaYLoop:
                mov di,cx
                xor bx,bx
@@CalcMetaXLoop:
                mov ax,cx
                sub ax,MetaY1
                imul ax
                xchg bp,ax
                mov ax,bx
                imul ax
                add bp,ax
                mov ax,32767
                xor dx,dx
                inc bp
                div bp
                mov si,ax

                mov ax,cx
                sub ax,MetaY2
                imul ax
                xchg bp,ax
                mov ax,bx
                imul ax
                add bp,ax
                mov ax,32767
                xor dx,dx
                inc bp
                div bp
                add ax,si
                xchg ax,bp

                xor ax,ax
                cmp bp,128
                jb @@EmptyMeta
                mov al,3
@@EmptyMeta:

                stosb
                add di,128-1
                inc bx
                cmp bx,32
                jb @@CalcMetaXLoop
                inc cx
                cmp cx,128
                jb @@CalcMetaYLoop


                                
ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakeMetaColors
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeMetaColors  MACRO
                push ds
                pop es
                mov di,METACOLORS
                xor cx,cx

@@MakeMetaColorsLoop:
                mov bx,cx
                shl bx,2
                and bx,255
                shl bx,1
                mov ax,[SINETAB+bx]
                add ax,256
                shr ax,6

                add bx,20
                and bx,511
                mov dx,[SINETAB+bx]
                add dx,256
                shr dx,6
                shl dx,5
                add ax,dx

                stosb
                inc cl
                jnz @@MakeMetaColorsLoop


ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakePart2BG:
;
;    Makes a background image for part2.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakePart2BG     MACRO

                mov es,Texture2
                mov fs,Texture
                xor di,di
                mov dx,200
@@MakePart2BGYLoop:
                mov cx,320
@@MakePart2BGXLoop:
                mov bl,cl
                mov bh,dl
                mov al,fs:[bx]
                shr al,2
                mov ah,fs:[bx+10*256+10]
                shr ah,2
                shl ah,4
                add al,ah
                mov es:[di],al
                inc di
                dec cx
                jnz @@MakePart2BGXLoop
                dec dx
                jnz @@MakePart2BGYLoop

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawBG:
;
;    Draws the background for part2 (and 4?).
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawBG          proc

                mov es,Virt
                push ds
                mov ds,Texture2
                xor edi,edi
                xor esi,esi
                mov ecx,16000
                rep movsd
                pop ds
                retn
DrawBG          endp

