#include <Arduino.h>

#include "leds.h"
#include "configuracoes.h"


// =====================================
// VARIAVEIS DE CONTROLE
// =====================================

static EstadoTerminal estadoTerminal = EstadoTerminal::PRONTO;

static bool sinalizacaoMicroSDAtiva = false;
static bool sinalizacaoImpressoraAtiva = false;

static bool estadoLed = false;
static bool aguardandoPausa = false;

static uint8_t quantidadePiscadas = 0;

static unsigned long ultimoTempo = 0;

static constexpr unsigned long INTERVALO_PISCADA_MS = 200;
static constexpr unsigned long INTERVALO_PAUSA_MS = 1500;

static unsigned long intervaloAtual = INTERVALO_PISCADA_MS;


// =====================================
// INICIALIZAR LED
// =====================================

void inicializarLed() {

    pinMode(PIN_LED, OUTPUT);

    estadoTerminal = EstadoTerminal::PRONTO;

    sinalizacaoMicroSDAtiva = false;
    sinalizacaoImpressoraAtiva = false;

    estadoLed = true;
    aguardandoPausa = false;
    quantidadePiscadas = 0;

    digitalWrite(PIN_LED, HIGH);
}


// =====================================
// DEFINIR ESTADO DO TERMINAL
// =====================================

void definirEstadoTerminal(EstadoTerminal novoEstado) {

    estadoTerminal = novoEstado;
}


// =====================================
// INICIAR SINALIZACAO DE FALHA
// =====================================

static void iniciarSinalizacaoFalha() {

    estadoLed = true;
    quantidadePiscadas = 0;
    aguardandoPausa = false;

    intervaloAtual = INTERVALO_PISCADA_MS;
    ultimoTempo = millis();

    digitalWrite(PIN_LED, HIGH);
}


// =====================================
// INICIAR SINALIZACAO DO MICROSD
// =====================================

void iniciarSinalizacaoMicroSD() {

    sinalizacaoMicroSDAtiva = true;

    iniciarSinalizacaoFalha();
}


// =====================================
// PARAR SINALIZACAO DO MICROSD
// =====================================

void pararSinalizacaoMicroSD() {

    sinalizacaoMicroSDAtiva = false;

    if (!sinalizacaoImpressoraAtiva) {

        aguardandoPausa = false;
        quantidadePiscadas = 0;

        estadoLed = (estadoTerminal == EstadoTerminal::PRONTO);

        digitalWrite(PIN_LED, estadoLed ? HIGH : LOW);
    }
}


// =====================================
// INICIAR SINALIZACAO DA IMPRESSORA
// =====================================

void iniciarSinalizacaoImpressora() {

    sinalizacaoImpressoraAtiva = true;

    iniciarSinalizacaoFalha();
}


// =====================================
// PARAR SINALIZACAO DA IMPRESSORA
// =====================================

void pararSinalizacaoImpressora() {

    sinalizacaoImpressoraAtiva = false;

    if (!sinalizacaoMicroSDAtiva) {

        aguardandoPausa = false;
        quantidadePiscadas = 0;

        estadoLed = (estadoTerminal == EstadoTerminal::PRONTO);

        digitalWrite(PIN_LED, estadoLed ? HIGH : LOW);
    }
}


// =====================================
// ATUALIZAR SINALIZACAO
// =====================================

void atualizarSinalizacao() {

    // Verifica se existe alguma falha ativa
    if (
        !sinalizacaoMicroSDAtiva &&
        !sinalizacaoImpressoraAtiva
    ) {

        estadoLed = (estadoTerminal == EstadoTerminal::PRONTO);

        digitalWrite(PIN_LED, estadoLed ? HIGH : LOW);

        return;
    }

    unsigned long agora = millis();

    if (agora - ultimoTempo < intervaloAtual) {
        return;
    }

    ultimoTempo = agora;

    // Durante a pausa, aguarda para reiniciar
    if (aguardandoPausa) {

        aguardandoPausa = false;

        quantidadePiscadas = 0;

        intervaloAtual = INTERVALO_PISCADA_MS;

        estadoLed = true;

        digitalWrite(PIN_LED, HIGH);

        return;
    }

    // Alterna o estado do LED
    estadoLed = !estadoLed;

    digitalWrite(PIN_LED, estadoLed ? HIGH : LOW);

    // Conta uma piscada quando o LED apaga
    if (!estadoLed) {

        quantidadePiscadas++;

        if (quantidadePiscadas >= 2) {

            aguardandoPausa = true;

            intervaloAtual = INTERVALO_PAUSA_MS;
        }
    }
}