#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
        nova->pista[0] = '\0'; // sala sem pista

    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


typedef struct NodoBST {
    char pista[50];
    struct NodoBST* esquerda;
    struct NodoBST* direita;
} NodoBST;

NodoBST* inserir(NodoBST* raiz, const char* pista) {
    if (raiz == NULL) {
        NodoBST* novo = (NodoBST*) malloc(sizeof(NodoBST));
        snprintf(novo->pista, sizeof(novo->pista), "%s", pista);
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }

    if (strcmp(pista, raiz->pista) < 0) {
        raiz->esquerda = inserir(raiz->esquerda, pista);
    } else {
        raiz->direita = inserir(raiz->direita, pista);
    }

    return raiz;
}

void emOrdem(NodoBST* raiz) {
    if (raiz == NULL) return;

    emOrdem(raiz->esquerda);
    printf("🔎 %s\n", raiz->pista);
    emOrdem(raiz->direita);
}


void explorarSalas(Sala* atual, NodoBST** arvorePistas) {
    char opcao;

    while (atual != NULL) {

        printf("\n📍 Você está em: **%s**\n", atual->nome);

       
        if (strlen(atual->pista) > 0) {
            printf("🧩 Você encontrou uma pista: \"%s\"!\n", atual->pista);
            *arvorePistas = inserir(*arvorePistas, atual->pista);
        }

        
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🔚 Você chegou ao fim do caminho!\n");
            return;
        }

        printf("\nPara onde deseja ir?\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda  → %s\n", atual->esquerda->nome);
        if (atual->direita  != NULL) printf("  (d) Ir para a direita   → %s\n", atual->direita->nome);
        printf("  (p) Ver pistas encontradas\n");
        printf("  (s) Sair da exploração\n");
        printf("Escolha: ");

        scanf(" %c", &opcao);

        switch (opcao) {
            case 'e':
                if (atual->esquerda != NULL)
                    atual = atual->esquerda;
                else
                    printf("❌ Caminho inexistente!\n");
                break;

            case 'd':
                if (atual->direita != NULL)
                    atual = atual->direita;
                else
                    printf("❌ Caminho inexistente!\n");
                break;

            case 'p':
                printf("\n📜 PISTAS COLETADAS (em ordem alfabética):\n");
                emOrdem(*arvorePistas);
                break;

            case 's':
                printf("👋 Saindo da exploração...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}


int main() {
    
    Sala* hallEntrada = criarSala("Hall de Entrada", NULL);

    hallEntrada->esquerda = criarSala("Sala de Estar", "Chave Dourada");
    hallEntrada->direita  = criarSala("Corredor Principal", NULL);

    hallEntrada->esquerda->esquerda = criarSala("Biblioteca", "Página Rasgada");
    hallEntrada->esquerda->direita  = criarSala("Sala de Música", "Partitura Misteriosa");

    hallEntrada->direita->esquerda  = criarSala("Cozinha", NULL);
    hallEntrada->direita->direita   = criarSala("Jardim Interno", "Mapa Antigo");

    
    NodoBST* arvorePistas = NULL;

    printf("=== 🏰 Exploração da Mansão — Nível Aventureiro ===\n\n");
    explorarSalas(hallEntrada, &arvorePistas);

    printf("\n📜 Pistas finais coletadas:\n");
    emOrdem(arvorePistas);

    return 0;
}
