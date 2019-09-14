;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Declaracao de Pinos
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
SA EQU P1.2
SB EQU P1.1
STRD EQU P3.7
BZ EQU P1.0
B1 EQU P1.6
B2 EQU P1.3
B3 EQU P1.4
B4 EQU P1.5
LCD EQU P0
E EQU P3.1
SENSOR EQU P2
RELE EQU P3.2
WR1 EQU P3.6
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Declaração de variaveis
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
INC_DEC EQU 0030H
AD_TEMP EQU 0031H
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Declaração de Bits
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
F_BOTAO BIT 78H
F_BOTAO1 BIT 79H
F_BOTAO2 BIT 7AH
F_BOTAO3 BIT 7BH
F_LIGA_DESLIGA BIT 7DH
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Vetores de Interrupções
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

ORG 0000H
    LJMP INICIO
ORG 0003H
    JMP TRATA_INT0
ORG 000BH
    JMP TRATA_TMR0
ORG 0013H
    JMP TRATA_INT1
ORG 001BH
    JMP TRATA_TMR1
ORG 0023H
    JMP TRATA_SERIAL
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Tratativa de Interrupções
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
TRATA_INT0:
    RETI
TRATA_TMR0:
    RETI
TRATA_INT1:
    RETI
TRATA_TMR1:
    RETI
TRATA_SERIAL:
    RETI

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Subrotinas
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Subrotinas do LCD   ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Rotina de Inicialização
INICIA_LCD:
    MOV A, #38H
        LCALL COMANDO_LCD
        MOV A, #06H
        LCALL COMANDO_LCD
        MOV A, #0CH
        LCALL COMANDO_LCD
        MOV A, #01H
        LCALL COMANDO_LCD
    RET

;Rotina de Configurações (Escrita de Instruções no LCD)
COMANDO_LCD:
    CLR RS
    MOV LCD, A
        SETB E
        CALL ATRASO
        CLR E
    RET

;Rotina de Escrita de Dados na Tela do LCD
DADO_LCD:
    SETB RSTMOV LCD, A
        SETB E
        CALL ATRASO
        CLR E
    RET

;Rotina de Escrita de Frase na Tela do LCD
MENSAGEM:
    CLR A
    MOVC A, @A+DPTR
    CJNE A, #'%', MOSTRA
        RET
MOSTRA:
    CALL DADO_LCD
    INS DPTR
        JMP MENSAGEM

;Atraso de 10ms
ATRASO:
    MOV R1, #255
    MOV R2, #20
LOOP:
    DJNZ R1, $
    DJNZ R1, LOOP
    RET

LE_AD:
    CLR WR1
    CALL ATRASO
    SETB WR1
    MOV AD_TEMP, SENSOR
    MOV A, AD_TEMP
    RL A
    MOV AD_TEMP, A
    RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Inicio do programa
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

INICIO:

    MOV LCD, #11111111B
    CLR F_BOTAO1
    CLR F_BOTAO2
    CLR F_BOTAO3
    CLR F_BOTAO4
    CLR F_LIGA_DESLIGA
    CALL INICIA_LCD

    MOV A, #080H
        CALL COMANDO_LCD
    MOV DPTR, #FRASE_INICIO
    CALL MENSAGEM
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;Loop Principal
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
MAIN:
;;;;;  Rotina Botão ON/OFF   ;;;;;
R_LER_B1:

    JB F_BOTAO1, B1_SOLTO
    JB B1, R_LER_B1
    SETB F_BOTAO1
    JMP FIM_R_LER_B1

B1_SOLTO:
    JB B1, FIM_R_LER_B1
    CLR F_BOTAO1
    SETB F_LIGA_DESLIGA
    MOV A, #080H
    CALL COMANDO_LCD
    MOV DPTR, #TEMP_A
    CALL MENSAGEM

    MOV A, #0C0H
    CALL COMANDO_LCD
    MOV DPTR, #TEMP_P
    CALL MENSAGEM

    CLR A
    MOV A, #088H
    CALL COMANDO_LCD
    CLR A
    MOV DPTR, #TABELA
    MOVC A, @A+DPTR
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    INC A
    MOV INC_DEC, A
    MOV B, #10
    DIV A B
    ADD A, #30H
    CALL DADO_LCD
    MOV A, B
    ADD A,#30H
    CALL DADO_LCD

    JMP R_LER_SA

FIM_R_LER_B1:
    JMP R_LER_B1

;;;;;;;;; Rotina liga Buzzer ;;;;;;;;
R_LER_SA:
    JB SA, FIM_BUZZER
    CLR BZ
    JMP TEMP_SENSOR

FIM_BUZZER:
    SETB BZ

;;;;; Rotina leitura Sensor de Temperatura ;;;;;
TEMP_SENSOR:

    MOV A, #0C8H
    CALL COMANDO_LCD
    MOV B, #10
    CALL LE_AD
    MOV , AD_TEMP
    DIV A B
    ADD A, #30H
    CALL DADO_LCD

;;;;; Rotina Botão Incrementa ;;;;;

R_LER_B2:

    JB F_BOTAO2, B2_SOLTO
    JB B2, FIM_R_LER_B2
    SETB F_BOTAO2
    JMP R_LER_B2

B2_SOLTO:
    JNB B2, R_LER_B2
    CLR F_BOTAO2

    MOV A, INC_DEC
    CJNE A, #25H, PROGRESSO_B2
    JMP FIM_R_LER_B2

PROGRESSO_B2:
    MOV A, #088H
    CALL COMANDO_LCD

    CLR A

    MOV A, INC_DEC
    MOV B, #10
    INC AMOV INC_DEC, A
    DIV A B
    ADD A, #30H
    CALL DADO_LCD

    MOV A, BADD A, #30H
    CALL DADO_LCD
    LJMP R_LER_SA

FIM_R_LER_B2:
    JMP R_LER_B3

;;;;; Rotina Botão decrementa ;;;;;

R_LER_B3:
    JB F_BOTAO3, B3_SOLTO
    JB B3, FIM_R_LER_B3
    SETB F_BOTAO3
    JMP R_LER_B3

B3_SOLTO:
    JNB B3, R_LER_B3
    CLR F_BOTAO3
    MOV A, INC_EC
    CJNE A, #19H, PROGRESSO_B3
    JMP FIM_R_LER_B3

PROGRESSO_B3:
    MOV A, #088H
    CALL COMANDO_LCD
    CLR A
    MOV A, INC_DEC
    DEC A
    MOV INC_DEC, A
    MOV B, #10
    DIV A BADD A, #30H
    CALL DADO_LCD

    MOV A, B
    ADD A, #30H
    CALL DADO_LCD
    LJMP R_LER_SA
FIM_R_LER_B3:
    JMP R_LER_B4

;;;;; Rotina Botao Auto Programação ;;;;;

R_LER_B4:
    JB F_BOTAO4, B4_SOLTO
    JB B4, FIM_R_LER_B4
    SETB F_BOTAO4
    JMP R_LER_B4

B4_SOLTO:
    JNB B4, R_LER_B4
    CLR F_BOTAO4

    MOV A, #088H
    CALL COMANDO_LCD

    CLR AMOV A, #25H
    MOV INC_DEC, A
    MOV B, #10
    DIV A B
    ADD A, #30H
    CALL DADO_LCD

    MOV A, BADD A, #30H
    CALL DADO_LCD
    LJMP R_LER_SA

FIM_R_LER_B4:
    JMP R_LER_SB

;;;;; Rotina Leitura de Sensor Baixo ;;;;;

R_LER_SB:

    JNB SB, R_LER_BOT1_2
    LJMP R_LER_SA

;;;;; Rotina Desligar Sistema ;;;;;

R_LER_BOT1_2:

    JB B1, R_LER_BOT1_3
    CLR F_LIGA_DESLIGA

R_LER_BOT1_3:
    JB F_LIGA_DESLIGA, COMPARACAO

R_LER_BOT1_4:
    JNB B1, R_LER_BOT1_4
    LJMP MAIN

;;;;; Rotina Comparação ;;;;;

COMPARACAO:

    CALL LE_AD
    CLR A
    MOV A, INC_DEC
    MOV B, AD_TEMP
    CLR C
    SUBB A, B
    JC DESLIGA_RTR
    CLR RELE
    LJMP R_LER_SA

DESLIGA_RTR:
    SETB RELE

FIM_PG:
    LJMP R_LER_SA

;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Frases Armazenadas na ROM ;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Memoria ROM ;;;

FRASE_INICIO: DB 'DESLIGADO       ','%' ; Frase de inicio do LCD
TEMP_A: DB 'TEMP.A           ','%' ; Temperatura Ajustada
TEMP_p: DB 'TEMP.P           ','%' ; Temperatura Programada
TABELA: DB 19H, 1AH, 1BH, 1CH, 1DH, 1EH, 1FH, 20H, 21H, 22H, 23H, 24H, 25H ; Tabela de numeros LCD

END