#ifndef REGISTRO_H
#define REGISTRO_H

#include <Arduino.h>
#include "categorias.h"
#include "relogio.h"

// Resultados possiveis de uma operacao
enum class ResultadoOperacao {
    SELECAO,
    SUCESSO,
    FALHA
};

bool inicializarRegistro();

bool registrarOperacao(
    const DataHora &dataHora,
    Categoria categoria,
    const char* nomeArquivo,
    ResultadoOperacao resultado
);

#endif