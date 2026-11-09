
#include <Arduino.h>
#include <SD.h>
#include <limits.h>

#include "sorteio.h"
#include "microsd.h"


// =====================================
// QUANTIDADE DE CATEGORIAS
// =====================================

static constexpr uint8_t QUANTIDADE_CATEGORIAS = 4;


// =====================================
// HISTORICO DOS SORTEIOS
// =====================================

// Ultimo indice confirmado de cada categoria
static uint32_t ultimoIndice[QUANTIDADE_CATEGORIAS] = {0};

// Indica se cada categoria ja possui historico
static bool possuiUltimoIndice[QUANTIDADE_CATEGORIAS] = {false};

// Indice do sorteio que aguarda confirmacao
static uint32_t indicePendente[QUANTIDADE_CATEGORIAS] = {0};

// Indica se existe um sorteio pendente
static bool sorteioPendente[QUANTIDADE_CATEGORIAS] = {false};


// =====================================
// IDENTIFICAR INDICE DA CATEGORIA
// =====================================

static int obterIndiceCategoria(Categoria categoria) {

    switch (categoria) {

        case Categoria::POESIA:
            return 0;

        case Categoria::CHARGE:
            return 1;

        case Categoria::COLORIR:
            return 2;

        case Categoria::FRASE:
            return 3;

        default:
            return -1;
    }
}


// =====================================
// INICIALIZAR SORTEIO
// =====================================

void inicializarSorteio() {

    randomSeed(micros());
}


// =====================================
// SORTEAR ARQUIVO
// =====================================

File sortearArquivo(
    const char* caminho,
    Categoria categoria
) {

    int indiceCategoria = obterIndiceCategoria(categoria);

    if (indiceCategoria < 0 || caminho == nullptr) {
        return File();
    }

    // Descarta eventual sorteio anterior ainda nao confirmado
    sorteioPendente[indiceCategoria] = false;

    // Conta os arquivos disponiveis
    uint32_t quantidadeArquivos = contarArquivos(caminho);

    if (quantidadeArquivos == 0) {
        return File();
    }

    // random() utiliza um limite do tipo long
    if (quantidadeArquivos > (uint32_t)LONG_MAX) {
        return File();
    }

    // Verifica se o ultimo indice ainda existe
    bool podeEvitarRepeticao =
        possuiUltimoIndice[indiceCategoria] &&
        ultimoIndice[indiceCategoria] < quantidadeArquivos;

    uint32_t indiceSorteado;

    // Evita repetir o ultimo arquivo quando existem alternativas
    if (
        quantidadeArquivos > 1 &&
        podeEvitarRepeticao
    ) {

        // Sorteia entre todos os indices, exceto o ultimo
        indiceSorteado = (uint32_t)random(
            (long)(quantidadeArquivos - 1)
        );

        // Ajusta o indice para pular o ultimo sorteado
        if (indiceSorteado >= ultimoIndice[indiceCategoria]) {
            indiceSorteado++;
        }

    } else {

        // Primeiro sorteio ou historico invalido
        indiceSorteado = (uint32_t)random(
            (long)quantidadeArquivos
        );
    }

    // Abre o arquivo selecionado
    File arquivo = abrirArquivoBMP(
        caminho,
        indiceSorteado
    );

    if (!arquivo) {
        return File();
    }

    // Guarda o indice aguardando confirmacao
    indicePendente[indiceCategoria] = indiceSorteado;

    sorteioPendente[indiceCategoria] = true;

    return arquivo;
}


// =====================================
// CONFIRMAR SORTEIO
// =====================================

void confirmarSorteio(Categoria categoria) {

    int indiceCategoria = obterIndiceCategoria(categoria);

    if (indiceCategoria < 0) {
        return;
    }

    // So atualiza o historico se existir sorteio pendente
    if (!sorteioPendente[indiceCategoria]) {
        return;
    }

    ultimoIndice[indiceCategoria] =
        indicePendente[indiceCategoria];

    possuiUltimoIndice[indiceCategoria] = true;

    sorteioPendente[indiceCategoria] = false;
}
