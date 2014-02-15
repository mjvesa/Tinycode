;±°ÄÍ                                      
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DAATTA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
      
        ZCHANGE         equ     2

;ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ        
        
        ObjectX         dw      80             ;Coords on display
        ObjectY         dw      100
        ObjectZ         dw      100


        HorizLoop       dw      ?

        CurWall         dw      ?

        CurZ            dw      ?
        ZLoop           dw      ?

        XCoord          dw      ?
        XLenght         dw      ?
        YCoord          dw      ?
        YLenght         dw      ?


        KubeX           dw      ?
        DeltaX          dw      ?
        DeltaY          dw      ?

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°KOOODI°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakeTinyAngTab:
;
;    This calculates a tiny angletab from the bigger wun.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeTinyAngTable        MACRO

                push ds
                pop es
                push ds
                mov ds,AngTab

                mov di,ANGLETABLE
                mov si,96*256+96
                mov dx,64
@@MakeTinyAngTabYLoop:
                mov cx,32
                rep movsw
                add si,192
                dec dx
                jnz @@MakeTinyAngTabYLoop
                pop ds
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  RenderScene:
;
;     Draws a lathered object in a very stupid way.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
RenderScene   proc                   

                mov es,Virt
                mov fs,Kube2                     ;kube
                  
                mov ax,ObjectZ
                add ax,16*ZCHANGE
                mov CurZ,ax
                mov ZLoop,32                    ;Sqrt(32^2*2)
@@DrawKubeZLoop:

;Tableoptimization.

                mov bx,CurZ

                mov ax,-5120
                cwd
                idiv bx
                add ax,ObjectX
                mov XCoord,ax

                mov ax,5120
                cwd
                idiv bx
                add ax,ObjectX
                sub ax,XCoord
                mov XLenght,ax

                mov ax,-9200
                cwd
                idiv bx
                add ax,ObjectY
                mov YCoord,ax

                mov ax,9200
                cwd
                idiv bx
                add ax,ObjectY
                sub ax,YCoord
                mov YLenght,ax

                mov ax,128*256
                xor dx,dx
                div YLenght
                mov DeltaY,ax

                mov KubeX,0
               
                mov ax,32*256
                xor dx,dx
                div XLenght
                mov DeltaX,ax

@@DrawKubeXLoop:

                mov ax,YCoord
                mov bx,ax
                shl ax,8
                shl bx,6
                add ax,bx
                add ax,XCoord
                mov di,ax

                mov bx,ZLoop
                shl bx,1
                shl bx,6
                mov ax,KubeX
                shr ax,8
                shl ax,1
                add bx,ax
                xor ax,ax
                mov al,[bx+RADIUSTABLE]
                cmp al,31
                ja @@NoLine
                mov si,ax
                shl si,7

                mov ah,[bx+ANGLETABLE]
                shr ah,2
      ;          add ah,16

;                mov ah,byte ptr [ZLoop]
                xor dx,dx
                mov bp,DeltaY
                xor bx,bx
                mov cx,YLenght
@@DrawKubeYLoop:
                mov bl,dh
                mov al,fs:[si+bx]
                test al,al
                jz @@DontDraw
                xor bx,bx
                mov bl,cl
                mov al,[METACOLORS+bx]

                mov es:[di],al
@@DontDraw:
                add dx,bp
                add di,320
                loop @@DrawKubeYLoop
@@NoLine:

                mov ax,DeltaX
                add KubeX,ax

                inc XCoord
                dec XLenght
                jnz @@DrawKubeXLoop
                sub CurZ,ZCHANGE
                dec ZLoop
                jnz @@DrawKubeZLoop
                
                retn
RenderScene     endp                
