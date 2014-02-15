;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;  Sprite3d.inc:
;
;    This file contais a macro for generating two 32^3 voxels.
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Calc3dSprites:
;
;    Calculates those kewlz spritez.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Calc3dSprites   MACRO

                mov es,Kube
                mov fs,AngTab

                xor di,di
                mov dl,-16
@@CalcKubeYLoop:
                mov dh,-16
@@CalcKubeZLoop:
                mov cl,-16
@@CalcKubeXLoop:
;Calculate both kubes here.
;--------------------------------------------------------------------------------
                mov al,cl
                imul al
                xchg bp,ax
                mov al,dh
                imul al
                add bp,ax
                mov al,dl
                imul al
                add ax,bp
                  
                xor bx,bx
                cmp ax,160
                jg @@DontChange

                mov bl,dl
                add bl,16
                shr bl,1
                shl bl,4
@@DontChange:
                cmp dl,-16
                jne @@Plaahhh
                

                mov al,cl
                imul al
                xchg bp,ax
                mov al,dh
                imul al
                add ax,bp
                shr ax,6
                sub al,ah
                add al,4
                xchg ax,bx

@@Plaahhh:
                mov es:[di],bl                  ;store Value !
;--------------------------------------------------------------------------------
                mov al,cl
                imul al
                xchg bp,ax
                mov al,dh
                imul al
                add bp,ax

                mov al,dl
                neg al
                add al,16
                imul al
                shr ax,2

                xor bx,bx
                cmp ax,bp
                jl @@Holex
                mov bl,dl
                neg bl
                add bl,16
                shr bl,1
                shl bl,4

@@Holex:
                cmp dl,-15
                jg @@Plaah

                mov al,cl
                add al,16
                mov ah,dh
                add ah,16
                xor al,ah
                shr al,1
                shl al,4
                xchg ax,bx
@@Plaah:
                mov es:[di+32768],bl
;--------------------------------------------------------------------------------
                inc di
                inc cl
                cmp cl,16
                jl @@CalcKubeXLoop
                inc dh
                cmp dh,16
                jl @@CalcKubeZLoop
                inc dl
                cmp dl,16
                jl @@CalcKubeYLoop


                
ENDM

