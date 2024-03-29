;===========================================
;   asmMain.asm
;   printfm & printf caller
;===========================================

;-------------------------------------------
; External stuff
;-------------------------------------------

extern printf                           ; stl

extern printfm
extern callPrintfm


;-------------------------------------------
; Main program with functions callings
;-------------------------------------------
section .text
global asmMain
asmMain:
                push rbx                ; store external rbx to stack


                mov rcx, MsgPrintf
                mov rdx, StringPrintf
                mov r8, 1775
                mov r9, 776

                push qword "g"
                push qword -778
                ; push qword 777
                sub rsp, (8*4)
                call printf             ; printf (MsgPrintf, 7, MsgText2, rsp, 775)
                add rsp, (8*6)


                push qword "g"
                push qword -778
                push qword -777
                push qword 776
                push qword 1775
                push qword StringPrintf
                push qword MsgPrintf
                call printfm
                add rsp, (8*7)


; callPrintfm("%s-%s-%s-%s-%s%d %s %x %d%%%c%b\n", "aaa", "bbb", "ccc", "ddd", "eee", -1, "love", 3802, 100, 33, 127);

                push qword PrintfParam12
                push qword PrintfParam11
                push qword PrintfParam10
                push qword PrintfParam9
                push qword PrintfParam8
                push qword PrintfParam7
                push qword PrintfParam6
                push qword PrintfParam5
                push qword PrintfParam4
                push qword PrintfParam3
                push qword PrintfParam2
                push qword PrintfParam1
                call printfm
                add rsp, (8*12)

                ; push qword -1
                ; push qword -1
                ; push qword MsgD
                ; call printfm
                ; add rsp, (8*3)

                ; mov rcx, MsgD
                ; mov rdx, -1
                ; mov r8, -1
                ; call callPrintfm


                pop rbx                 ; restore external rbx

                ret
                ; mov rcx, 0
                ; call ExitProcess        ; ExitProcess (0)
;-------------------------------------------
;-------------------------------------------


;-------------------------------------------
; x64 c calling convention
;
;       1st     RCX
;       2nd     RDX	
;	    3rd     R8	
;	    4th     R9
;       5th+    to stack
;-------------------------------------------


;-------------------------------------------
; Constants
;-------------------------------------------
section .data

MsgTitle        db "Title", 0ah, 0
MsgText         db "This function was written with asm", 0ah
MsgTextLength   equ ($ - MsgText)
MsgText2        db "Noooo", 0
MsgText3        db "Yess", 0ah, 0

; MsgPrintf       db "Ququququ '%s'%x|%d qwe%% rsp:%d,%d,%b,%d,%d", 0ah, 0
; MsgPrintf2      db "KUKUKUuu '%s'%x|%d qwe%% rsp:%d,%d,%b", 0ah, 0

MsgPrintfSimple db "Ququ string:'%s', dec:%d, dec:%d, dec:%d, dec:%d, char:%c~", 0ah, 0
MsgPrintf       db "Ququ string:'%s', hex:%x, oct:%o, bin:%b, dec:%d, char:%c~", 0ah, 0
StringPrintf    db "Yeaa", 0
MsgD            db "|%d| |%b|", 0ah , 0

STD_OUTPUT_HANDLE equ (-11)

PrintfParam1    db "Asm : %s-%s-%s-%s-%s%d %s %x %d%%%c%b", 0ah, 0
PrintfParam2    db "aaa", 0
PrintfParam3    db "bbb", 0
PrintfParam4    db "ccc", 0
PrintfParam5    db "ddd", 0
PrintfParam6    db "eee", 0
PrintfParam7    equ -1
PrintfParam8    db "love", 0
PrintfParam9    equ 3802
PrintfParam10   equ 100
PrintfParam11   equ 33
PrintfParam12   equ 127

;-------------------------------------------
;-------------------------------------------
; ld hello.obj "C:\Windows\System32\kernel32.dll" "C:\Windows\System32\user32.dll" -e __start
;       or
; nasm printfm.asm -f win64
;-------------------------------------------
;-------------------------------------------
