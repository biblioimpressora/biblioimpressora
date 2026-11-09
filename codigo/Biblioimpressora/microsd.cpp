#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

#include "microsd.h"
#include "configuracoes.h"


// =====================================
// ABRIR PASTA DO MICROSD
// =====================================

static File abrirPasta(const char* caminho) {

    if (caminho == nullptr) {
        return File();
    }

    File pasta = SD.open(caminho);

    if (!pasta || !pasta.isDirectory()) {
        pasta.close();
        return File();
    }

    return pasta;
}


// =====================================
// VERIFICAR EXTENSAO BMP
// =====================================

static bool possuiExtensaoBMP(const char* nome) {

    if (nome == nullptr) {
        return false;
    }

    size_t tamanho = strlen(nome);

    if (tamanho < 4) {
        return false;
    }

    const char* extensao = nome + tamanho - 4;

    return
        extensao[0] == '.' &&
        tolower((unsigned char)extensao[1]) == 'b' &&
        tolower((unsigned char)extensao[2]) == 'm' &&
        tolower((unsigned char)extensao[3]) == 'p';
}


// =====================================
// CONTAR ARQUIVOS BMP
// =====================================

uint32_t contarArquivos(const char* caminho) {

    File pasta = abrirPasta(caminho);

    if (!pasta) {
        return 0;
    }

    uint32_t quantidade = 0;

    while (true) {

        File arquivo = pasta.openNextFile();

        if (!arquivo) {
            break;
        }

        if (
            !arquivo.isDirectory() &&
            arquivo.size() > 0 &&
            possuiExtensaoBMP(arquivo.name())
        ) {

            if (quantidade < UINT32_MAX) {
                quantidade++;
            }

        }

        arquivo.close();
    }

    pasta.close();

    return quantidade;
}


// =====================================
// ABRIR BMP PELO INDICE
// =====================================

File abrirArquivoBMP(
    const char* caminho,
    uint32_t indice
) {

    File pasta = abrirPasta(caminho);

    if (!pasta) {
        return File();
    }

    uint32_t indiceAtual = 0;

    while (true) {

        File arquivo = pasta.openNextFile();

        if (!arquivo) {
            break;
        }

        bool arquivoValido =
            !arquivo.isDirectory() &&
            arquivo.size() > 0 &&
            possuiExtensaoBMP(arquivo.name());

        if (arquivoValido) {

            if (indiceAtual == indice) {

                pasta.close();

                return arquivo;
            }

            indiceAtual++;
        }

        arquivo.close();
    }

    pasta.close();

    return File();
}


// =====================================
// INICIALIZAR MICROSD
// =====================================

bool inicializarMicroSD() {

    // Configura os pinos da comunicacao SPI
    SPI.begin(
        PIN_MICROSD_SCK,
        PIN_MICROSD_MISO,
        PIN_MICROSD_MOSI,
        PIN_MICROSD_CS
    );

    // Inicializa o cartao
    if (!SD.begin(PIN_MICROSD_CS, SPI)) {
        return false;
    }

    return true;
}