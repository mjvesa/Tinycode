;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;  Tunnel.inc:
;
;    Routines for tunnelshit (parts 3 and 4).
; 
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DATA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

                TunnelX         dw      ?
                TunnelZ         dw      ?

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawTunnelSurface:
;
;   This routines draws those objects mapped onto the surface of this tunnel
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawTunnelSurface       MACRO

                mov es,TunnelSurface

                xor di,di
                xor ax,ax
                mov cx,16*256
                rep stosw

                mov fs,Kube


                xor bp,bp
@@DrawSurfaceHLoop:
                xor di,di
                mov bx,1
@@DrawSurfaceYLoop:
                mov ax,bp
                shl ax,8
                neg ax
                add ax,255*64

                xor dx,dx
                div bx
                mov si,bp
                mov dx,ax
                shr dx,7

                shl si,10                       ;Ycoord..
                add ax,TunnelZ                  ;ZCoord
                shr ax,3
                and ax,15
                shl ax,6
                add si,ax

                mov cx,32
@@HLineLoop:
                xor ax,ax
                mov al,fs:[si+32768]
                test al,al
                jz @@DontPlot
                shr al,4
                sub ax,dx
                jnc @@KRaa
                xor ax,ax
@@KRaa:

                mov es:[di],al
@@DontPlot:
                inc si
                inc di
                loop @@HLineLoop
        
                inc bl
                cmp bl,140
                jb @@DrawSurfaceYLoop
                inc bp
                cmp bp,32
                jl @@DrawSurfaceHLoop
ENDM


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawTunnel:
;
;    Draws the damn tunnel.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawTunnel      MACRO

                DrawTunnelSurface

                mov es,Virt
                mov fs,OffsetTable
                mov gs,TunnelSurface
                xor di,di
                xor si,si
                mov cx,32000
;OPTAAA SAATANA !
@@DrawTunnelLoop:
                mov bx,fs:[si]
                xor ax,ax
                mov al,bh
                shl ax,5
                and bx,31
                add bx,ax
                mov al,gs:[bx]
                mov ah,al
                stosw
                add si,2
                loop @@DrawTunnelLoop
ENDM

