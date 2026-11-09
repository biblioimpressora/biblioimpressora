
#ifndef RELOGIO_H
#define RELOGIO_H

#include <Arduino.h>

struct DataHora {
    uint16_t ano;
    uint8_t mes;
    uint8_t dia;
    uint8_t hora;
    uint8_t minuto;
    uint8_t segundo;
};

bool inicializarRelogio();

bool obterDataHora(DataHora &dataHora);

bool ajustarRelogio(
    uint16_t ano,
    uint8_t mes,
    uint8_t dia,
    uint8_t hora,
    uint8_t minuto,
    uint8_t segundo
);

#endif