;±°ÄÍ                                      
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
        
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DAATTA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
      
        Z_ADD           equ     2
        FIRST_Z         equ     32

;ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ        
        
        
        Angle1          dw     (128)            ; kamera yl”zp„in
        Angle2          dw      0               ; koordinaatit alhaalla          
        
        VoxCameraZ      dw      220*256
        VoxCameraX      dw      128*256            
        VoxCameraY      dw      128

        LoopVar         dw      ?

        CurY            dw      ?



      


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°KOOODI°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  AddBorder:
;
;     Makes a rectangular border with color 255. This is just so that the
;     voxel routine won't go outside it's area. 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
AddBorder        proc

                mov es,Topo
                mov al,255
                mov di,65535-256
                mov cx,512
                rep stosb

                xor di,di
                mov cx,256
@@VBorderLoop:
                mov es:[di],al
                mov es:[di+255],al
                add di,256
                loop @@VBorderLoop
                retn
AddBorder       endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawEvox:
;
;     Draws a topographical map using "extended voxels". Just simple
;     vertical texturing added to a regular voxel routine. This one does not
;     draw regular landscapes though. Only floors (=areas with the same
;     height) and walls (when the height changes).
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawEvox   proc                   

                mov es,Virt
                mov fs,Topo

                mov ax,Angle
                add ax,160
                and ax,1023
                xchg ax,si

;--------------------------- Drawing Begins here------------------------------
                             

                mov LoopVar,320
@@DrawLandLoop:

                mov bx,si
                shl bx,1
                mov bp,[bx+ESINE+512]
                shl ebp,16
                mov bp,[bx+ESINE]

                push si

                mov ecx,dword ptr VoxCameraZ    ;Both coords interpolated in the same reg

                mov si,FIRST_Z                  ; FirstZ = si = Z coordinate
                
                mov dl,199                      ; Initial height of vline
                mov di,LoopVar                  ; Same with screen ptr
                add di,199*320-1
                
                mov gs,Texture2
@@DrawOLoop:
                
                mov bh,ch                       ;y
                rol ecx,16
                mov bl,ch                       ;x
                rol ecx,16                      ;bx -> points to topo

                mov al,fs:[bx]                  ;Load height value
                
                cmp al,255
                je @@EndLine                    ;Check for topo edge
                
                xor ah,ah
                neg ax
                add ax,VoxCameraY

                sal ax,7                        
                
                push dx
                
                cwd
                idiv si
                add ax,99                        ;ax -> screen y
                mov CurY,ax
                                                 
                cwd
                not dx
                and ax,dx
                cmp ax,199                       ;clip screen y
                jb @@Guu
                mov al,199
@@Guu:
                pop dx

                cmp al,dl
                jae @@DrawNutin

                cmp dh,fs:[bx]
                jne @@DrawWall
                mov dh,dl
                sub dh,al
                xchg dl,al

                push bx
                shr bl,1
                shr bh,1
                mov al,gs:[bx]
                pop bx

;-----------------------------------------------------------------------------
@@DrawNormalILoop:
                mov es:[di],al                  ;Normal loop, for the floors, no interpolation
                sub di,320
                dec dh
                jnz @@DrawNormalILoop
                jmp @@DrewNormalVoxel
;-----------------------------------------------------------------------------
@@DrawWall:
                push bx
                push cx
                push si
                push bp

                add bl,bh
                shl bx,8                                ;bx texture y 

                xor cx,cx
                mov cl,dl
                sub cl,al                       ;cx-> pituus
                xchg dl,al                      ;dl->  

                push dx

                mov ax,si
                mov bp,ax

                xor dx,dx
                cmp CurY,0                ;Check if clipping is needed
                jge @@NoClip
                
                mov dx,CurY
                neg dx
                imul dx
                xchg dx,ax
@@NoClip:
                push cx

                mov ax,cx
                shl ax,6                        ;Calculate displacement
                shl cx,8                        ;y*320
                add ax,cx
                sub di,ax
                pop cx

                push ax
                
                push ds
                mov ds,Texture
@@DrawWallILoop:
                mov si,dx                       ;Get texture y-coord
                shr si,8
                mov al,[si+bx]
                mov es:[di],al                 
                add dx,bp
                add di,320
                loop @@DrawWallILoop
                pop ds
                pop ax
                sub di,ax

                pop dx
                pop bp
                pop si
                pop cx
                pop bx
;-----------------------------------------------------------------------------
@@DrewNormalVoxel:

                test dl,dl            ;test for zero, if so, we've reached the top of display
                jz  @@EndLine

@@DrawNutin:
                mov dh,fs:[bx]                  ;Remember last height..For floor/wall checks
                add ecx,ebp                     ;Interpolate ray
                
                add si,Z_ADD
                jmp @@DrawOLoop
;-----------------------------------------------------------------------------
@@EndLine:
                pop si

                dec si
                and si,1023
                
                dec LoopVar
                jnz @@DrawLandLoop
;---------------------------Drawing ends  -----------------------------------
                retn
DrawEvox     endp                


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
;  MakeText:
;
;   Quickly hacked macro to englarge a piece of text for use in the last part. 
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeText        MACRO

                mov es,Topo
                mov si,THEEND
                mov di,(128-8)*256+64+8
                mov dx,8
@@MakeTextYLoop:
                mov cx,64
@@MakeTextXLoop:
                lodsb
                cmp al,7
                jne @@NoLetter
                mov ax,0ffffh
                mov word ptr es:[di],01f1fh
                mov word ptr es:[di+256],01f1fh
                mov word ptr es:[di+512],01f1fh
                mov word ptr es:[di+768],01f1fh
                mov word ptr es:[di+1024],01f1fh
@@NoLetter:
                add di,2
                loop @@MakeTextXLoop
                add di,256*2+256-64*2
                dec dx
                jnz  @@MakeTextYLoop
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CreateTopo1      MACRO
                mov es,Topo
                xor di,di
                mov ax,0f0f0h
                mov cx,32768
                rep stosw

                xor di,di
@@DrawCircleLoop:
                mov ax,di
                sub al,128
                imul al
                xchg bp,ax
                mov ax,di
                mov al,ah
                sub al,128
                imul al
                add ax,bp
                cmp ax,123*123
                ja @@DontDrawCircle
                mov es:[di],dl
@@DontDrawCircle:
                inc di
                jnz @@DrawCircleLoop


                call AddBorder

ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
CreateTopo2      MACRO
                mov es,Topo
                xor di,di
                mov ax,0f0f0h
                mov cx,32768
                rep stosw

                xor di,di
@@DrawCircleLoop2:
                mov ax,di
                sub al,128
                imul al
                xchg bp,ax
                mov ax,di
                mov al,ah
                sub al,128
                imul al
                add ax,bp
                cmp ax,123*123
                ja @@DontDrawCircle2
                mov es:[di],dl
@@DontDrawCircle2:
                inc di
                jnz @@DrawCircleLoop2
                
                call AddBorder
                MakeText


ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DoEvoxWobble:
;
;    This is soooo cheap. Basically:
;
;    for (y=64;y<128+64;y++)
;       for(x=64;x<128+64;x++)
;          Topo[x+y*256]=(int)(64+32*(sin((Time+x)*pi/128)+sin(y*pi/128)) & 240;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DoEvoxWobble    MACRO

                mov es,Topo
                mov bp,Time
                shl bp,1
                mov di,64*256+64
                mov dx,128
@@EvoxWobbleYLoop:
                mov cx,128
@@EvoxWobbleXLoop:
                mov bx,cx
                shl bx,1

                add bx,bp
                and bx,255
                shl bx,1
                mov ax,[SINETAB+bx]
                mov bx,dx
          ;      add bx,bp
                shl bx,1
                and bx,255
                shl bx,1
                add ax,[SINETAB+bx]
                add ax,512
                shr ax,3
                and al,11110000b
                mov es:[di],al
                inc di
                
                loop @@EvoxWobbleXLoop
                add di,128                        
                dec dx
                jnz @@EvoxWobbleYLoop
                

ENDM

