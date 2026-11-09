#ifndef SORTEIO_H
#define SORTEIO_H

#include <Arduino.h>
#include <SD.h>

#include "categorias.h"

void inicializarSorteio();

// Sorteia e abre uma imagem BMP
File sortearArquivo(const char* caminho, Categoria categoria);

// Confirma que a impressao comecou
void confirmarSorteio(Categoria categoria);

#endif