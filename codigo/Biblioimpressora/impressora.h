#ifndef IMPRESSORA_H
#define IMPRESSORA_H

#include <Arduino.h>
#include <SD.h>

// Inicializa a comunicacao com a impressora
void inicializarImpressora();

// Verifica se a impressora esta disponivel
bool impressoraDisponivel();

// Imprime uma imagem BMP
bool imprimirImagemBMP(File &arquivo);

#endif