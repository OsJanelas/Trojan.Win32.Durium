bits 16
org 0x7C00

%define WSCREEN 320
%define HSCREEN 200

start:
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

    finit               ; Inicializa FPU

    mov ax, 0x0013
    int 0x10

main_loop:
    inc byte [time]
    
    ; Prepara o valor do tempo para a FPU (convertendo byte para word)
    movzx ax, byte [time]
    mov [temp_w], ax

    ; --- Cálculo de Seno e Cosseno uma vez por frame (Otimização) ---
    fild word [temp_w]  ; Carrega o tempo
    fsincos             ; Calcula Seno e Cosseno simultaneamente!
    fstp dword [cosa]   ; Salva Cos (ST0)
    fstp dword [sina]   ; Salva Sin (ST1)

    push es
    mov ax, 0xA000
    mov es, ax
    xor di, di

    mov word [y], 0
.y_loop:
    mov word [x], 0
.x_loop:
    ; Centralização básica (X - 160)
    mov ax, [x]
    sub ax, 160
    mov [temp_w], ax
    fild word [temp_w]  ; Carrega X relativo
    fmul dword [cosa]   ; X * cos

    ; (Y - 100)
    mov ax, [y]
    sub ax, 100
    mov [temp_w], ax
    fild word [temp_w]  ; Carrega Y relativo
    fmul dword [sina]   ; Y * sin

    fsubp               ; (X*cos) - (Y*sin)
    fistp word [gx]     ; Resultado para gx

    ; Cor e desenho
    mov ax, [gx]
    add al, [time]
    stosb               ; Escreve pixel

    inc word [x]
    cmp word [x], WSCREEN
    jl .x_loop

    inc word [y]
    cmp word [y], HSCREEN
    jl .y_loop
    pop es

    ; --- Texto ---
    mov ah, 0x02
    xor bh, bh
    mov dx, 0x010E      ; Linha 1, Coluna 14
    int 0x10

    mov si, msg
    mov bl, [time]
.print:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    inc bl
    int 0x10
    jmp .print
.done:

    jmp main_loop

; --- Dados ---
time:   db 0
temp_w: dw 0            ; Variável temporária para conversão de tipos
x:      dw 0
y:      dw 0
gx:     dw 0
sina:   dd 0.0          ; dword (4 bytes) para combinar com fmul
cosa:   dd 0.0          ; dword (4 bytes) para combinar com fmul
msg:    db "The power of Durium", 0

times 510-($-$$) db 0
dw 0xAA55