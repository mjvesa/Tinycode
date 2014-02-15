;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;  Tunnel.inc:
;
;    Routines for tunnelshit.
; 
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DATA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
                                
                PillarAddress   dw      0
      

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  CalcPillarMap:
;
;    This calculates a pillarmap that contains walls. Whee.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcPillarMap   MACRO
                mov es,PillarMap
                mov fs,AngTab
                mov bp,PillarAddress
                xor di,di
@@CalcPillarMapLoop:
                xor ax,ax
                mov bx,bp

                shr bl,6
                shr bh,6
                add bl,bh
                and bl,1
                jz @@Crap
comment %
                shr bl,4
                and bl,7
                shr bh,4
                and bh,7
                shl bh,3
                add bl,bh
                and bx,63
                cmp byte ptr [Pillars+bx],0
               
                je @@Crap
                 %

                xor bx,bx
                mov ax,bp
                mov bx,ax
                and ax,63
                shr bx,8
                and bl,63
                shl bx,6
                add bx,ax
                xor al,al
                cmp byte ptr [bx+RADIUSTABLE],15
                ja @@Crap
                mov bx,bp
                and bx,3f3fh
                mov al,fs:[bx+96*256+96]


@@Crap:
                mov es:[di],al
                inc bp
                inc di
                jnz @@CalcPillarMapLoop
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  CalcProjectionTable:
;
;    This precalcs a table to be used for the y-coordinate in our
;    pillareffect.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcProjectionTable     MACRO

                push ds
                pop es
                mov di,PTAB
                mov bx,1
@@CalcPTabLoop:
                mov ax,256*8
                xor dx,dx
                div bx
                stosb
                inc bl
                jnz @@CalcPTabLoop
               
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawTunnelSurface:
;
;   This routines draws those objects mapped onto the surface of this tunnel
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CalcPillarImage         MACRO

                mov es,PillarImage
                mov fs,PillarMap
                mov gs,Texture

                

                xor si,si
                xor cx,cx
@@CalcImageAngleLoop:

                mov bp,PillarAddress

                mov di,cx
                shr di,8
                mov ax,128*256                          ;ax=x
                mov dx,ax                               ;dx=y
                mov cl,128
@@CalcImageRadiusLoop:
                mov bh,dh
                mov bl,ah
                cmp byte ptr fs:[bx],0
                jne @@EndRadiusLoop
                add bx,bp
                mov bl,gs:[bx]
                shr bl,2
                mov bh,bl
                shl bh,4
                add bl,bh
                mov byte ptr es:[di],bl
                add dx,[si+SINETAB]                     ;sin = YDelta
                add ax,[si+SINETAB+128]                 ;cos = XDelta
                add di,256
                dec cl
                jnz @@CalcImageRadiusLoop
                jmp @@SkipPillarDraw
@@EndRadiusLoop:

                
;add texturecalculation here ! cl = Lenght of line.
                mov bl,fs:[bx]
                xor bp,bp

@@DrawPillarLoop:
                mov bh,ds:[PTAB+bp]
                mov al,gs:[bx]
                shr al,2
                shl al,4
                mov byte ptr es:[di],al

                add di,256
                inc bp
                dec cl
                jnz @@DrawPillarLoop

@@SkipPillarDraw:

                add si,2
                inc ch
                jnz @@CalcImageAngleLoop

ENDM


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawTunnel:
;
;    Draws the damn tunnel.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawPillars      MACRO

                CalcPillarMap
                CalcPillarImage

                mov es,Virt
                mov fs,OffsetTable
                mov gs,PillarImage


                xor di,di
                xor si,si
                mov cx,32768
@@DrawPillarsLoop:
                mov bx,fs:[si]
                mov al,gs:[bx]
                mov ah,al
                stosw

                add si,2                        ;lodsw ? 
                loop @@DrawPillarsLoop

ENDM

