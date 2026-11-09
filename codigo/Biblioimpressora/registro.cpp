
#include <Arduino.h>
#include <SD.h>

#include "registro.h"


// =====================================
// ARQUIVO DE REGISTRO
// =====================================

constexpr const char* ARQUIVO_REGISTRO = "/registro.csv";


// =====================================
// IDENTIFICAR NOME DA CATEGORIA
// =====================================

static const char* obterNomeCategoria(Categoria categoria) {

    switch (categoria) {

        case Categoria::POESIA:
            return "Poesia";

        case Categoria::CHARGE:
            return "Charge";

        case Categoria::COLORIR:
            return "Colorir";

        case Categoria::FRASE:
            return "Frase";

        default:
            return "Desconhecida";
    }
}


// =====================================
// IDENTIFICAR RESULTADO
// =====================================

static const char* obterNomeResultado(
    ResultadoOperacao resultado
) {

    switch (resultado) {

        case ResultadoOperacao::SELECAO:
            return "Selecao";

        case ResultadoOperacao::SUCESSO:
            return "Sucesso";

        case ResultadoOperacao::FALHA:
            return "Falha";

        default:
            return "Desconhecido";
    }
}


// =====================================
// INICIALIZAR REGISTRO
// =====================================

bool inicializarRegistro() {

    if (!SD.exists(ARQUIVO_REGISTRO)) {

        File arquivo = SD.open(
            ARQUIVO_REGISTRO,
            FILE_WRITE
        );

        if (!arquivo) {
            return false;
        }

        arquivo.println(
            "Data,Hora,Categoria,Arquivo,Resultado"
        );

        bool sucesso = arquivo.getWriteError() == 0;

        arquivo.close();

        if (!sucesso) {
            return false;
        }
    }

    return true;
}


// =====================================
// REGISTRAR OPERACAO
// =====================================

bool registrarOperacao(
    const DataHora &dataHora,
    Categoria categoria,
    const char* nomeArquivo,
    ResultadoOperacao resultado
) {

    if (nomeArquivo == nullptr) {
        return false;
    }

    File arquivo = SD.open(
        ARQUIVO_REGISTRO,
        FILE_APPEND
    );

    if (!arquivo) {
        return false;
    }


    // Data
    if (dataHora.dia < 10) {
        arquivo.print("0");
    }

    arquivo.print(dataHora.dia);
    arquivo.print("/");

    if (dataHora.mes < 10) {
        arquivo.print("0");
    }

    arquivo.print(dataHora.mes);
    arquivo.print("/");
    arquivo.print(dataHora.ano);
    arquivo.print(",");


    // Hora
    if (dataHora.hora < 10) {
        arquivo.print("0");
    }

    arquivo.print(dataHora.hora);
    arquivo.print(":");

    if (dataHora.minuto < 10) {
        arquivo.print("0");
    }

    arquivo.print(dataHora.minuto);
    arquivo.print(":");

    if (dataHora.segundo < 10) {
        arquivo.print("0");
    }

    arquivo.print(dataHora.segundo);
    arquivo.print(",");


    // Categoria
    arquivo.print(obterNomeCategoria(categoria));
    arquivo.print(",");


    // Nome do arquivo protegido por aspas
    arquivo.print("\"");
    arquivo.print(nomeArquivo);
    arquivo.print("\",");


    // Resultado
    arquivo.println(obterNomeResultado(resultado));


    bool sucesso = arquivo.getWriteError() == 0;

    arquivo.close();

    return sucesso;
}
