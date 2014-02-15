;±°ÄÍ
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±
;±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±

.386
.387

CODE    segment use16 'CODE'
        assume cs:CODE,ds:CODE
        org 100h


BeginningOfTime:
                jmp start
;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°DATA°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
        
        SINETAB         equ     8192
        CURVE           equ     8192+4096
        FADETABLE       equ     16384
        RADIUSTABLE     equ     16384+4096
        ANGLETABLE      equ     16384+4096*2
        PTAB            equ     16384+4096*3
        METACOLORS      equ     32768           ;256 tavua..
        ESINE           equ     32768+256
        THEEND          equ     32768+8192




        
                
        Virt            dw      ?
label   PillarImage
        Kube            dw      ?
        AngTab          dw      ?
        Texture         dw      ?
        Texture2        dw      ?
        OffsetTable     dw      ?
label   PillarMap
label   Topo
label   Kube2
        TunnelSurface   dw      ?


        PartLenght      dw      ?

;ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ

        IntroText       db      'THE END',13,10,10,10,10
                        db      '       ---- Motion Sickness ----',10,13
                        db      10,13,10,10,10,10
                        db      '       A 4k intro by yobi/wAMMA',10,13
                        db      10,13,10,10,10,10
                        db      '       Released at assembly 2001$'

;ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°INCLUDE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
include sin.inc
include vox.inc
include time.inc
include meta.inc
include kube.inc
include pkube.inc
include tunnel.inc
include pillar.inc
include radang.inc
include texture.inc
include sprite3d.inc


;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°CODE°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  SetPalette:
;
;     Sets the only palette used during the intro. Using this special
;     palette is similar to unpalettized modes (eg. 24bit)
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SetPalette      MACRO
                mov dx,03c8h
                xor al,al
                out dx,al
                inc dx
                mov bl,0
@@PaletteLoop1:                
                mov cl,0
@@PaletteLoop2:                
                mov al,cl
                out dx,al

                mov al,cl
                add al,bl
                shr al,1
                out dx,al
                
                mov al,bl
                out dx,al

                add cl,4
                cmp cl,63
                jbe @@PaletteLoop2
                add bl,4
                cmp bl,63
                jbe @@PaletteLoop1

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  MakeFadeTable:
;
;    This routine calculates a fade table used for transformations between
;    parts...
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
MakeFadeTable   MACRO
                mov di,FADETABLE
                xor cx,cx
@@MakeFadeTabYLoop:
                xor cl,cl
@@MakeFadeTabXLoop:

                mov al,cl
                sub al,32
                imul al
                xchg bp,ax
                mov al,ch
                sub al,32
                imul al
                add ax,bp
                shr ax,4
                xchg bx,ax

;                mov bl,cl
;                add bl,ch
                call Random
                and al,15
                add al,bl
                mov [di],al                  ;Values between 0 and 126+7
                inc di
                inc cl
                cmp cl,64
                jb @@MakeFadeTabXLoop
                inc ch
                cmp ch,64
                jb @@MakeFadeTabYLoop
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  ApplyTransformation:
;
;    This adds a transformation to the virtual screen. Simple routine
;    that can be recycled and parts look less crap.
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
ApplyTransformation     proc

                mov ax,Time
                cmp ax,126+7
                jb @@TransFade
                mov ax,PartLenght
                sub ax,Time
                cmp ax,126+7
                jg @@NoFades
                cmp ax,0
                jge @@FadeValOK
                xor ax,ax
@@FadeValOk:

@@TransFade:
                mov dx,ax

                mov es,Virt
                xor di,di
                xor ax,ax
                mov bp,200
@@ApplyYLoop:
                mov cx,320
@@ApplyXLoop:
                mov bx,bp
                and bl,63
                shl bx,6
                mov al,cl
                and al,63
                add bx,ax
                cmp [bx+FADETABLE],dl
                jb @@NoBlack
                mov al,[bx+FADETABLE]
                xor ax,ax
                mov es:[di],al
@@NoBlack:
                inc di
                loop @@ApplyXLoop
                dec bp
                jnz @@ApplyYLoop

@@NoFades:


                
                retn                
ApplyTransformation     endp


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;
;  Rotate:
;
;
;     Rotates our objectplane. Actually just sets camera coords.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Rotate          MACRO


                mov bx,Time
                shr bx,2
                and bx,255
                mov Angle,bx
                shl bx,1
                
                mov ax,[SINETAB+bx]
                neg ax
                sal ax,7
                add ax,128*256
                mov CameraX,ax

                mov ax,[SINETAB+bx+128]
                neg ax
                sal ax,7
                add ax,128*256
                mov CameraZ,ax

                mov ax,[SINETAB+bx]
                sal ax,5
                sub ax,16*256
                mov CameraY,ax

ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  WaitForRT
;
;    Waits for screen refresh.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
WaitForRT       proc

                mov dx,3dah
@@Wait1:
                in al,dx
                and al,00001000b
                jnz @@Wait1
@@Wait2:
                in al,dx
                and al,00001000b
                jz @@Wait2

                retn
WaitForRT       endp


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Flip:
;
;     Yeah. That.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Flip            proc
                push 0a000h
                pop es
                push ds
                mov ds,Virt
                xor edi,edi
                xor esi,esi
                mov ecx,16000
                rep movsd
                pop ds
                retn

comment %
                mov es,Virt
                xor edi,edi
                mov ecx,16000
                xor eax,eax
                rep stosd
%

Flip            endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; Sets those pointers for your pleasure
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
SetPtrs         MACRO

                mov ax,ds
                add ax,4096

                mov Virt,ax
                add ax,4096

                mov Kube,ax
                add ax,4096

                mov Texture,ax
                add ax,4096

                mov Texture2,ax
                add ax,4096

                mov AngTab,ax
                add ax,4096

                mov OffsetTable,ax
                add ax,4096

                mov TunnelSurface,ax
                add ax,512
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
; ROTAATEEE
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Rotate2         proc

                mov ax,Time
                shr ax,1
                mov Angle1,ax
                add ax,128
                mov Angle2,ax

                and Angle2,255
                and Angle1,255
                
                mov ax,Angle1
                shl ax,2
                mov Angle,ax
                
                mov si,Angle2
                shl si,1                
                
                mov bx,100
                mov ax,[si+SINETAB]
                imul bx
                add ax,127*256
                mov VoxCameraZ,ax

                mov ax,[si+SINETAB+128]
                imul bx
                add ax,127*256
                mov VoxCameraX,ax
                
                ret

Rotate2         endp

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  DrawIntroText:
;
;    A simple routine that just draws the introductionary text..
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
DrawIntroText   MACRO

                mov dx,offset IntroText
                mov ah,09h
                int 21h
ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  GetText:
;
;     Writes text onto the screen using int21 services and captures that.
;     Cheap-o.
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
GetText         MACRO

                push 0a000h
                pop fs
                push ds
                pop es
                mov dx,8
                xor si,si
                mov di,THEEND
@@GetTextYLoop:
                mov cx,64
@@GetTextXLoop:
                mov al,fs:[si]
                mov byte ptr fs:[si],0
                stosb
                inc si
                loop @@GetTextXLoop
                add si,320-64
                dec dx
                jnz @@GetTextYLoop

ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Intro
;
;    Shows the introductionary text. Some Fades also...
;
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Intro          MACRO

                mov dx,3c8h
                mov al,7
                out dx,al
                inc dx

                xor al,al
                out dx,al
                out dx,al
                out dx,al

                DrawIntroText
                GetText

                mov Time,0
@@FadeUpLoop:
                call WaitForRT
                mov dx,03c8h
                mov al,7
                out dx,al
                inc dx
                mov ax,Time
                shr ax,2
                out dx,al
                out dx,al
                out dx,al
                cmp Time,3*70
                jb @@FadeUpLoop


                mov Time,0
@@IntroPause:
                cmp Time,3*70
                jb @@IntroPause

                mov Time,0

@@FadeDownLoop:
                call WaitForRT
                mov dx,03c8h
                mov al,7
                out dx,al
                inc dx
                mov ax,3*70
                sub ax,Time
                shr ax,2
                out dx,al
                out dx,al
                out dx,al
                cmp Time,3*70
                jb @@FadeDownLoop

                push 0a000h
                pop es
                xor ax,ax
                xor di,di
                mov cx,32000
                rep stosw                       ;Clean, bug removal !

                                       

ENDM


;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part1:
;
;    Motion sickness, part1: Voxelmapped plane.
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part1           MACRO

                mov PartLenght,20*70
                mov Time,0
@@Part1:
                Rotate

                DrawPKube

                call ApplyTransformation

                call Flip

                mov ax,Time
                shl ax,8
                add al,byte ptr [Time]
                mov Sky,ax

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part1


ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part2:
;
;    Motion sickness, part2: Metaballs.
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part2           MACRO

                mov PartLenght,10*70
                mov Time,0
@@Part2:


                mov bx,Time
                xor bh,bh
                shl bx,1

                mov ax,[SINETAB+bx]
                add ax,256
                shr ax,3
                add ax,32
                mov MetaY1,ax

                mov bx,Time
                shl bx,1
                xor bh,bh
                shl bx,1
                
                mov ax,[SINETAB+bx]
                add ax,256
                shr ax,3
                add ax,32
                mov MetaY2,ax

                call DrawBG

                CalcMeta

                call RenderScene

                call ApplyTransformation

                call Flip

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part2


ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part3:
;
;    Motion sickness, part3: Objects projected onto a tunnel
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part3           MACRO

                mov PartLenght,20*70
                mov Time,0
@@Part3:

                DrawTunnel

                call ApplyTransformation
                call Flip

                mov ax,Time
                shl ax,1
                xor  ah,ah
                mov TunnelZ,ax

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part3

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part4:
;
;    Motion sickness, part4: Pillars
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part4           MACRO

                mov PartLenght,20*70
                mov Time,0
@@Part4:

                DrawPillars
                call ApplyTransformation
                call Flip

                mov bx,Time
                xor bh,bh
                shl bx,1
                mov ax,[SINETAB+bx]
                mov bp,10
                imul bp
                mov bp,25
                cwd
                idiv bp
                shl ax,8
                xchg cx,ax
                mov ax,[SINETAB+bx+128]
                mov bp,10
                imul bp
                mov bp,25
                cwd
                idiv bp
                add ax,cx
                add ax,32*256+32
                mov PillarAddress,ax

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part4

ENDM

;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part6:
;
;    Motion sickness, part6: Extended voxel rotation, "The End."
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part5   MACRO
                mov PartLenght,20*70
                mov Time,0
@@Part5:
                mov es,Virt
                mov ax,04444h
                xor di,di
                mov cx,32000
                rep stosw

                DoEvoxWobble

                call Rotate2
                call DrawEvox
                                                       
                call ApplyTransformation
                call Flip

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part5

ENDM
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
;  Part6:
;
;    Motion sickness, part6: End text
; 
;ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
Part6   MACRO
                mov PartLenght,10*70
                mov Time,0
@@Part6:
                mov es,Virt
                mov ax,04444h
                xor di,di
                mov cx,32000
                rep stosw


                call Rotate2
                call DrawEvox

                call ApplyTransformation
                call Flip

                in al,60h
                cmp al,1
                je @@EndIntro

                mov ax,PartLenght
                cmp Time,ax
                jb @@Part6

ENDM

;°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°START°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
start:
                SetPtrs

                mov ax,13h
                int 10h

                HookTimer

                Parabolica                              ;Whut ?
                MakeSineTab                             ;AFSEW
                CalcAngTable
                MakeTexture
                Calc3dSprites
                MakeFadeTable
                CalcRadiusTable
                MakeTinyAngTable
                CreateOffsetTable
                CalcProjectionTable
                MakeMetaColors
                                                            
                Intro

                SetPalette

                Part1
               
                MakePart2BG

                Part2

                Part3

                Part4

                MakeExtendedSineTab
                MakeEvoxTextures

                CreateTopo1
                Part5

                CreateTopo2

                Part6

@@EndIntro:
                ReturnTimer

                mov ax,3h
                int 10h


                retn

ENDS    CODE
END     BeginningOfTime
