#ifndef BOTOES_H
#define BOTOES_H

#include <Arduino.h>
#include "categorias.h"

// Inicializa os quatro botoes
void inicializarBotoes();

// Verifica se algum botao foi pressionado
bool lerBotao(Categoria &categoriaPressionada);

#endif
