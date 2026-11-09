#ifndef LEDS_H
#define LEDS_H

enum class EstadoTerminal {
    PRONTO,
    OCUPADO
};

void inicializarLed();

void definirEstadoTerminal(EstadoTerminal novoEstado);

void iniciarSinalizacaoMicroSD();
void pararSinalizacaoMicroSD();

void iniciarSinalizacaoImpressora();
void pararSinalizacaoImpressora();

void atualizarSinalizacao();

#endif