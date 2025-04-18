#ifndef DADOS_H
#define DADOS_H

#define QNT_LINHAS 1000
#define MAX_IDS 10

typedef struct {
    int Id;
    char Numero[50];
    char DataAjuizamento[20];
    int IdClasse[MAX_IDS];
    int IdAssunto[MAX_IDS];
    int QtdIdAssunto;
    int AnoEleicao;
} Registro;

Registro* CarregarCSV(const char *caminho, int total);
void OrdenarPorIdentificador(Registro *V, int T);
void OrdenarPorAbertura(Registro *V, int T);
int BuscarPorClasse(Registro *V, int T, int classe);
int ContarAssuntosUnicos(Registro *V, int T);
void MostrarDiversosAssuntos(Registro *V, int T);
int DiasDesdeInicio(char *dataStr);
void ExportarCSV(const char *arquivo, Registro *V, int T);

#endif
