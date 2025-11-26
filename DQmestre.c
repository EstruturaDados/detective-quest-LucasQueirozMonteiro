#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// ========================================
//  ESTRUTURA DAS SALAS (ÁRVORE BINÁRIA)
// ========================================
typedef struct Sala {
    char nome[50];
    char pista[50];  
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(const char* nome, const char* pista) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    snprintf(nova->nome, sizeof(nova->nome), "%s", nome);

    if (pista != NULL)
        snprintf(nova->pista, sizeof(nova->pista), "%s", pista);
    else
        nova->pista[0] = '\0';

    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

//
// ========================================
//  BST PARA PISTAS
// ========================================
typedef struct NodoBST {
    char pista[50];
    struct NodoBST* esquerda;
    struct NodoBST* direita;
} NodoBST;

NodoBST* inserirBST(NodoBST* raiz, const char* pista) {
    if (raiz == NULL) {
        NodoBST* novo = (NodoBST*) malloc(sizeof(NodoBST));
        snprintf(novo->pista, sizeof(novo->pista), "%s", pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0)
        raiz->esquerda = inserirBST(raiz->esquerda, pista);
    else
        raiz->direita = inserirBST(raiz->direita, pista);

    return raiz;
}

void emOrdemBST(NodoBST* raiz) {
    if (raiz == NULL) return;
    emOrdemBST(raiz->esquerda);
    printf("🔎 %s\n", raiz->pista);
    emOrdemBST(raiz->direita);
}

//
// ========================================
//  TABELA HASH PARA RELACIONAR PISTA → SUSPEITO
// ========================================
#define TAM 26  // índice baseado na primeira letra da pista

typedef struct NodoHash {
    char pista[50];
    char suspeito[50];
    struct NodoHash* prox;
} NodoHash;

NodoHash* tabela[TAM];

// Função hash simples baseada no primeiro caractere
int hash(const char* pista) {
    char c = pista[0];
    if (c >= 'A' && c <= 'Z') c += 32; 
    return (c - 'a') % TAM;
}

void inserirHash(const char* pista, const char* suspeito) {
    int h = hash(pista);

    NodoHash* novo = (NodoHash*) malloc(sizeof(NodoHash));
    snprintf(novo->pista, sizeof(novo->pista), "%s", pista);
    snprintf(novo->suspeito, sizeof(novo->suspeito), "%s", suspeito);
    novo->prox = tabela[h];
    tabela[h] = novo;
}

void listarHash() {
    printf("\n🔍 Relação PISTA → SUSPEITO:\n");

    for (int i = 0; i < TAM; i++) {
        NodoHash* atual = tabela[i];
        while (atual != NULL) {
            printf("   🧩 \"%s\" → 👤 %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

//
// Determinar o suspeito mais citado
//
char* suspeitoMaisCitado() {
    typedef struct Cont {
        char nome[50];
        int qtd;
    } Cont;

    Cont lista[20];
    int total = 0;

    // varrer tabela hash
    for (int i = 0; i < TAM; i++) {
        NodoHash* atual = tabela[i];
        while (atual != NULL) {

            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(lista[j].nome, atual->suspeito) == 0) {
                    lista[j].qtd++;
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                snprintf(lista[total].nome, 50, "%s", atual->suspeito);
                lista[total].qtd = 1;
                total++;
            }

            atual = atual->prox;
        }
    }

    // descobrir o maior
    int maior = -1;
    int indice = -1;
    for (int i = 0; i < total; i++) {
        if (lista[i].qtd > maior) {
            maior = lista[i].qtd;
            indice = i;
        }
    }

    if (indice == -1) return NULL;

    // CORREÇÃO: variável estática garante ponteiro válido
    static char resultado[50];
    strcpy(resultado, lista[indice].nome);
    return resultado;
}

//
// ========================================
//  EXPLORAÇÃO DA MANSÃO (coleta pistas + hash)
// ========================================
void explorarSalas(Sala* atual, NodoBST** arvBST) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Você está em: **%s**\n", atual->nome);

        // Se a sala contém pista → adicionar na BST e HASH
        if (strlen(atual->pista) > 0) {
            printf("🧩 Você encontrou uma pista: \"%s\"!\n", atual->pista);

            *arvBST = inserirBST(*arvBST, atual->pista);

            // ASSOCIA PISTA → SUSPEITO
            if (strstr(atual->pista, "Chave")) inserirHash(atual->pista, "Mordomo");
            else if (strstr(atual->pista, "Mapa")) inserirHash(atual->pista, "Jardineiro");
            else if (strstr(atual->pista, "Partitura")) inserirHash(atual->pista, "Maestro");
            else if (strstr(atual->pista, "Página")) inserirHash(atual->pista, "Bibliotecária");
            else inserirHash(atual->pista, "Desconhecido");
        }

        // Nó-folha → fim do caminho
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🔚 Fim do caminho!\n");
            return;
        }

        printf("\nPara onde deseja ir?\n");
        if (atual->esquerda) printf("  (e) Esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita)  printf("  (d) Direita  → %s\n", atual->direita->nome);
        printf("  (p) Ver pistas coletadas\n");
        printf("  (h) Ver suspeitos relacionados às pistas\n");
        printf("  (s) Sair\n");
        printf("Escolha: ");
        scanf(" %c", &opcao);

        switch (opcao) {
            case 'e': atual = atual->esquerda; break;
            case 'd': atual = atual->direita;  break;

            case 'p':
                printf("\n📜 PISTAS (ordem alfabética):\n");
                emOrdemBST(*arvBST);
                break;

            case 'h':
                listarHash();
                break;

            case 's':
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}

//
// ========================================
//  MAIN — MONTAGEM DO MAPA
// ========================================
int main() {

    // Zerar tabela hash
    for (int i = 0; i < TAM; i++) tabela[i] = NULL;

    // Montagem da mansão
    Sala* hall = criarSala("Hall de Entrada", NULL);
    hall->esquerda = criarSala("Sala de Estar", "Chave Dourada");
    hall->direita  = criarSala("Corredor Principal", NULL);

    hall->esquerda->esquerda = criarSala("Biblioteca", "Página Rasgada");
    hall->esquerda->direita  = criarSala("Sala de Música", "Partitura Misteriosa");
    hall->direita->esquerda  = criarSala("Cozinha", NULL);
    hall->direita->direita   = criarSala("Jardim Interno", "Mapa Antigo");

    NodoBST* arvorePistas = NULL;

    printf("=== 🏰 NÍVEL MESTRE — Investigação da Mansão ===\n");
    explorarSalas(hall, &arvorePistas);

    printf("\n📚 Todas as pistas coletadas:\n");
    emOrdemBST(arvorePistas);

    printf("\n🕵️ Relação completa pista → suspeito:\n");
    listarHash();

    char* culpado = suspeitoMaisCitado();
    if (culpado != NULL)
        printf("\n⚠️ Suspeito mais associado às pistas: **%s** ⚠️\n", culpado);
    else
        printf("\nNenhuma pista foi encontrada.\n");

    return 0;
}