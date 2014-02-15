;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±        
;
;  Time.inc:
;
;    Contains all shite neccesary for timing.
;
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±        
        
        TIMERVAL        equ     17045
        
        OldHandlerT     dd      ?
        Time            dw      0


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  TimerHandler:
;
;     Just a simple handler. Notifies PIC and increments a counter. 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
TimerHandler    proc
                pusha
                push ds
                push cs
                pop ds

                inc Time
                mov al,20h
                out 20h,al
                pop ds
                popa
                iret
TimerHandler    endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  HookTimer:
;
;     Sets timer frequency to abt. 70Hz and reprograms the timer interrupt.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
HookTimer       MACRO
                mov ah,35h
                mov al,08h
                int 21h
                mov word ptr [OldHandlerT],es
                mov word ptr [OldHandlerT+2],bx
                
                mov ah,25h
                mov al,08h
                mov dx,offset TimerHandler
                int 21h
                
                cli
                mov al,036h
                out 43h,al
                pusha
                popa
                mov cx,TimerVal
                mov al,cl
                out 40h,al
                pusha
                popa
                mov al,ch
                out 40h,al
                pusha
                popa

                sti
                
ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  SetClock:
;
;     This loads the correct time from BIOS and sets the system clock. 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SetClock        MACRO

                mov ah,02h
                int 1ah

                mov bl,10
                
                mov al,ch
                shr al,4
                mul bl
                and ch,15
                add al,ch
                mov ch,al
                
                mov al,cl
                shr al,4
                mul bl
                and cl,15
                add al,cl
                mov cl,al
                
                mov al,dh
                shr al,4
                mul bl
                and dh,15
                add al,dh
                mov dh,al

                xor dl,dl

                mov ah,2dh
                int 21h

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  ReturnTimer:
;
;     Return old timerhandler and frequency. Then set clock back to
;     the right time.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ReturnTimer     MACRO
                
                mov ah,25h
                mov al,08h
                push ds
                mov dx,word ptr [OldHandlerT+2]
                mov ds,word ptr [OldHandlerT]

                int 21h
                pop ds
                
                cli
                
                mov al,00110110b
                out 43h,al
                xor ax,ax
                out 40h,al
                mov ah,al
                out 40h,al

                sti
                
                SetClock

ENDM

