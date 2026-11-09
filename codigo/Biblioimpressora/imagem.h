#ifndef IMAGEM_H
#define IMAGEM_H

#include <Arduino.h>
#include <SD.h>

// Verifica o cabecalho e identifica as caracteristicas do BMP
bool verificarCabecalhoBMP(
    File &arquivo,
    int32_t &largura,
    int32_t &altura,
    uint16_t &profundidade,
    uint32_t &inicioPixels
);

// Le uma linha de pixels da imagem BMP
bool lerLinhaBMP(
    File &arquivo,
    uint8_t *buffer,
    uint32_t bytesPorLinha
);

// Posiciona o arquivo no inicio de uma linha
bool posicionarLinhaBMP(
    File &arquivo,
    uint32_t inicioPixels,
    uint32_t bytesPorLinha,
    int32_t altura,
    int32_t numeroLinha
);

// Obtem a altura absoluta da imagem
uint32_t obterAlturaBMP(int32_t altura);

// Le o valor de um pixel
uint8_t lerPixelBMP(
    const uint8_t* linha,
    uint32_t coluna
);

// Verifica a paleta de cores
bool verificarPaletaBMP(
    File &arquivo,
    uint32_t inicioPixels,
    bool &corZeroPreta
);

// Identifica se um pixel representa a cor preta
bool pixelEhPreto(uint8_t valorPixel, bool corZeroPreta);

// Conta os pixels pretos de uma linha
uint32_t contarPixelsPretos(
    const uint8_t* linha,
    uint32_t largura,
    bool corZeroPreta
);

// Converte uma linha para o formato de impressao
bool converterLinhaBMP(
    const uint8_t* linha,
    uint32_t largura,
    bool corZeroPreta,
    uint8_t* dadosImpressao,
    uint32_t tamanhoBuffer
);

// Obtem a quantidade de bytes necessarios para impressao
uint32_t obterBytesImpressaoBMP(uint32_t largura);

// Obtem a quantidade de bytes armazenados por linha no BMP
uint32_t obterBytesLinhaBMP(int32_t largura);

// Verifica se a largura e valida
bool larguraBMPValida(int32_t largura);

// Verifica se a altura e valida
bool alturaBMPValida(int32_t altura);

// Le uma linha completa da imagem
bool lerLinhaBMPCompleta(
    File &arquivo,
    uint32_t inicioPixels,
    int32_t altura,
    int32_t numeroLinha,
    uint32_t bytesPorLinha,
    uint8_t* buffer
);

// Prepara uma linha BMP para impressao
bool prepararLinhaBMP(
    File &arquivo,
    uint32_t inicioPixels,
    int32_t altura,
    int32_t numeroLinha,
    uint32_t largura,
    uint32_t bytesPorLinha,
    bool corZeroPreta,
    uint8_t* bufferOriginal,
    uint32_t tamanhoBufferOriginal,
    uint8_t* dadosImpressao,
    uint32_t tamanhoBufferImpressao
);

#endif