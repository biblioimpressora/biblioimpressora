#include "botoes.h"
#include "configuracoes.h"


// Quantidade de botoes do terminal
constexpr uint8_t QUANTIDADE_BOTOES = 4;


// Inicializa os quatro botoes
void inicializarBotoes() {

    pinMode(PIN_BOTAO_POESIA, INPUT_PULLUP);
    pinMode(PIN_BOTAO_CHARGE, INPUT_PULLUP);
    pinMode(PIN_BOTAO_COLORIR, INPUT_PULLUP);
    pinMode(PIN_BOTAO_FRASE, INPUT_PULLUP);

}


// Verifica se algum botao foi pressionado
bool lerBotao(Categoria &categoriaPressionada) {

    // Pinos dos quatro botoes
    const uint8_t pinos[QUANTIDADE_BOTOES] = {
        PIN_BOTAO_POESIA,
        PIN_BOTAO_CHARGE,
        PIN_BOTAO_COLORIR,
        PIN_BOTAO_FRASE
    };


    // Categorias correspondentes aos botoes
    const Categoria categorias[QUANTIDADE_BOTOES] = {
        Categoria::POESIA,
        Categoria::CHARGE,
        Categoria::COLORIR,
        Categoria::FRASE
    };


    // Estados dos botoes
    static bool ultimaLeitura[QUANTIDADE_BOTOES] = {
        HIGH, HIGH, HIGH, HIGH
    };

    static bool estadoEstavel[QUANTIDADE_BOTOES] = {
        HIGH, HIGH, HIGH, HIGH
    };


    // Momento da ultima mudanca de cada botao
    static unsigned long ultimoTempoMudanca[QUANTIDADE_BOTOES] = {
        0, 0, 0, 0
    };


    // Impede novas solicitacoes ate todos os botoes serem soltos
    static bool bloqueadoAteSoltar = false;


    // Momento atual
    unsigned long agora = millis();


    // =====================================
    // 1. ATUALIZAR O DEBOUNCE DOS BOTOES
    // =====================================

    for (uint8_t i = 0; i < QUANTIDADE_BOTOES; i++) {

        bool leituraAtual = digitalRead(pinos[i]);

        // Detecta uma mudanca na leitura
        if (leituraAtual != ultimaLeitura[i]) {

            ultimoTempoMudanca[i] = agora;

            ultimaLeitura[i] = leituraAtual;

        }

        // Confirma a mudanca depois do debounce
        if (agora - ultimoTempoMudanca[i] >= TEMPO_DEBOUNCE_MS) {

            estadoEstavel[i] = ultimaLeitura[i];

        }

    }


    // =====================================
    // 2. VERIFICAR O BLOQUEIO DOS BOTOES
    // =====================================

    if (bloqueadoAteSoltar) {

        bool todosSoltos = true;

        for (uint8_t i = 0; i < QUANTIDADE_BOTOES; i++) {

            if (estadoEstavel[i] == LOW) {

                todosSoltos = false;

            }

        }

        // Libera os botoes quando todos estiverem soltos
        if (todosSoltos) {

            bloqueadoAteSoltar = false;

        }

        return false;

    }


    // =====================================
    // 3. IDENTIFICAR UM BOTAO PRESSIONADO
    // =====================================

    for (uint8_t i = 0; i < QUANTIDADE_BOTOES; i++) {

        if (estadoEstavel[i] == LOW) {

            categoriaPressionada = categorias[i];

            bloqueadoAteSoltar = true;

            return true;

        }

    }


    // Nenhum botao foi pressionado
    return false;

}
