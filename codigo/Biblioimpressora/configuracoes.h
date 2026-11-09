#ifndef CONFIGURACOES_H
#define CONFIGURACOES_H
#include <Arduino.h>

// ==============================
// PINOS DOS BOTOES
// ==============================

constexpr uint8_t PIN_BOTAO_POESIA = 4;
constexpr uint8_t PIN_BOTAO_CHARGE = 5;
constexpr uint8_t PIN_BOTAO_COLORIR = 6;
constexpr uint8_t PIN_BOTAO_FRASE = 15;


// ==============================
// PINO DO LED
// ==============================

constexpr uint8_t PIN_LED = 7;

// ==============================
// PINOS DO CARTAO MICROSD
// ==============================

constexpr uint8_t PIN_MICROSD_CS = 10; //CS: seleciona o cartão.
constexpr uint8_t PIN_MICROSD_MOSI = 11; //MOSI: envia dados da ESP32 para o cartão.
constexpr uint8_t PIN_MICROSD_SCK = 12; //SCK: fornece o sinal de sincronização.
constexpr uint8_t PIN_MICROSD_MISO = 13; //MISO: recebe dados do cartão.

// ==============================
// PINOS DO RELOGIO RTC
// ==============================

constexpr uint8_t PIN_RTC_SDA = 8;
constexpr uint8_t PIN_RTC_SCL = 9;

// ==============================
// PINOS DA IMPRESSORA
// ==============================

constexpr uint8_t PIN_IMPRESSORA_RX = 18; //RX: recebe os dados enviados pelo TX do módulo.
constexpr uint8_t PIN_IMPRESSORA_TX = 17; //TX: envia dados para o RX do módulo.

// ==============================
// PARAMETROS GERAIS
// ==============================

// Comunicacao com o computador
constexpr uint32_t VELOCIDADE_MONITOR_SERIAL = 115200;

// Tempo para evitar multiplos acionamentos
constexpr uint16_t TEMPO_DEBOUNCE_MS = 50;

// ==============================
// PARAMETROS DA IMPRESSORA
// ==============================

// Largura provisoria da area de impressao
constexpr uint16_t LARGURA_IMPRESSAO = 576;

// PENDENTE: confirmar a velocidade no autoteste.
// Definir VELOCIDADE_IMPRESSORA_BAUD apos o teste.

// PENDENTE: validar a largura de 576 pontos.

// PENDENTE: testar a tabela PC860 e os caracteres acentuados.

// PENDENTE: testar o comando de consulta do sensor de papel.

// PENDENTE: testar o comando de corte parcial.


// ==============================
// OUTRAS CONFIGURACOES PENDENTES
// ==============================

// PENDENTE: definir intervalo de verificacao
// da disponibilidade da impressora.

// PENDENTE: definir intervalo da sinalizacao
// de falha do cartao microSD.

// PENDENTE: definir os tempos de sinalizacao
// do LED conforme o fluxograma.

constexpr uint32_t VELOCIDADE_IMPRESSORA_BAUD = 9600; // PROVISORIO
#endif



