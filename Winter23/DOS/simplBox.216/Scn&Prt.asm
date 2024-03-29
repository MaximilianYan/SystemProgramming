.model tiny
.code
locals @@
org 100h

Start:
                mov bx, 0d                      ; Scan number to stack
                call ScanNDec
                push bx
                mov bx, 0d                      ; Scan number to stack
                call ScanNDec
                push bx

                pop ax                          ; Addiction
                pop bx
                add bx, ax
                push bx

                mov ax, 0b800h                  ; адрес сегмента с видеопамятью -> es
                mov es, ax                      ;

                mov ah, 00001010b               ; Light green on black
                mov bx, 160d*18 + 160d/2 - 18d*2 + 4d; Середина строчки
                mov ch, 5                       ; Height
                mov cl, 18                      ; Width
                call DrawBox


                mov dh, 00000010b               ; Dark green on black
                pop ax                          ; Scanned number
                push ax                         ;


                mov bx, 160d*19 + 160d/2        ; Середина строчки

                call PrintNDec

                
                pop ax                          ; Scanned number
                push ax                         ;

                mov bx, 160d*20 + 160d/2        ; Середина строчки

                call PrintNHex

                
                pop ax                          ; Scanned number
                push ax                         ;

                mov bx, 160d*21 + 160d/2        ; Середина строчки

                call PrintNBin

                mov ax, 4c00h                   ; exit(0)
                int 21h

include ..\LianLib\ScanNDec.asm
include ..\LianLib\PrntNBin.asm
include ..\LianLib\PrntNHex.asm
include ..\LianLib\PrntNDec.asm
include ..\LianLib\DrawBox.asm

.data
include ..\LianLib\Alphabet.asm

end             Start
