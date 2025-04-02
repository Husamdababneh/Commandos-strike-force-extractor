; File: chkstk.asm
; Assemble with MASM	
; https://github.com/skeeto/w64devkit/blob/master/src/libchkstk.S
.CODE
PUBLIC __chkstk
__chkstk PROC
    push    rax
    push    rcx
    mov     rcx, gs:[10h]   ; rcx = stack low address
    neg     rax             ; rax = frame low address
    add     rax, rsp        ; "
    jb      overflow        ; frame low address overflow?
    xor     eax, eax        ; overflowed: frame low address = null

commit_stack:
    sub     rcx, 1000h      ; extend stack into guard page
    test    eax, [rcx]      ; commit page (two instruction bytes)
    cmp     rcx, rax
    ja      commit_stack

overflow:
    pop     rcx
    pop     rax
    ret
__chkstk ENDP
END

