#include <stdio.h>
#include <stdlib.h>


typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;


Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*) malloc(sizeof(Sala));
    if (!nova) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }

    
    snprintf(nova->nome, sizeof(nova->nome), "%s", nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}


void explorarSalas(Sala* atual) {
    char opcao;

    while (atual != NULL) {
        printf("\n📍 Você está em: **%s**\n", atual->nome);

        
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("🔚 Você chegou ao fim do caminho!\n");
            return;
        }

        printf("Para onde deseja ir?\n");
        if (atual->esquerda != NULL) printf("  (e) Esquerda → %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf("  (d) Direita  → %s\n", atual->direita->nome);
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

            case 's':
                printf("👋 Saindo da exploração...\n");
                return;

            default:
                printf("Opção inválida!\n");
        }
    }
}

int main() {
    

    
    Sala* hallEntrada = criarSala("Hall de Entrada");

    
    hallEntrada->esquerda = criarSala("Sala de Estar");
    hallEntrada->direita  = criarSala("Corredor Principal");

    
    hallEntrada->esquerda->esquerda = criarSala("Biblioteca");
    hallEntrada->esquerda->direita  = criarSala("Sala de Música");

    hallEntrada->direita->esquerda  = criarSala("Cozinha");
    hallEntrada->direita->direita   = criarSala("Jardim Interno");

    
    printf("=== 🏰 Exploração da Mansão Assombrada ===\n");
    explorarSalas(hallEntrada);

    return 0;
}
