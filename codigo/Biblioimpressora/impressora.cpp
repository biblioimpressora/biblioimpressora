
#include <Arduino.h>

#include "impressora.h"
#include "configuracoes.h"
#include "imagem.h"


// =====================================
// COMUNICACAO SERIAL COM A IMPRESSORA
// =====================================

// UART2 do ESP32
static HardwareSerial serialImpressora(2);


// =====================================
// CONFIGURAR A IMPRESSORA
// =====================================

static void configurarImpressora() {

    // Seleciona a tabela de caracteres PC860
    const uint8_t tabelaCaracteres[] = {
        0x1B, 0x74, 0x03
    };

    serialImpressora.write(
        tabelaCaracteres,
        sizeof(tabelaCaracteres)
    );

    // Define a largura da area de impressao
    const uint8_t largura[] = {
        0x1D, 0x57, 0x40, 0x02
    };

    serialImpressora.write(
        largura,
        sizeof(largura)
    );

    serialImpressora.flush();
}


// =====================================
// INICIALIZAR A IMPRESSORA
// =====================================

void inicializarImpressora() {

    serialImpressora.begin(
        VELOCIDADE_IMPRESSORA_BAUD,
        SERIAL_8N1,
        PIN_IMPRESSORA_RX,
        PIN_IMPRESSORA_TX
    );

    configurarImpressora();
}


// =====================================
// VERIFICAR DISPONIBILIDADE
// =====================================

bool impressoraDisponivel() {

    // Descarta respostas antigas
    while (serialImpressora.available() > 0) {
        serialImpressora.read();
    }

    // Consulta o sensor de papel
    const uint8_t comando[] = {
        0x10, 0x04, 0x04
    };

    if (
        serialImpressora.write(
            comando,
            sizeof(comando)
        ) != sizeof(comando)
    ) {
        return false;
    }

    serialImpressora.flush();

    // Aguarda a resposta
    unsigned long inicio = millis();

    while (serialImpressora.available() == 0) {

        if (millis() - inicio >= 200) {
            return false;
        }

        delay(1);
    }

    // Le a resposta
    int resposta = serialImpressora.read();

    if (resposta < 0) {
        return false;
    }

    // Interpretacao provisoria dos bits de estado do papel
    bool papelDisponivel =
        (((uint8_t)resposta & 0x60) == 0);

    return papelDisponivel;
}


// =====================================
// CORTAR PAPEL
// =====================================

static bool cortarPapel() {

    const uint8_t comando[] = {
        0x1D, 0x56, 0x01
    };

    if (
        serialImpressora.write(
            comando,
            sizeof(comando)
        ) != sizeof(comando)
    ) {
        return false;
    }

    serialImpressora.flush();

    return true;
}


// =====================================
// IMPRIMIR IMAGEM BMP
// =====================================

bool imprimirImagemBMP(File &arquivo) {

    // Verifica se o arquivo esta aberto
    if (!arquivo) {
        return false;
    }

    // Verifica a disponibilidade da impressora
    if (!impressoraDisponivel()) {
        return false;
    }

    // Caracteristicas da imagem
    int32_t largura;
    int32_t altura;
    uint16_t profundidade;
    uint32_t inicioPixels;

    // Verifica o cabecalho BMP
    if (
        !verificarCabecalhoBMP(
            arquivo,
            largura,
            altura,
            profundidade,
            inicioPixels
        )
    ) {
        return false;
    }

    // Verifica a paleta de cores
    bool corZeroPreta;

    if (
        !verificarPaletaBMP(
            arquivo,
            inicioPixels,
            corZeroPreta
        )
    ) {
        return false;
    }

    // Verifica as dimensoes da imagem
    if (
        largura <= 0 ||
        largura > LARGURA_IMPRESSAO
    ) {
        return false;
    }

    // Calcula o tamanho dos buffers
    uint32_t bytesPorLinha =
        obterBytesLinhaBMP(largura);

    uint32_t bytesImpressao =
        obterBytesImpressaoBMP(largura);

    // Calcula o tamanho maximo dos buffers
    constexpr uint32_t TAMANHO_BUFFER_ORIGINAL =
        ((LARGURA_IMPRESSAO + 31) / 32) * 4;

    constexpr uint32_t TAMANHO_BUFFER_IMPRESSAO =
        (LARGURA_IMPRESSAO + 7) / 8;

    // Verifica os tamanhos calculados
    if (
        bytesPorLinha == 0 ||
        bytesImpressao == 0 ||
        bytesPorLinha > TAMANHO_BUFFER_ORIGINAL ||
        bytesImpressao > TAMANHO_BUFFER_IMPRESSAO
    ) {
        return false;
    }

    // Reserva memoria para uma linha
    uint8_t bufferOriginal[
        TAMANHO_BUFFER_ORIGINAL
    ];

    uint8_t dadosImpressao[
        TAMANHO_BUFFER_IMPRESSAO
    ];

    // Identifica a altura da imagem
    uint32_t alturaImagem = obterAlturaBMP(altura);

    // Verifica os limites do comando raster
    if (
        alturaImagem == 0 ||
        alturaImagem > UINT16_MAX ||
        bytesImpressao > UINT16_MAX
    ) {
        return false;
    }

    // Comando GS v 0
    const uint8_t comando[] = {
        0x1D, 0x76, 0x30, 0x00,
        (uint8_t)(bytesImpressao & 0xFF),
        (uint8_t)((bytesImpressao >> 8) & 0xFF),
        (uint8_t)(alturaImagem & 0xFF),
        (uint8_t)((alturaImagem >> 8) & 0xFF)
    };

    // Envia o comando para a impressora
    if (
        serialImpressora.write(
            comando,
            sizeof(comando)
        ) != sizeof(comando)
    ) {
        return false;
    }

    // Percorre todas as linhas da imagem
    for (
        uint32_t linha = 0;
        linha < alturaImagem;
        linha++
    ) {

        // Le e converte a linha BMP
        if (
            !prepararLinhaBMP(
                arquivo,
                inicioPixels,
                altura,
                (int32_t)linha,
                largura,
                bytesPorLinha,
                corZeroPreta,
                bufferOriginal,
                sizeof(bufferOriginal),
                dadosImpressao,
                sizeof(dadosImpressao)
            )
        ) {
            return false;
        }

        // Envia os pixels para a impressora
        if (
            serialImpressora.write(
                dadosImpressao,
                bytesImpressao
            ) != bytesImpressao
        ) {
            return false;
        }
    }

    // Aguarda o envio dos dados
    serialImpressora.flush();

    // Solicita o corte parcial
    if (!cortarPapel()) {
        return false;
    }

    return true;
}