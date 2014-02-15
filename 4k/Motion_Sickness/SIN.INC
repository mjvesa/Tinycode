;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
; MakeSineTab
;
;  Creates a sinetab using iteration. This one needs 128 bytes of empty space
;  _before_ the sinetable since the strange optimizations put data down there..
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeSinetab     MACRO

                mov si,SINETAB+128+512-2            ;End of sinetable.
                mov di,SINETAB+128

                push ds
                pop es

                mov cx,128
@@SinGenLoop:

                mov ax,192
                sub ax,cx
                imul cx
                imul cx
                shr ax,11
                dec ah                          ;sub ax,256
                sal dx,5
                add ax,dx

                mov [si],ax
                mov [si-512],ax
                stosw
                dec si
                dec si

                loop @@SinGenLoop
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakeExtendedSineTab:
;
;     Makes a larger sine table from a 256-angle one by linear interpolation.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeExtendedSineTab     MACRO
                push ds
                pop es
                mov di,ESINE
                mov si,SINETAB
                mov cx,256+64
@@MakeESineLoop:
                lodsw
                stosw
                mov bp,[si]
                cmp cx,1
                ja @@SNUGG
                mov bp,255
@@SNUGG:
                sub bp,ax
                xchg bp,ax
                mov bx,5
                cwd
                idiv bx
                xchg ax,bp
                mov dx,3
@@JeeLoop:
                add ax,bp
                stosw
                dec dx
                jnz @@JeeLoop
                loop @@MakeESineLoop
ENDM

