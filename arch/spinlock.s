;
; spinlock.s
; Joel Wolfrath, 2013
;
; x86 spinlock impl
;

[GLOBAL _lock]
_lock:
    mov edx, [esp+8]            ; Fetch the address off the stack
    mov eax, 1
_lock_try_acquire:
    xchg eax, [edx]
    test eax, eax               ; Verify we acquired the lock
    jnz _lock_wait
    ret
_lock_wait:                     ; avoid exclusive use of the memory bus
    pause                       ; hyperthreading performance
    test eax, [edx]
    jnz _lock_wait
    jmp _lock_try_acquire

[GLOBAL _unlock]
_unlock:
    xor eax, eax
    mov edx, [esp+8]            ; Fetch the address off the stack
    xchg eax, [edx]             ; Technically, a strict memory fence is not
                                ; required, but enforcing for sanity or
                                ; until performance is a real issue.
