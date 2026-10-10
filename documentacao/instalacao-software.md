# Instalação do software

## 1. Objetivo

Este documento registra as ferramentas de software instaladas para o desenvolvimento do projeto Biblioimpressora, incluindo suas versões, funções e links oficiais.

## 2. Ferramentas instaladas

### 2.1. Arduino IDE

* **Versão:** 2.3.10
* **Função:** ambiente de desenvolvimento utilizado para escrever, compilar e enviar programas para placas compatíveis.
* **Site oficial:** https://www.arduino.cc/en/software/

### 2.2. Visual Studio Code (VS Code)

* **Versão:** 1.140.0
* **Sistema operacional:** Windows 64 bits
* **Função:** editor de código que poderá ser utilizado para editar os arquivos do projeto.
* **Site oficial:** https://code.visualstudio.com/

### 2.3. Git

* **Versão:** 2.55.0.windows.5
* **Função:** sistema de controle de versões utilizado para registrar e acompanhar alterações nos arquivos do projeto.
* **Site oficial:** https://git-scm.com/

A instalação foi verificada pelo Prompt de Comando do Windows, utilizando o comando `git --version`.

### 2.4. Pacote de placas ESP32

* **Nome:** esp32 by Espressif Systems
* **Versão:** 3.3.12
* **Ambiente:** Arduino IDE
* **Função:** disponibilizar suporte às placas ESP32 no ambiente Arduino.
* **Documentação oficial:** https://docs.espressif.com/projects/arduino-esp32/en/latest/

## 3. Criação do repositório

O repositório do projeto foi criado no GitHub, dentro da organização `biblioimpressora`, conforme a orientação da coordenação.

Foram criadas as seguintes pastas para organizar os arquivos:

* `codigo/` — código-fonte do terminal.
* `conteudo/poesias/` — poesias.
* `conteudo/charges/` — charges.
* `conteudo/desenhos/` — desenhos para impressão.
* `documentacao/` — documentos e manuais.
* `imagens/` — imagens e registros do projeto.

Também foi criado um arquivo `README.md` com a apresentação inicial, os objetivos e a estrutura do projeto.

## 4. Observações

As ferramentas e versões foram registradas com base nas informações disponíveis no computador durante a preparação do ambiente de desenvolvimento.

A configuração específica da placa ESP32 e a integração com a impressora térmica serão documentadas após a confirmação dos componentes e das conexões utilizadas no projeto.
