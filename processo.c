#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "processo.h"

Registro* CarregarCSV(const char *caminho, int total) {
    FILE *fp = fopen(caminho, "r");
    if (!fp) {
        printf("Falha ao abrir o arquivo.\n");
        return NULL;
    }

    Registro *R = malloc(total * sizeof(Registro));
    if (!R) {
        printf("Memória insuficiente.\n");
        fclose(fp);
        return NULL;
    }

    char cabecalho[256];
    fgets(cabecalho, sizeof(cabecalho), fp);

    char linha[1024];
    int i = 0;

    while (i < total && fgets(linha, sizeof(linha), fp)) {
        memset(&R[i], 0, sizeof(Registro));
        R[i].QtdIdAssunto = 0;

        char *p = linha;
        int campo = 0;
        int em_lista = 0;
        char buffer[256];
        int bidx = 0;

        while (*p && campo <= 5) {
            if (*p == '{') {
                em_lista = 1;
                p++;
                continue;
            } else if (*p == '}') {
                em_lista = 0;
                p++;
                continue;
            }

            if ((*p == ',' && !em_lista) || *p == '\n') {
                buffer[bidx] = '\0';

                switch (campo) {
                    case 0: R[i].Id = atoi(buffer); break;
                    case 1: strncpy(R[i].Numero, buffer, sizeof(R[i].Numero) - 1); break;
                    case 2: strncpy(R[i].DataAjuizamento, buffer, sizeof(R[i].DataAjuizamento) - 1); break;
                    case 3: {
                        int k = 0;
                        char *tok = strtok(buffer, ",");
                        while (tok && k < MAX_IDS) {
                            R[i].IdClasse[k++] = atoi(tok);
                            tok = strtok(NULL, ",");
                        }
                        break;
                    }
                    case 4: {
                        int k = 0;
                        char *tok = strtok(buffer, ",");
                        while (tok && k < MAX_IDS) {
                            R[i].IdAssunto[k++] = atoi(tok);
                            tok = strtok(NULL, ",");
                        }
                        R[i].QtdIdAssunto = k;
                        break;
                    }
                    case 5: R[i].AnoEleicao = atoi(buffer); break;
                }

                campo++;
                bidx = 0;
                p++;
            } else {
                if (*p != '"' && *p != ' ')
                    buffer[bidx++] = *p;
                p++;
            }
        }

        i++;
    }

    fclose(fp);
    return R;
}

void OrdenarPorIdentificador(Registro *V, int T) {
    for (int i = 0; i < T - 1; i++) {
        int menor = i;
        for (int j = i + 1; j < T; j++) {
            if (V[j].Id < V[menor].Id)
                menor = j;
        }
        if (menor != i) {
            Registro tmp = V[i];
            V[i] = V[menor];
            V[menor] = tmp;
        }
    }
}

void OrdenarPorAbertura(Registro *V, int T) {
    for (int i = 0; i < T - 1; i++) {
        int recente = i;
        for (int j = i + 1; j < T; j++) {
            if (strcmp(V[j].DataAjuizamento, V[recente].DataAjuizamento) > 0)
                recente = j;
        }
        if (recente != i) {
            Registro tmp = V[i];
            V[i] = V[recente];
            V[recente] = tmp;
        }
    }
}

int BuscarPorClasse(Registro *V, int T, int classe) {
    int cont = 0;
    for (int i = 0; i < T; i++) {
        for (int j = 0; j < MAX_IDS; j++) {
            if (V[i].IdClasse[j] == classe) {
                cont++;
                break;
            }
        }
    }
    return cont;
}

int VerificaSeExiste(int *vetor, int tam, int val) {
    for (int i = 0; i < tam; i++)
        if (vetor[i] == val) return 1;
    return 0;
}

int ContarAssuntosUnicos(Registro *V, int T) {
    int encontrados[QNT_LINHAS * MAX_IDS];
    int total = 0;

    for (int i = 0; i < T; i++) {
        for (int j = 0; j < MAX_IDS; j++) {
            int id = V[i].IdAssunto[j];
            if (id != 0 && !VerificaSeExiste(encontrados, total, id)) {
                encontrados[total++] = id;
            }
        }
    }

    return total;
}

void MostrarDiversosAssuntos(Registro *V, int T) {
    printf("Registros com mais de um código de assunto:\n");
    for (int i = 0; i < T; i++) {
        int qtd = 0;
        for (int j = 0; j < MAX_IDS; j++) {
            if (V[i].IdAssunto[j] != 0) qtd++;
        }

        if (qtd > 1) {
            printf("ID: %d | Num.: %s | Assuntos: ", V[i].Id, V[i].Numero);
            for (int j = 0; j < MAX_IDS; j++) {
                if (V[i].IdAssunto[j] != 0) {
                    printf("%d ", V[i].IdAssunto[j]);
                }
            }
            printf("\n");
        }
    }
}

int DiasDesdeInicio(char *dataStr) {
    struct tm data = {0};
    char tmp[11];
    strncpy(tmp, dataStr, 10);
    tmp[10] = '\0';

    sscanf(tmp, "%d-%d-%d", &data.tm_year, &data.tm_mon, &data.tm_mday);
    data.tm_year -= 1900;
    data.tm_mon -= 1;

    time_t inicio = mktime(&data);
    if (inicio == -1) return -1;

    time_t agora = time(NULL);
    double segundos = difftime(agora, inicio);
    return (int)(segundos / (60 * 60 * 24));
}

void ExportarCSV(const char *arquivo, Registro *V, int T) {
    FILE *fp = fopen(arquivo, "w");
    if (!fp) {
        printf("Erro ao salvar %s\n", arquivo);
        return;
    }

    fprintf(fp, "Id,Numero,DataAjuizamento,IdClasse,IdAssunto,AnoEleicao\n");

    for (int i = 0; i < T; i++) {
        fprintf(fp, "%d,%s,%s,", V[i].Id, V[i].Numero, V[i].DataAjuizamento);

        fprintf(fp, "{");
        for (int j = 0; j < MAX_IDS && V[i].IdClasse[j] != 0; j++) {
            fprintf(fp, "%d", V[i].IdClasse[j]);
            if (j + 1 < MAX_IDS && V[i].IdClasse[j + 1] != 0)
                fprintf(fp, ",");
        }
        fprintf(fp, "},");

        fprintf(fp, "{");
        for (int j = 0; j < MAX_IDS && V[i].IdAssunto[j] != 0; j++) {
            fprintf(fp, "%d", V[i].IdAssunto[j]);
            if (j + 1 < MAX_IDS && V[i].IdAssunto[j + 1] != 0)
                fprintf(fp, ",");
        }
        fprintf(fp, "},%d\n", V[i].AnoEleicao);
    }

    fclose(fp);
}
