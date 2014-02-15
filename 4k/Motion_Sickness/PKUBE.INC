;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;  Pkube.inc:
;
;     Voxels mapped onto a plane. Done by stacking 16 planes on top of
;     eachother and cutting those planes using a 32x32x16 voxel. 
;
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DATA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

        Angle           dw      ?

        PLANEY          equ     63

        CameraX         dw      ?
        CameraZ         dw      ?
        CameraY         dw      15*256

        LeftDeltaX      dw      ?
        LeftDeltaY      dw      ?
        
        RightDeltaX     dw      ?
        RightDeltaY     dw      ?

      ;  DeltaX          dw      ?
      ;  DeltaY          dw      ?

        FixedX          dw      ?
        FixedY          dw      ?

        KubeH           dw      ?

        YLoop           dw      ?
        XLoop           dw      ?

        Sky             dw      256*128+50
      


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
;  Parabolica.
;    
;     x^2
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Parabolica      MACRO

                push ds
                pop es
                mov di,CURVE
                mov cx,256
@@ParabolicaLoop:
                mov ax,cx
                shr ax,3
                sub al,16
                imul al
                sub al,ah                       ;Wonderklipzing
                shr al,4
                add al,16*15
                stosb                           ;Store
                loop @@ParabolicaLoop
ENDM



;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
;  DrawPKube:
;
;    This is the objectdrawer. It draws the objectplane at the specified
;    angle and coordinates.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawPKube       MACRO

                mov es,Virt
                mov fs,Kube

                mov bx,Angle
                sub bx,32
                and bx,255
                shl bx,1

                mov ax,[SINETAB+bx]
                mov LeftDeltaY,ax
                mov ax,[SINETAB+bx+128]
                mov LeftDeltaX,ax

                mov bx,Angle
                add bx,32
                and bx,255
                shl bx,1

                mov ax,[SINETAB+bx]
                mov RightDeltaY,ax
                mov ax,[SINETAB+bx+128]
                mov RightDeltaX,ax

                mov ax,Sky
                mov word ptr ds:[offset Shite+3],ax

                mov KubeH,0
@@DaKubeLoop:

                cmp KubeH,16
                jb @@Blah
                xor di,di
                jmp @@NoBlah
@@Blah:
                mov di,320*99
@@NoBlah:
                
                mov YLoop,100
@@DrawPlaneKubeYLoop:

                cmp KubeH,16
                jb @@Blah2
                mov bx,YLoop
                neg bx
                shl bx,1
                mov dx,-PLANEY*256
                jmp @@NoBlah2
@@Blah2:
                mov bx,201
                mov ax,YLoop
                shl ax,1
                sub bx,ax
                mov dx,PLANEY*256
@@NoBlah2:

                mov ax,KubeH
                neg ax
                shl ax,9
                add ax,dx                   ;PlaneY
                sub ax,CameraY              ;PlaneY-CameraY-KubeH
                cwd
                idiv bx                     ;ax -> Distance

                xchg ax,bp

                mov ax,LeftDeltaX
                imul bp
                add ax,CameraX
                mov FixedX,ax
                mov ax,RightDeltaX
                sub ax,LeftDeltaX
                mov bx,320
               
                imul bp
                idiv bx
                mov DeltaX,ax

                mov ax,LeftDeltaY
                imul bp
                add ax,CameraZ
                mov FixedY,ax
                mov ax,RightDeltaY
                sub ax,LeftDeltaY
                imul bp
                idiv bx
                mov DeltaY,ax

                mov ax,DeltaY
                mov word ptr ds:[delta1+2],ax           ;SelfModifyingCode!
                mov word ptr ds:[delta2+2],ax           
                mov word ptr ds:[delta3+2],ax           

                mov ax,FixedX
                mov dx,FixedY
                mov bp,DeltaX

                mov si,KubeH
                shl si,11
                
                cmp KubeH,16
                jb @@DrawKube

                mov gs,Texture

                mov cx,320
@@DrawNormalPlaneXLoop:   
                mov bl,dh
                mov bh,ah
Shite:          mov bl,gs:[bx+1234h]                   ;Smc.
                add bl,gs:[bx]
                xor bh,bh
                mov bl,[CURVE+bx]
                mov es:[di],bl
delta1:         add dx,0ffffh
                add ax,bp
                inc di
                loop @@DrawNormalPlaneXLoop

                jmp @@DrewNormal


                mov cx,320
@@DrawNormalPlaneXLoop2:   
                mov bl,dh
                mov bh,ah
                mov bl,gs:[bx]           
                mov es:[di],bl
delta2:         add dx,0ffffh
                add ax,bp
                inc di
                loop @@DrawNormalPlaneXLoop2

                jmp @@DrewNormal

@@DrawKube:
                mov cx,320
@@DrawPlaneKubeXLoop:
                xor bx,bx
                mov bl,dh
                and bl,31
                shl bx,5
                and ah,31
                add bl,ah
                mov bl,fs:[bx+si]
                test bl,bl
                jz @@NoPix
                mov es:[di],bl
@@NoPix:

delta3:         add dx,01234h
               
                add ax,bp
                inc di
                loop @@DrawPlaneKubeXLoop

@@DrewNormal:

                dec YLoop
                jnz @@DrawPlaneKubeYLoop

                inc KubeH
                cmp KubeH,17
                jb @@DaKubeLoop
ENDM

