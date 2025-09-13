#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    struct Sala* esquerda;   // Ponteiro para o cômodo à esquerda
    struct Sala* direita;    // Ponteiro para o cômodo à direita
} Sala;

/*
 * Função: criarSala
 * Propósito: Cria dinamicamente uma nova sala com o nome especificado
 * Parâmetros: nome - string com o nome da sala
 * Retorno: ponteiro para a nova sala criada
 */
Sala* criarSala(const char* nome) {
    // Aloca memória dinamicamente para uma nova sala
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro: Não foi possível alocar memória para a sala.\n");
        exit(1);
    }
    
    // Copia o nome para a estrutura e inicializa os ponteiros
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Função: explorarSalas
 * Propósito: Permite a navegação interativa do jogador pela árvore
 * Parâmetros: salaAtual - ponteiro para a sala onde o jogador está
 * Retorno: void
 */
void explorarSalas(Sala* salaAtual) {
    char opcao;
    
    // Loop principal de exploração
    while (salaAtual != NULL) {
        // Exibe informações da sala atual
        printf("\n=== Você está na: %s ===\n", salaAtual->nome);
        
        // Verifica se é uma sala sem saídas (nó-folha)
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            printf("Esta sala não possui mais caminhos!\n");
            printf("Fim da exploração. Obrigado por jogar!\n");
            break;
        }
        
        // Exibe opções disponíveis
        printf("\nOpções disponíveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("(e) - Ir para a esquerda: %s\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("(d) - Ir para a direita: %s\n", salaAtual->direita->nome);
        }
        printf("(s) - Sair do jogo\n");
        
        // Solicita escolha do jogador
        printf("\nEscolha uma opção: ");
        scanf(" %c", &opcao);
        
        // Processa a escolha do jogador
        switch (opcao) {
            case 'e':
            case 'E':
                if (salaAtual->esquerda != NULL) {
                    salaAtual = salaAtual->esquerda;
                    printf("Você foi para a esquerda...\n");
                } else {
                    printf("Não há caminho à esquerda!\n");
                }
                break;
                
            case 'd':
            case 'D':
                if (salaAtual->direita != NULL) {
                    salaAtual = salaAtual->direita;
                    printf("Você foi para a direita...\n");
                } else {
                    printf("Não há caminho à direita!\n");
                }
                break;
                
            case 's':
            case 'S':
                printf("Saindo do jogo... Obrigado por jogar!\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para sair.\n");
                break;
        }
    }
}

/*
 * Função: liberarMemoria
 * Propósito: Libera recursivamente toda a memória alocada para a árvore
 * Parâmetros: sala - ponteiro para o nó raiz da árvore
 * Retorno: void
 */
void liberarMemoria(Sala* sala) {
    if (sala != NULL) {
        // Libera recursivamente os filhos
        liberarMemoria(sala->esquerda);
        liberarMemoria(sala->direita);
        // Libera o nó atual
        free(sala);
    }
}

/*
 * Função: main
 * Propósito: Monta o mapa inicial da mansão e inicia a exploração
 * Retorno: 0 se execução bem-sucedida
 */
int main() {
    // Apresentação do jogo
    printf("========================================\n");
    printf("    BEM-VINDO À MANSÃO MISTERIOSA!     \n");
    printf("========================================\n");
    printf("Explore os cômodos da mansão para encontrar pistas.\n");
    printf("Use 'e' para ir à esquerda, 'd' para a direita, ou 's' para sair.\n");
    printf("========================================\n");
    
    // Criação da estrutura da mansão (árvore binária)
    // Nível 0 - Entrada principal
    Sala* hall = criarSala("Hall de Entrada");
    
    // Nível 1 - Salas principais
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* biblioteca = criarSala("Biblioteca");
    
    // Nível 2 - Cômodos secundários
    Sala* cozinha = criarSala("Cozinha");
    Sala* quarto1 = criarSala("Quarto Principal");
    Sala* escritorio = criarSala("Escritório");
    Sala* jardim = criarSala("Jardim");
    
    // Nível 3 - Cômodos finais (nós-folha)
    Sala* despensa = criarSala("Despensa");
    Sala* banheiro = criarSala("Banheiro");
    Sala* closet = criarSala("Closet");
    Sala* varanda = criarSala("Varanda");
    
    // Montagem da árvore - conectando os nós
    // Hall de Entrada
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    
    // Sala de Estar
    salaEstar->esquerda = cozinha;
    salaEstar->direita = quarto1;
    
    // Biblioteca
    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;
    
    // Cozinha
    cozinha->esquerda = despensa;
    cozinha->direita = banheiro;
    
    // Quarto Principal
    quarto1->esquerda = closet;
    quarto1->direita = varanda;
    
    // Escritório e Jardim são nós-folha (sem filhos)
    // Despensa, Banheiro, Closet e Varanda também são nós-folha
    
    // Inicia a exploração a partir do Hall de Entrada
    explorarSalas(hall);
    
    // Libera toda a memória alocada
    liberarMemoria(hall);
    
    return 0;
}