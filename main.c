#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "processo.h" 

int main() {
    Registro *registros = CarregarCSV("processos.csv", QNT_LINHAS);
    if (registros == NULL) {
        printf("Erro ao tentar ler os registros.\n");
        return 1;
    }
    printf("Registros importados com êxito.\n");
/*
  
    int chaveClasse = 11531;
    int totalClasse = BuscarPorClasse(registros, QNT_LINHAS, chaveClasse);
    printf("Total com o identificador de classe %d: %d\n", chaveClasse, totalClasse);

    int quantidadeAssuntos = ContarAssuntosUnicos(registros, QNT_LINHAS);
    printf("Quantidade de assuntos diferentes encontrados: %d\n", quantidadeAssuntos);

    MostrarDiversosAssuntos(registros, QNT_LINHAS);


    for (int i = 0; i < QNT_LINHAS; i++) {
        printf("ID: %d -- Num.: %s -- Abertura: %s -- Classe: %d -- Assunto: %d -- Ano: %d\n",
            registros[i].Id,
            registros[i].Numero,
            registros[i].DataAjuizamento,
            registros[i].IdClasse[0],
            registros[i].IdAssunto[0],
            registros[i].AnoEleicao
        );
    }

    for (int i = 0; i < QNT_LINHAS; i++) {
        int duracao = DiasDesdeInicio(registros[i].DataAjuizamento);
        printf("ID: %d -- Inicio: %s -- Em tramitacao: %d\n",
            registros[i].Id,
            registros[i].DataAjuizamento,
            duracao);
    }
*/
    OrdenarPorIdentificador(registros, QNT_LINHAS);
    ExportarCSV("ordenado_id.csv", registros, QNT_LINHAS);

    OrdenarPorAbertura(registros, QNT_LINHAS);
    ExportarCSV("ordenado_data.csv", registros, QNT_LINHAS);

    free(registros);
    return 0;
}
