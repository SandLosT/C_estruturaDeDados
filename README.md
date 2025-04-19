# Analisador de Registros de Processos

Este projeto em C realiza a leitura, análise e exportação de dados a partir de um arquivo CSV com registros de processos judiciais.

## Funcionalidades

- Leitura de registros a partir de um CSV (`processos.csv`);
- Ordenação por identificador (`Id`) e por data de ajuizamento (`DataAjuizamento`);
- Busca por um identificador de classe específico;
- Contagem de assuntos únicos presentes nos registros;
- Identificação de processos com múltiplos assuntos;
- Cálculo do número de dias desde a abertura de cada processo;
- Exportação dos dados ordenados para novos arquivos CSV (`ordenado_id.csv` e `ordenado_data.csv`).

## Estrutura do Projeto

- `main.c`: Arquivo principal que executa as operações com os registros.
- `dados.c`: Implementação das funções de manipulação, análise e exportação de dados.
- `dados.h`: Definições de estruturas e protótipos de funções.
- `processos.csv`: Arquivo de entrada com os dados dos processos (não incluído).

## Compilação

Utilize um compilador como `gcc` para compilar:

```bash
gcc main.c dados.c -o analisador
