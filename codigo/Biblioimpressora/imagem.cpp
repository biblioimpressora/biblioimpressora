#include <Arduino.h>
#include <SD.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>

#include "configuracoes.h"
#include "imagem.h"

// =====================================
// CALCULAR BYTES POR LINHA
// =====================================

static uint32_t calcularBytesPorLinha(int32_t largura) {

    return ((uint32_t)largura + 31) / 32 * 4;

}


// =====================================
// LER UM NUMERO DE 4 BYTES
// =====================================

static uint32_t lerNumero32(File &arquivo) {

    uint8_t bytes[4];

    if (arquivo.read(bytes, 4) != 4) {
        return 0;
    }

    return
        (uint32_t)bytes[0] |
        ((uint32_t)bytes[1] << 8) |
        ((uint32_t)bytes[2] << 16) |
        ((uint32_t)bytes[3] << 24);

}


// =====================================
// LER UM NUMERO DE 2 BYTES
// =====================================

static uint16_t lerNumero16(File &arquivo) {

    uint8_t bytes[2];

    if (arquivo.read(bytes, 2) != 2) {
        return 0;
    }

    return
        (uint16_t)bytes[0] |
        ((uint16_t)bytes[1] << 8);

}


// =====================================
// VERIFICAR CABECALHO BMP
// =====================================

bool verificarCabecalhoBMP(
    File &arquivo,
    int32_t &largura,
    int32_t &altura,
    uint16_t &profundidade,
    uint32_t &inicioPixels
) {

    // Volta para o inicio do arquivo
    if (!arquivo || !arquivo.seek(0)) {
        return false;
    }

    if (arquivo.size() < 34) {
        return false;
    }

    // Verifica a identificacao BM
    uint8_t identificador[2];

    if (arquivo.read(identificador, 2) != 2) {
        return false;
    }

    if (identificador[0] != 'B' || identificador[1] != 'M') {
        return false;
    }

    // Descobre onde comecam os pixels
    if (!arquivo.seek(10)) {
        return false;
    }

    inicioPixels = lerNumero32(arquivo);

    // Verifica o tamanho do cabecalho
    if (!arquivo.seek(14)) {
        return false;
    }

    uint32_t tamanhoCabecalho = lerNumero32(arquivo);

    if (tamanhoCabecalho != 40) {
        return false;
    }

    // Largura e altura
    if (!arquivo.seek(18)) {
        return false;
    }

    largura = (int32_t)lerNumero32(arquivo);
    altura = (int32_t)lerNumero32(arquivo);

    if (!larguraBMPValida(largura) || !alturaBMPValida(altura)) {
        return false;
    }

    // Verifica a quantidade de planos
    if (!arquivo.seek(26)) {
        return false;
    }

    uint16_t planos = lerNumero16(arquivo);

    if (planos != 1) {
        return false;
    }

    // Profundidade de cor
    if (!arquivo.seek(28)) {
        return false;
    }

    profundidade = lerNumero16(arquivo);

    if (profundidade != 1) {
        return false;
    }

    // Verifica se a imagem nao possui compressao
    if (!arquivo.seek(30)) {
        return false;
    }

    uint32_t compressao = lerNumero32(arquivo);

    if (compressao != 0) {
        return false;
    }

    // Verifica se o inicio dos pixels esta dentro do arquivo
    // A paleta de um BMP de 1 bit ocupa 8 bytes
// e comeca na posicao 54.
    if (inicioPixels < 62 || inicioPixels >= arquivo.size()) {
        return false;
    }

    // Verifica o tamanho dos dados dos pixels
    int64_t alturaAbsoluta = altura;

    if (alturaAbsoluta < 0) {
        alturaAbsoluta = -alturaAbsoluta;
    }

    uint64_t bytesPorLinha = calcularBytesPorLinha(largura);

    uint64_t tamanhoPixels =
        bytesPorLinha * alturaAbsoluta;

    uint64_t tamanhoArquivo = arquivo.size();

    if (tamanhoPixels > tamanhoArquivo - inicioPixels) {
        return false;
    }

    return true;

}


// =====================================
// LER UMA LINHA DE PIXELS
// =====================================

bool lerLinhaBMP(
    File &arquivo,
    uint8_t *buffer,
    uint32_t bytesPorLinha
) {

    if (!arquivo || buffer == nullptr || bytesPorLinha == 0) {
        return false;
    }

    if (arquivo.read(buffer, bytesPorLinha) != bytesPorLinha) {
        return false;
    }

    return true;

}


// =====================================
// POSICIONAR EM UMA LINHA DO BMP
// =====================================

bool posicionarLinhaBMP(
    File &arquivo,
    uint32_t inicioPixels,
    uint32_t bytesPorLinha,
    int32_t altura,
    int32_t numeroLinha
) {

    if (!arquivo || bytesPorLinha == 0 || altura == 0) {
        return false;
    }

    uint32_t alturaAbsoluta = obterAlturaBMP(altura);

    if (numeroLinha < 0 || (uint32_t)numeroLinha >= alturaAbsoluta) {
        return false;
    }

    // BMP com altura positiva armazena de baixo para cima
    uint32_t linhaArmazenada = numeroLinha;

    if (altura > 0) {
        linhaArmazenada = alturaAbsoluta - 1 - numeroLinha;
    }

    // Calcula a posicao usando 64 bits para evitar overflow
    uint64_t posicao =
        (uint64_t)inicioPixels +
        (uint64_t)linhaArmazenada * bytesPorLinha;

    if (posicao > UINT32_MAX) {
        return false;
    }

    return arquivo.seek((uint32_t)posicao);
}


// =====================================
// OBTER ALTURA ABSOLUTA DO BMP
// =====================================

uint32_t obterAlturaBMP(int32_t altura) {

    if (altura < 0) {
        return (uint32_t)(- (int64_t)altura);
    }

    return (uint32_t)altura;

}


// =====================================
// LER UM PIXEL DA LINHA
// =====================================

uint8_t lerPixelBMP(
    const uint8_t* linha,
    uint32_t coluna
) {

    if (linha == nullptr) {
        return 0;
    }

    uint32_t indiceByte = coluna / 8;

    uint8_t posicaoBit = 7 - (coluna % 8);

    return (linha[indiceByte] >> posicaoBit) & 1;

}


// =====================================
// VERIFICAR PALETA DE CORES DO BMP
// =====================================

bool verificarPaletaBMP(
    File &arquivo,
    uint32_t inicioPixels,
    bool &corZeroPreta
) {

    if (!arquivo || inicioPixels < 62) {
        return false;
    }

    if (!arquivo.seek(54)) {
        return false;
    }

    uint8_t paleta[8];

    if (arquivo.read(paleta, 8) != 8) {
        return false;
    }

    bool primeiraPreta =
        paleta[0] == 0 &&
        paleta[1] == 0 &&
        paleta[2] == 0;

    bool segundaPreta =
        paleta[4] == 0 &&
        paleta[5] == 0 &&
        paleta[6] == 0;

    bool primeiraBranca =
        paleta[0] == 255 &&
        paleta[1] == 255 &&
        paleta[2] == 255;

    bool segundaBranca =
        paleta[4] == 255 &&
        paleta[5] == 255 &&
        paleta[6] == 255;

    if (
        !((primeiraPreta && segundaBranca) ||
          (primeiraBranca && segundaPreta))
    ) {
        return false;
    }

    corZeroPreta = primeiraPreta;

    return true;

}


// =====================================
// IDENTIFICAR PIXEL PRETO
// =====================================

bool pixelEhPreto(uint8_t valorPixel, bool corZeroPreta) {

    if (valorPixel == 0) {
        return corZeroPreta;
    }

    return !corZeroPreta;

}


// =====================================
// CONTAR PIXELS PRETOS
// =====================================

uint32_t contarPixelsPretos(
    const uint8_t* linha,
    uint32_t largura,
    bool corZeroPreta
) {

    uint32_t quantidade = 0;

    for (uint32_t coluna = 0; coluna < largura; coluna++) {

        uint8_t valor = lerPixelBMP(linha, coluna);

        if (pixelEhPreto(valor, corZeroPreta)) {
            quantidade++;
        }

    }

    return quantidade;

}


// =====================================
// CONVERTER LINHA PARA IMPRESSAO
// =====================================

bool converterLinhaBMP(
    const uint8_t* linha,
    uint32_t largura,
    bool corZeroPreta,
    uint8_t* dadosImpressao,
    uint32_t tamanhoBuffer
) {

    if (linha == nullptr || dadosImpressao == nullptr || largura == 0) {
        return false;
    }

    uint32_t bytesNecessarios = (largura + 7) / 8;

    if (tamanhoBuffer < bytesNecessarios) {
        return false;
    }

    memset(dadosImpressao, 0, bytesNecessarios);

    for (uint32_t coluna = 0; coluna < largura; coluna++) {

        uint8_t valor = lerPixelBMP(linha, coluna);

        if (pixelEhPreto(valor, corZeroPreta)) {

            uint32_t indiceByte = coluna / 8;

            uint8_t posicaoBit = 7 - (coluna % 8);

            dadosImpressao[indiceByte] |= (1 << posicaoBit);

        }

    }

    return true;

}


// =====================================
// OBTER BYTES PARA IMPRESSAO
// =====================================

uint32_t obterBytesImpressaoBMP(uint32_t largura) {

    return (largura + 7) / 8;

}


// =====================================
// OBTER BYTES DA LINHA BMP
// =====================================

uint32_t obterBytesLinhaBMP(int32_t largura) {

    if (largura <= 0) {
        return 0;
    }

    return calcularBytesPorLinha(largura);

}


// =====================================
// VERIFICAR LARGURA DA IMAGEM
// =====================================

bool larguraBMPValida(int32_t largura) {

    return largura > 0 && largura <= LARGURA_IMPRESSAO;

}


// =====================================
// VERIFICAR ALTURA DA IMAGEM
// =====================================

bool alturaBMPValida(int32_t altura) {

    // Evita altura zero e o menor valor possivel de int32_t
    return altura != 0 && altura != INT32_MIN;
}


// =====================================
// LER UMA LINHA COMPLETA DO BMP
// =====================================

bool lerLinhaBMPCompleta(
    File &arquivo,
    uint32_t inicioPixels,
    int32_t altura,
    int32_t numeroLinha,
    uint32_t bytesPorLinha,
    uint8_t* buffer
) {

    if (buffer == nullptr) {
        return false;
    }

    if (!posicionarLinhaBMP(
        arquivo,
        inicioPixels,
        bytesPorLinha,
        altura,
        numeroLinha
    )) {
        return false;
    }

    return lerLinhaBMP(arquivo, buffer, bytesPorLinha);

}


// =====================================
// PREPARAR LINHA BMP PARA IMPRESSAO
// =====================================

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
) {

    if (
        bufferOriginal == nullptr ||
        dadosImpressao == nullptr ||
        largura == 0 ||
        largura > LARGURA_IMPRESSAO ||
        bytesPorLinha != calcularBytesPorLinha((int32_t)largura) ||
        bytesPorLinha > tamanhoBufferOriginal
    ) {
        return false;
    }

    if (!lerLinhaBMPCompleta(
        arquivo,
        inicioPixels,
        altura,
        numeroLinha,
        bytesPorLinha,
        bufferOriginal
    )) {
        return false;
    }

    return converterLinhaBMP(
        bufferOriginal,
        largura,
        corZeroPreta,
        dadosImpressao,
        tamanhoBufferImpressao
    );

}