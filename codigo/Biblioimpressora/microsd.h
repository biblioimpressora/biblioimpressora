#ifndef MICROSD_H
#define MICROSD_H

#include <Arduino.h>
#include <SD.h>

bool inicializarMicroSD();

uint32_t contarArquivos(const char* caminho);

File abrirArquivoBMP(
    const char* caminho,
    uint32_t indice
);

#endif