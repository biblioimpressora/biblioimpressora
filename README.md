# Biblioimpressora

**Projeto de extensão | IFSul Câmpus Camaquã**

A Biblioimpressora é um projeto de extensão que integra tecnologia, literatura e cultura local por meio da proposta de um terminal interativo para disponibilização gratuita de conteúdos impressos em papel térmico.

O projeto é desenvolvido pelo IFSul Câmpus Camaquã, em parceria com a Biblioteca Municipal de Camaquã e a Casa do Poeta Camaquense, com o propósito de aproximar a comunidade da produção cultural local e explorar possibilidades de reaproveitamento de equipamentos eletrônicos.

## Sobre o projeto

A proposta consiste no desenvolvimento de um terminal interativo no qual as pessoas poderão selecionar categorias de conteúdo e receber impressões gratuitas de poesias, charges e desenhos para colorir.

Além da valorização da literatura e dos artistas locais, a iniciativa busca promover o acesso à cultura, incentivar o uso consciente de recursos e explorar a reutilização de equipamentos eletrônicos que seriam descartados.

O desenvolvimento envolve programação embarcada, interação por botões, comunicação com impressora térmica e organização dos conteúdos digitais, considerando também a documentação técnica e o acompanhamento do funcionamento do sistema.

## Objetivos

- Incentivar a leitura e o contato com a produção literária local.
- Valorizar autores, artistas e manifestações culturais de Camaquã.
- Ampliar o acesso gratuito a conteúdos culturais e educativos.
- Explorar o reaproveitamento de equipamentos eletrônicos.
- Integrar conhecimentos de tecnologia, educação e cultura em uma iniciativa de extensão universitária.

## Estrutura do repositório

O repositório está organizado para reunir o código-fonte, os conteúdos destinados à impressão, a documentação técnica e os registros visuais do projeto.

| Diretório | Finalidade |
|---|---|
| `codigo/` | Organização do código-fonte do terminal. |
| `conteudo/poesias/` | Armazenamento de poesias autorizadas para disponibilização. |
| `conteudo/charges/` | Armazenamento de charges autorizadas para disponibilização. |
| `conteudo/desenhos/` | Organização de desenhos destinados à impressão e à pintura. |
| `documentacao/` | Reunião de documentos técnicos, instruções e referências. |
| `imagens/` | Armazenamento de fotografias e imagens relacionadas ao projeto. |

Os diretórios podem ser preenchidos e atualizados conforme o desenvolvimento avança e os materiais são preparados para integração ao sistema.

## Tecnologias e ferramentas

O desenvolvimento envolve ferramentas de programação, simulação e documentação técnica. Entre as ferramentas utilizadas nas etapas iniciais estão:

- **Arduino IDE:** ambiente utilizado para o desenvolvimento de código destinado à plataforma ESP32.
- **ESP32:** microcontrolador previsto para o controle das funções do terminal.
- **Wokwi:** plataforma utilizada para simulação inicial de circuitos e testes da lógica de interação.
- **Git e GitHub:** ferramentas utilizadas para versionamento do código, organização dos arquivos e documentação colaborativa.

As ferramentas, os componentes e os procedimentos poderão ser atualizados conforme as definições técnicas e a integração dos equipamentos forem avançando.

## Desenvolvimento atual

O projeto encontra-se em fase de desenvolvimento. As atividades iniciais incluem a organização do repositório, o estudo dos componentes e das referências técnicas, o planejamento do funcionamento do sistema e a simulação de circuitos e interações básicas.

A integração com a impressora térmica e os demais componentes físicos dependerá das definições de hardware e da realização dos testes correspondentes.

## Manuais de referência

Esta seção reúne materiais técnicos consultados para apoiar o estudo do funcionamento de impressoras térmicas e orientar as etapas de desenvolvimento. Os documentos devem ser interpretados de acordo com o modelo e os recursos do equipamento efetivamente utilizado.

### Jetway JP-800

- [Manual de Comandos da JP-800 (PDF)](https://www.jetway.com.br/wp-content/uploads/2019/11/manual_de_comandos_Impressora_Jetway_JP-800.pdf) — referência para consulta dos comandos da impressora.
- [Manual do Utilitário da JP-800 (PDF)](https://www.jetway.com.br/wp-content/uploads/2019/11/Manual_utilitario_impressora_Jetway_JP-800.pdf) — documentação do utilitário associado ao equipamento.
- [Página de produto e materiais da Jetway — JP-800](https://www.jetway.com.br/produtos/drivers-produto/?produto=213&produto-jetway=JP-800) — página de referência em que os manuais foram localizados.

### Epson TM-T88V

- [Manual de referência da Epson TM-T88V (PDF)](https://files.support.epson.com/pdf/pos/bulk/tmt88v_trg_reve.pdf) — documentação complementar para consulta sobre outro modelo de impressora térmica.

**Nota:** a inclusão desses materiais não significa que todos os comandos, recursos ou procedimentos sejam compatíveis com o equipamento que será utilizado na Biblioimpressora.

## Conteúdos e direitos autorais

Os materiais destinados à impressão devem respeitar os direitos autorais e as condições de uso aplicáveis. Serão priorizados conteúdos próprios, autorizados pelos respectivos autores ou disponibilizados em domínio público, conforme as condições legais pertinentes.

## Licença

A definição da licença do software está pendente de alinhamento com a coordenação do projeto. Até essa definição, este README não estabelece uma licença definitiva para o código-fonte.

## Participantes e parcerias

Projeto de extensão do **Instituto Federal de Educação, Ciência e Tecnologia Sul-rio-grandense (IFSul), Câmpus Camaquã**, desenvolvido em parceria com a **Biblioteca Municipal de Camaquã** e a **Casa do Poeta Camaquense**.
