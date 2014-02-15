;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
; MakeAngTable:
;
;  Calculates an angle table which is used for polar coordinate mapping.
;
;컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
MakeAngTable      MACRO


                mov es,AngTab

                xor di,di
                xor ax,ax
                mov cx,32768
                rep stosw

                xor si,si
                xor dx,dx
@@CalcTableAngleLoop:

                mov ax,128*256                          ;ax=x
                mov cx,ax                               ;cx=y

                xor dl,dl     
@@CalcTableRadiusLoop:
                mov bh,ch
                mov bl,ah
                mov es:[bx],dh
                add cx,[si+SINETAB]                     ;sin = YDelta
                cmp cx,256
                jb @@EndLoop
                cmp cx,256*255
                ja @@EndLoop
                add ax,[si+SINETAB+128]                 ;cos = XDelta
                cmp ax,256
                jb @@EndLoop
                cmp ax,256*255
                ja @@EndLoop
                inc dl
                jnz @@CalcTableRadiusLoop
@@EndLoop:
                add si,2
                inc dh
                jnz @@CalcTableAngleLoop


                mov si,1
                mov bp,128
@@FillOuterLoop:
                mov di,128
                mov al,es:[di]
                mov dx,256
@@FillLoopY:
                mov cx,128
@@FillLoopX:
                cmp byte ptr es:[di],0
                je @@NoChange
                mov al,es:[di]
@@NoChange:
                mov es:[di],al
                add di,si
                loop @@FillLoopX
                add di,bp
                dec dx
                jnz @@FillLoopY

                dec si
                jnz @@EndFuck
                mov si,-1
                mov bp,256+128
                jmp @@FillOuterLoop
@@EndFuck:

ENDM
