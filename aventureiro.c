#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição da estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    char pista[100];         // Pista encontrada na sala (opcional)
    struct Sala* esquerda;   // Ponteiro para o cômodo à esquerda
    struct Sala* direita;    // Ponteiro para o cômodo à direita
} Sala;

// Definição da estrutura para nós da árvore BST de pistas
typedef struct PistaNode {
    char conteudo[100];           // Conteúdo da pista
    struct PistaNode* esquerda;   // Filho esquerdo (menor alfabeticamente)
    struct PistaNode* direita;    // Filho direito (maior alfabeticamente)
} PistaNode;

// Variável global para armazenar a raiz da árvore de pistas
PistaNode* raizPistas = NULL;

/*
 * Função: criarSala
 * Propósito: Cria dinamicamente uma nova sala com nome e pista opcional
 * Parâmetros: nome - string com o nome da sala
 *            pista - string com a pista (pode ser vazia)
 * Retorno: ponteiro para a nova sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    // Aloca memória dinamicamente para uma nova sala
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    
    // Verifica se a alocação foi bem-sucedida
    if (novaSala == NULL) {
        printf("Erro: Não foi possível alocar memória para a sala.\n");
        exit(1);
    }
    
    // Copia o nome e a pista para a estrutura e inicializa os ponteiros
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/*
 * Função: criarPistaNode
 * Propósito: Cria dinamicamente um novo nó para a árvore BST de pistas
 * Parâmetros: conteudo - string com o conteúdo da pista
 * Retorno: ponteiro para o novo nó criado
 */
PistaNode* criarPistaNode(const char* conteudo) {
    // Aloca memória dinamicamente para um novo nó de pista
    PistaNode* novoNode = (PistaNode*)malloc(sizeof(PistaNode));
    
    // Verifica se a alocação foi bem-sucedida
    if (novoNode == NULL) {
        printf("Erro: Não foi possível alocar memória para a pista.\n");
        exit(1);
    }
    
    // Copia o conteúdo e inicializa os ponteiros
    strcpy(novoNode->conteudo, conteudo);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    
    return novoNode;
}

/*
 * Função: inserirPista
 * Propósito: Insere uma nova pista na árvore BST mantendo ordem alfabética
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 *            conteudo - string com o conteúdo da pista
 * Retorno: ponteiro para a raiz da árvore (pode ter mudado)
 */
PistaNode* inserirPista(PistaNode* raiz, const char* conteudo) {
    // Caso base: se a árvore está vazia, cria um novo nó
    if (raiz == NULL) {
        return criarPistaNode(conteudo);
    }
    
    // Compara o conteúdo com o nó atual para decidir onde inserir
    int comparacao = strcmp(conteudo, raiz->conteudo);
    
    if (comparacao < 0) {
        // Conteúdo é menor alfabeticamente, insere à esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (comparacao > 0) {
        // Conteúdo é maior alfabeticamente, insere à direita
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    // Se comparacao == 0, a pista já existe, não insere duplicata
    
    return raiz;
}

/*
 * Função: exibirPistas
 * Propósito: Exibe todas as pistas da árvore BST em ordem alfabética
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 * Retorno: void
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        // Percurso em ordem (in-order): esquerda -> raiz -> direita
        exibirPistas(raiz->esquerda);
        printf("  • %s\n", raiz->conteudo);
        exibirPistas(raiz->direita);
    }
}

/*
 * Função: contarPistas
 * Propósito: Conta o número total de pistas coletadas
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 * Retorno: número inteiro com a quantidade de pistas
 */
int contarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

/*
 * Função: explorarSalasComPistas
 * Propósito: Permite a navegação interativa com coleta automática de pistas
 * Parâmetros: salaAtual - ponteiro para a sala onde o jogador está
 * Retorno: void
 */
void explorarSalasComPistas(Sala* salaAtual) {
    char opcao;
    
    // Loop principal de exploração
    while (salaAtual != NULL) {
        // Exibe informações da sala atual
        printf("\n=== Você está na: %s ===\n", salaAtual->nome);
        
        // Verifica se há uma pista na sala atual
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 PISTA ENCONTRADA: %s\n", salaAtual->pista);
            printf("   (Pista adicionada ao seu inventário)\n");
            
            // Adiciona a pista à árvore BST
            raizPistas = inserirPista(raizPistas, salaAtual->pista);
        } else {
            printf("   Nenhuma pista encontrada nesta sala.\n");
        }
        
        // Exibe opções disponíveis
        printf("\nOpções disponíveis:\n");
        if (salaAtual->esquerda != NULL) {
            printf("(e) - Ir para a esquerda: %s\n", salaAtual->esquerda->nome);
        }
        if (salaAtual->direita != NULL) {
            printf("(d) - Ir para a direita: %s\n", salaAtual->direita->nome);
        }
        printf("(s) - Sair e ver relatório de pistas\n");
        
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
                printf("\n========================================\n");
                printf("    RELATÓRIO FINAL DE INVESTIGAÇÃO    \n");
                printf("========================================\n");
                
                int totalPistas = contarPistas(raizPistas);
                if (totalPistas > 0) {
                    printf("Pistas coletadas (%d total):\n\n", totalPistas);
                    exibirPistas(raizPistas);
                    printf("\n========================================\n");
                    printf("Investigação concluída! Analise as pistas\n");
                    printf("para resolver o mistério da mansão.\n");
                } else {
                    printf("Nenhuma pista foi coletada durante a exploração.\n");
                    printf("Tente explorar mais cômodos da mansão!\n");
                }
                printf("========================================\n");
                return;
                
            default:
                printf("Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para sair.\n");
                break;
        }
    }
}

/*
 * Função: liberarMemoriaSalas
 * Propósito: Libera recursivamente toda a memória alocada para a árvore de salas
 * Parâmetros: sala - ponteiro para o nó raiz da árvore
 * Retorno: void
 */
void liberarMemoriaSalas(Sala* sala) {
    if (sala != NULL) {
        // Libera recursivamente os filhos
        liberarMemoriaSalas(sala->esquerda);
        liberarMemoriaSalas(sala->direita);
        // Libera o nó atual
        free(sala);
    }
}

/*
 * Função: liberarMemoriaBST
 * Propósito: Libera recursivamente toda a memória alocada para a árvore BST de pistas
 * Parâmetros: raiz - ponteiro para o nó raiz da árvore BST
 * Retorno: void
 */
void liberarMemoriaBST(PistaNode* raiz) {
    if (raiz != NULL) {
        // Libera recursivamente os filhos
        liberarMemoriaBST(raiz->esquerda);
        liberarMemoriaBST(raiz->direita);
        // Libera o nó atual
        free(raiz);
    }
}

/*
 * Função: main
 * Propósito: Monta o mapa da mansão com pistas e inicia a exploração
 * Retorno: 0 se execução bem-sucedida
 */
int main() {
    // Apresentação do jogo
    printf("========================================\n");
    printf("       DETECTIVE QUEST - EXPANDIDO     \n");
    printf("========================================\n");
    printf("Explore a mansão misteriosa e colete pistas\n");
    printf("para resolver o caso! As pistas serão\n");
    printf("organizadas alfabeticamente no final.\n");
    printf("========================================\n");
    printf("Comandos: 'e' (esquerda), 'd' (direita), 's' (sair)\n");
    printf("========================================\n");
    
    // Criação da estrutura da mansão com pistas associadas
    // Nível 0 - Entrada principal
    Sala* hall = criarSala("Hall de Entrada", "Mapa da mansão encontrado");
    
    // Nível 1 - Salas principais
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas suspeitas no tapete");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com páginas rasgadas");
    
    // Nível 2 - Cômodos secundários
    Sala* cozinha = criarSala("Cozinha", "Faca com manchas estranhas");
    Sala* quarto1 = criarSala("Quarto Principal", "Carta misteriosa na gaveta");
    Sala* escritorio = criarSala("Escritório", "Documento confidencial");
    Sala* jardim = criarSala("Jardim", "Chave enterrada no solo");
    
    // Nível 3 - Cômodos finais
    Sala* despensa = criarSala("Despensa", ""); // Sem pista
    Sala* banheiro = criarSala("Banheiro", "Frasco de remédio vazio");
    Sala* closet = criarSala("Closet", "Joia valiosa escondida");
    Sala* varanda = criarSala("Varanda", ""); // Sem pista
    
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
    explorarSalasComPistas(hall);
    
    // Libera toda a memória alocada
    liberarMemoriaSalas(hall);
    liberarMemoriaBST(raizPistas);
    
    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}