;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;  RadAng.inc:
;
;    This file contais routines to create an angletable and all related
;    bullshit. Now it also calculates a tiny radius table !
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Sqrt:
;
;    Calculates the squareroot of a value given in ax. Not the fastest way.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Sqrt    proc

                push bx
                push cx
                push dx
                xchg ax,bx
                mov cx,-1
@@FindSqrt:
                inc cx
                mov ax,cx
                imul ax
                cmp ax,bx
                jb @@FindSqrt
                xchg ax,cx
                pop dx
                pop cx
                pop bx
                retn
Sqrt    endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; CalcRadiusTable:
;
;   This calculates a tiny radiustable...
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcRadiusTable         MACRO

                push ds
                pop es

                mov di,RADIUSTABLE
                mov bx,-32
@@CalcRadiusTableYLoop:
                mov cx,-32
@@CalcRadiusTableXLoop:
                mov ax,cx
                imul ax
                xchg ax,bp
                mov ax,bx
                imul ax
                add ax,bp
                call Sqrt
                stosb
                inc cx
                cmp cx,32
                jl @@CalcRadiusTableXLoop
                inc bx
                cmp bx,32
                jl @@CalcRadiusTableYLoop
ENDM


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; CalcAngTable:
;
;   This calculates a cool table with anglezz
;
;   Sinetable is uzed.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcAngTable      MACRO


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

                xor dl,dl                               ;Theoretically not needed
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
SmcAdd1:        add di,si
                loop @@FillLoopX
SmcAdd2:        add di,bp
                dec dx
                jnz @@FillLoopY

                dec si
                jnz @@EndFuck
                mov si,-1
                mov bp,256+128
                jmp @@FillOuterLoop
@@EndFuck:

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  CreateOffsetTable:
;
;    This creates an 160x200 offsettable for some kewlish routines..
;
;   Begin x = 256-160 =96 / 2 = 48 Polaariset 
;   Begin y = 256-200 =56/2 = 23
;   Begin offset = 256*43+23
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CreateOffsetTable    MACRO

                mov es,OffsetTable
                mov fs,AngTab
                xor di,di
                mov si,256*23+48
                mov bx,-100
@@CreateOffsTabYLoop:
                mov cx,-80
@@CreateOffsTabXLoop:

                mov ax,cx
                imul ax
                xchg ax,bp
                mov ax,bx
                imul ax
                add ax,bp
                call Sqrt
                mov ah,al
                mov al,fs:[si]                  ;Get Angle
                stosw
                inc si
                inc cx
                cmp cx,80
                jl @@CreateOffsTabXLoop
                add si,96
                inc bx
                cmp bx,100
                jl @@CreateOffsTabYLoop

ENDM

