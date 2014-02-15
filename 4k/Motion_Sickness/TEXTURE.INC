;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° DATA °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

                Seed            dw      39313
                RandVal         dw      ?
                a               dw      ?


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°° CODE2 °°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Bump. fs - Source es - destination
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Bump            proc
                xor di,di
@@BumpLoop:
                mov al,fs:[di+1]
                sub al,fs:[di-1]
                mov ah,fs:[di+256]
                sub ah,fs:[di-256]
                add al,ah
                add al,92
                mov es:[di],al
                inc di
                jnz @@BumpLoop
                retn
Bump            endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Smooth. es - destination
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Smooth          proc
                xor di,di
                xor ax,ax
@@SmoothLoop:
                mov al,es:[di-1]                
                add al,es:[di+1]
                adc ah,0
                add al,es:[di-256]
                adc ah,0
                add al,es:[di+256]
                adc ah,0
                shr ax,2
                mov es:[di],al
                inc di
                jnz @@SmoothLoop
                retn
Smooth          endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Random:
;
;    Generates overkewl random numbers.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Random proc
                mov ax,Seed
                push bx
                push dx
                mov bx,997
                mul bx
                inc ax
                mov Seed,ax
                shr ax,7
                pop dx
                pop bx

                mov RandVal,ax

                retn
Random endp               


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakeTexture:
;
;    Shadebob texture. Whoa. 
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeTexture    MACRO

                mov es,Texture


                xor di,di
                xor ax,ax
                mov cx,32768
                rep stosw

                mov a,100

@@DrawBobzOuterLoop:

                call Random
                mov di,RandVal
                shl di,8
                call Random
                add di,RandVal

                mov bx,-32
@@DrawBobYLoop:
                mov cx,-32
@@DrawBobXLoop:
                mov ax,cx
                imul ax
                xchg bp,ax
                mov ax,bx
                imul ax
                add ax,bp
                shr ax,7
                neg ax
                add ax,8
                cmp ax,0
                jle @@DontAdd

                add es:[di],al
@@DontAdd:
                inc di
                inc cx
                cmp cx,32
                jl @@DrawBobXLoop

                add di,192
                inc bx
                cmp bx,32
                jl @@DrawBobYLoop

                dec a
                jnz @@DrawBobzOuterLoop
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ

MakeEvoxTextures        MACRO


                mov gs,Texture
                mov fs,Texture2
                mov es,Virt
                xor si,si
@@MakeEvoxTexturesLoop:
                call Random
                mov es:[si],al
                inc si
                jnz @@MakeEvoxTexturesLoop


                call Smooth
                call Smooth
                call Smooth

                mov es,Texture2
                mov fs,Virt

                call Bump

                xor si,si
@@MakeEvoxTexturesLoop2:
                mov al,es:[si]
                shr al,4
                mov ah,al
                shl ah,4
                add al,ah
                mov es:[si],al
                inc si
                jnz @@MakeEvoxTexturesLoop2


                mov fs,Texture
                mov es,Virt
                xor si,si
@@MakeEvoxTexturesLoop3:
                mov al,gs:[si]
                shr al,2
                mov ah,gs:[si+4*256+4]
                shr ah,2
                shl ah,4
                add al,ah
                mov es:[si],al
                inc si
                jnz @@MakeEvoxTexturesLoop3

                mov es,Texture
                push ds
                mov ds,Virt
                xor di,di
                xor si,si
                mov cx,32768
                rep movsw
                pop ds

ENDM
