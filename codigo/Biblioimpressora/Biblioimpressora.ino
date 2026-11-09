#include <Arduino.h>

#include "configuracoes.h"
#include "categorias.h"
#include "botoes.h"
#include "leds.h"
#include "microsd.h"
#include "sorteio.h"
#include "imagem.h"
#include "impressora.h"
#include "relogio.h"
#include "registro.h"


// =====================================
// DISPONIBILIDADE DOS MODULOS
// =====================================

static bool cartaoMicroSDAvailavel = false;
static bool relogioDisponivel = false;
static bool registroDisponivel = false;
static bool impressoraDisponivelNoMomento = false;


// =====================================
// INTERVALOS DE RECUPERACAO
// =====================================

static unsigned long ultimaTentativaMicroSD = 0;
static unsigned long ultimaTentativaImpressora = 0;

constexpr unsigned long INTERVALO_TENTATIVA_MS = 3000;


// =====================================
// IDENTIFICAR A PASTA DA CATEGORIA
// =====================================

const char* obterCaminhoCategoria(Categoria categoria) {

    switch (categoria) {

        case Categoria::POESIA:
            return "/poesias";

        case Categoria::CHARGE:
            return "/charges";

        case Categoria::COLORIR:
            return "/desenhos";

        case Categoria::FRASE:
            return "/frases";

        default:
            return nullptr;
    }
}


// =====================================
// REGISTRAR RESULTADO NO CSV
// =====================================

void registrarResultado(
    Categoria categoria,
    const char* nomeArquivo,
    ResultadoOperacao resultado
) {

    if (!registroDisponivel || !relogioDisponivel) {
        return;
    }

    DataHora agora;

    if (!obterDataHora(agora)) {

        Serial.println(
            "ERRO: nao foi possivel obter a data e hora."
        );

        return;
    }

    bool registrado = registrarOperacao(
        agora,
        categoria,
        nomeArquivo,
        resultado
    );

    if (registrado) {

        Serial.println("Operacao registrada no CSV!");

    } else {

        Serial.println(
            "ERRO: nao foi possivel registrar a operacao."
        );
    }
}


// =====================================
// FINALIZAR OPERACAO
// =====================================

void finalizarOperacao() {

    definirEstadoTerminal(EstadoTerminal::PRONTO);

    Serial.println(
        "Terminal pronto para nova solicitacao."
    );
}


// =====================================
// CONFIGURACAO INICIAL
// =====================================

void setup() {

    Serial.begin(VELOCIDADE_MONITOR_SERIAL);

    inicializarSorteio();
    inicializarLed();
    inicializarBotoes();


    // Inicializa o microSD
    cartaoMicroSDAvailavel = inicializarMicroSD();

    if (cartaoMicroSDAvailavel) {

        Serial.println("Cartao microSD inicializado!");

    } else {

        Serial.println(
            "ERRO: nao foi possivel inicializar o microSD."
        );

        iniciarSinalizacaoMicroSD();
    }


    // Inicializa o relogio RTC
    relogioDisponivel = inicializarRelogio();

    if (relogioDisponivel) {

        Serial.println("Relogio RTC inicializado!");

        DataHora agora;

        if (obterDataHora(agora)) {

            Serial.print("Data: ");
            Serial.print(agora.dia);
            Serial.print("/");
            Serial.print(agora.mes);
            Serial.print("/");
            Serial.println(agora.ano);

            Serial.print("Hora: ");
            Serial.print(agora.hora);
            Serial.print(":");
            Serial.print(agora.minuto);
            Serial.print(":");
            Serial.println(agora.segundo);
        }

    } else {

        Serial.println(
            "ERRO: nao foi possivel inicializar o RTC."
        );
    }


    // Inicializa o arquivo de registro
    if (cartaoMicroSDAvailavel) {

        registroDisponivel = inicializarRegistro();

        if (registroDisponivel) {

            Serial.println("Arquivo de registro pronto!");

        } else {

            Serial.println(
                "ERRO: nao foi possivel criar o registro."
            );
        }
    }


    // Inicializa a impressora
    inicializarImpressora();

    impressoraDisponivelNoMomento = impressoraDisponivel();

    if (impressoraDisponivelNoMomento) {

        Serial.println("Impressora disponivel!");

    } else {

        Serial.println(
            "ERRO: impressora indisponivel ou sem papel."
        );

        iniciarSinalizacaoImpressora();

        ultimaTentativaImpressora = millis();
    }


    definirEstadoTerminal(EstadoTerminal::PRONTO);

    Serial.println();
    Serial.println("Biblioimpressora iniciada!");
    Serial.println("Terminal pronto para receber solicitacoes.");
}


// =====================================
// FUNCIONAMENTO CONTINUO
// =====================================

void loop() {

    atualizarSinalizacao();


    // =================================
    // RECUPERACAO DO MICROSD
    // =================================

    if (!cartaoMicroSDAvailavel) {

        if (
            millis() - ultimaTentativaMicroSD >=
            INTERVALO_TENTATIVA_MS
        ) {

            ultimaTentativaMicroSD = millis();

            Serial.println(
                "Tentando recuperar o microSD..."
            );

            cartaoMicroSDAvailavel = inicializarMicroSD();

            if (cartaoMicroSDAvailavel) {

                Serial.println("MicroSD recuperado!");

                registroDisponivel = inicializarRegistro();

                if (registroDisponivel) {

                    Serial.println(
                        "Arquivo de registro pronto!"
                    );

                } else {

                    Serial.println(
                        "ERRO: nao foi possivel criar o registro."
                    );
                }

                pararSinalizacaoMicroSD();
            }
        }

        return;
    }


    // =================================
    // RECUPERACAO DA IMPRESSORA
    // =================================

    if (!impressoraDisponivelNoMomento) {

        if (
            millis() - ultimaTentativaImpressora >=
            INTERVALO_TENTATIVA_MS
        ) {

            ultimaTentativaImpressora = millis();

            Serial.println(
                "Verificando disponibilidade da impressora..."
            );

            impressoraDisponivelNoMomento =
                impressoraDisponivel();

            if (impressoraDisponivelNoMomento) {

                Serial.println("Impressora recuperada!");

                pararSinalizacaoImpressora();
            }
        }

        return;
    }


    // =================================
    // VERIFICAR BOTOES
    // =================================

    Categoria categoriaPressionada;

    if (!lerBotao(categoriaPressionada)) {
        return;
    }

    definirEstadoTerminal(EstadoTerminal::OCUPADO);

    Serial.println();
    Serial.println("Solicitacao recebida!");


    // =================================
    // IDENTIFICAR CATEGORIA
    // =================================

    const char* caminho =
        obterCaminhoCategoria(categoriaPressionada);

    if (caminho == nullptr) {

        Serial.println("ERRO: categoria invalida.");

        registrarResultado(
            categoriaPressionada,
            "Categoria invalida",
            ResultadoOperacao::FALHA
        );

        finalizarOperacao();
        return;
    }


    // =================================
    // SORTEAR ARQUIVO
    // =================================

    File arquivo = sortearArquivo(
        caminho,
        categoriaPressionada
    );

    if (!arquivo) {

        Serial.println(
            "ERRO: nenhuma imagem encontrada."
        );

        registrarResultado(
            categoriaPressionada,
            "Nenhum arquivo encontrado",
            ResultadoOperacao::FALHA
        );

        finalizarOperacao();
        return;
    }

    String nomeArquivo = arquivo.name();


    // =================================
    // VALIDAR IMAGEM BMP
    // =================================

    int32_t largura;
    int32_t altura;
    uint16_t profundidade;
    uint32_t inicioPixels;
    bool corZeroPreta;

    bool imagemValida =
        verificarCabecalhoBMP(
            arquivo,
            largura,
            altura,
            profundidade,
            inicioPixels
        ) &&
        verificarPaletaBMP(
            arquivo,
            inicioPixels,
            corZeroPreta
        );

    if (!imagemValida) {

        Serial.println("ERRO: imagem BMP invalida.");

        registrarResultado(
            categoriaPressionada,
            nomeArquivo.c_str(),
            ResultadoOperacao::FALHA
        );

        arquivo.close();

        finalizarOperacao();
        return;
    }


    // =================================
    // INFORMACOES DA IMAGEM
    // =================================

    Serial.println("Imagem selecionada!");

    Serial.print("Arquivo: ");
    Serial.println(nomeArquivo);

    Serial.print("Largura: ");
    Serial.println(largura);

    Serial.print("Altura: ");
    Serial.println(obterAlturaBMP(altura));


    // =================================
    // IMPRIMIR IMAGEM
    // =================================

    Serial.println("Iniciando impressao...");

    bool impressaoSucesso = imprimirImagemBMP(arquivo);

    arquivo.close();


    // =================================
    // REGISTRAR RESULTADO
    // =================================

    if (impressaoSucesso) {

        confirmarSorteio(categoriaPressionada);

        Serial.println(
            "Dados enviados para a impressora!"
        );

        registrarResultado(
            categoriaPressionada,
            nomeArquivo.c_str(),
            ResultadoOperacao::SUCESSO
        );

    } else {

        Serial.println("ERRO durante a impressao.");

        registrarResultado(
            categoriaPressionada,
            nomeArquivo.c_str(),
            ResultadoOperacao::FALHA
        );

        // Verifica se a falha foi causada pela impressora
        impressoraDisponivelNoMomento = impressoraDisponivel();

        if (!impressoraDisponivelNoMomento) {

            iniciarSinalizacaoImpressora();

            ultimaTentativaImpressora = millis();

            Serial.println(
                "Impressora indisponivel. Aguardando recuperacao."
            );
        }
    }


    // =================================
    // FINALIZAR
    // =================================

    finalizarOperacao();
}