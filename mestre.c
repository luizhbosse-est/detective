#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 13
#define MAX_SUSPEITOS 10

// Definição da estrutura que representa uma sala da mansão
typedef struct Sala {
    char nome[50];           // Nome da sala
    char pista[100];         // Pista encontrada na sala
    struct Sala* esquerda;   // Ponteiro para o cômodo à esquerda
    struct Sala* direita;    // Ponteiro para o cômodo à direita
} Sala;

// Definição da estrutura para nós da árvore BST de pistas
typedef struct PistaNode {
    char conteudo[100];           // Conteúdo da pista
    struct PistaNode* esquerda;   // Filho esquerdo (menor alfabeticamente)
    struct PistaNode* direita;    // Filho direito (maior alfabeticamente)
} PistaNode;

// Definição da estrutura para nós da tabela hash
typedef struct HashNode {
    char pista[100];              // Chave: conteúdo da pista
    char suspeito[50];            // Valor: nome do suspeito
    struct HashNode* proximo;     // Ponteiro para próximo nó (tratamento de colisões)
} HashNode;

// Estrutura para contar pistas por suspeito
typedef struct ContadorSuspeito {
    char nome[50];
    int contador;
} ContadorSuspeito;

// Variáveis globais
PistaNode* raizPistas = NULL;
HashNode* tabelaHash[TAMANHO_HASH];
ContadorSuspeito contadores[MAX_SUSPEITOS];
int numSuspeitos = 0;

/*
 * Função: funcaoHash
 * Propósito: Calcula o índice hash para uma string
 * Parâmetros: chave - string para calcular hash
 * Retorno: índice hash (0 a TAMANHO_HASH-1)
 */
unsigned int funcaoHash(const char* chave) {
    unsigned int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash = hash * 31 + chave[i];
    }
    return hash % TAMANHO_HASH;
}

/*
 * Função: criarSala
 * Propósito: Cria dinamicamente uma nova sala com nome e pista
 * Parâmetros: nome - string com o nome da sala
 *            pista - string com a pista (pode ser vazia)
 * Retorno: ponteiro para a nova sala criada
 */
Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    
    if (novaSala == NULL) {
        printf("Erro: Não foi possível alocar memória para a sala.\n");
        exit(1);
    }
    
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
    PistaNode* novoNode = (PistaNode*)malloc(sizeof(PistaNode));
    
    if (novoNode == NULL) {
        printf("Erro: Não foi possível alocar memória para a pista.\n");
        exit(1);
    }
    
    strcpy(novoNode->conteudo, conteudo);
    novoNode->esquerda = NULL;
    novoNode->direita = NULL;
    
    return novoNode;
}

/*
 * Função: criarHashNode
 * Propósito: Cria dinamicamente um novo nó para a tabela hash
 * Parâmetros: pista - string com a pista (chave)
 *            suspeito - string com o nome do suspeito (valor)
 * Retorno: ponteiro para o novo nó hash criado
 */
HashNode* criarHashNode(const char* pista, const char* suspeito) {
    HashNode* novoNode = (HashNode*)malloc(sizeof(HashNode));
    
    if (novoNode == NULL) {
        printf("Erro: Não foi possível alocar memória para o nó hash.\n");
        exit(1);
    }
    
    strcpy(novoNode->pista, pista);
    strcpy(novoNode->suspeito, suspeito);
    novoNode->proximo = NULL;
    
    return novoNode;
}

/*
 * Função: inserirPista
 * Propósito: Insere uma nova pista na árvore BST mantendo ordem alfabética
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 *            conteudo - string com o conteúdo da pista
 * Retorno: ponteiro para a raiz da árvore
 */
PistaNode* inserirPista(PistaNode* raiz, const char* conteudo) {
    if (raiz == NULL) {
        return criarPistaNode(conteudo);
    }
    
    int comparacao = strcmp(conteudo, raiz->conteudo);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, conteudo);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, conteudo);
    }
    // Se comparacao == 0, a pista já existe, não insere duplicata
    
    return raiz;
}

/*
 * Função: inserirNaHash
 * Propósito: Insere associação pista/suspeito na tabela hash
 * Parâmetros: pista - string com a pista (chave)
 *            suspeito - string com o suspeito (valor)
 * Retorno: void
 */
void inserirNaHash(const char* pista, const char* suspeito) {
    unsigned int indice = funcaoHash(pista);
    HashNode* novoNode = criarHashNode(pista, suspeito);
    
    // Inserção no início da lista (tratamento de colisões por encadeamento)
    novoNode->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNode;
}

/*
 * Função: encontrarSuspeito
 * Propósito: Consulta o suspeito correspondente a uma pista na tabela hash
 * Parâmetros: pista - string com a pista a ser consultada
 * Retorno: ponteiro para string com nome do suspeito ou NULL se não encontrado
 */
char* encontrarSuspeito(const char* pista) {
    unsigned int indice = funcaoHash(pista);
    HashNode* atual = tabelaHash[indice];
    
    // Percorre a lista ligada no índice calculado
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return NULL; // Pista não encontrada
}

/*
 * Função: adicionarSuspeitoContador
 * Propósito: Adiciona ou incrementa contador de um suspeito
 * Parâmetros: suspeito - nome do suspeito
 * Retorno: void
 */
void adicionarSuspeitoContador(const char* suspeito) {
    // Procura se o suspeito já existe no array de contadores
    for (int i = 0; i < numSuspeitos; i++) {
        if (strcmp(contadores[i].nome, suspeito) == 0) {
            contadores[i].contador++;
            return;
        }
    }
    
    // Se não existe, adiciona novo suspeito
    if (numSuspeitos < MAX_SUSPEITOS) {
        strcpy(contadores[numSuspeitos].nome, suspeito);
        contadores[numSuspeitos].contador = 1;
        numSuspeitos++;
    }
}

/*
 * Função: contarPistasPorSuspeito
 * Propósito: Conta quantas pistas apontam para cada suspeito
 * Parâmetros: raiz - raiz da árvore BST de pistas
 * Retorno: void
 */
void contarPistasPorSuspeito(PistaNode* raiz) {
    if (raiz != NULL) {
        // Busca o suspeito associado à pista atual
        char* suspeito = encontrarSuspeito(raiz->conteudo);
        if (suspeito != NULL) {
            adicionarSuspeitoContador(suspeito);
        }
        
        // Recursão para filhos
        contarPistasPorSuspeito(raiz->esquerda);
        contarPistasPorSuspeito(raiz->direita);
    }
}

/*
 * Função: exibirPistas
 * Propósito: Exibe todas as pistas da árvore BST em ordem alfabética
 * Parâmetros: raiz - ponteiro para a raiz da árvore BST
 * Retorno: void
 */
void exibirPistas(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistas(raiz->esquerda);
        char* suspeito = encontrarSuspeito(raiz->conteudo);
        printf("  • %s", raiz->conteudo);
        if (suspeito != NULL) {
            printf(" → %s", suspeito);
        }
        printf("\n");
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
 * Função: verificarSuspeitoFinal
 * Propósito: Conduz a fase de julgamento final do jogo
 * Parâmetros: void
 * Retorno: void
 */
void verificarSuspeitoFinal() {
    printf("\n========================================\n");
    printf("    FASE DE JULGAMENTO FINAL           \n");
    printf("========================================\n");
    
    int totalPistas = contarPistas(raizPistas);
    if (totalPistas == 0) {
        printf("Nenhuma pista foi coletada! Não é possível fazer uma acusação.\n");
        return;
    }
    
    printf("RELATÓRIO DE EVIDÊNCIAS COLETADAS:\n\n");
    exibirPistas(raizPistas);
    
    // Conta pistas por suspeito
    numSuspeitos = 0; // Reset contador
    contarPistasPorSuspeito(raizPistas);
    
    printf("\n========================================\n");
    printf("ANÁLISE DE SUSPEITOS:\n\n");
    
    for (int i = 0; i < numSuspeitos; i++) {
        printf("%d. %s (%d pista%s)\n", 
               i + 1, 
               contadores[i].nome, 
               contadores[i].contador,
               contadores[i].contador == 1 ? "" : "s");
    }
    
    printf("\n========================================\n");
    printf("Baseado nas evidências coletadas, quem você acusa?\n");
    printf("Digite o número do suspeito (1-%d): ", numSuspeitos);
    
    int escolha;
    scanf("%d", &escolha);
    
    if (escolha < 1 || escolha > numSuspeitos) {
        printf("\nEscolha inválida! Julgamento cancelado.\n");
        return;
    }
    
    char* suspeitoAcusado = contadores[escolha - 1].nome;
    int pistasDoSuspeito = contadores[escolha - 1].contador;
    
    printf("\n========================================\n");
    printf("    VEREDICTO FINAL                    \n");
    printf("========================================\n");
    
    printf("Você acusou: %s\n", suspeitoAcusado);
    printf("Evidências contra o acusado: %d pista%s\n\n", 
           pistasDoSuspeito, 
           pistasDoSuspeito == 1 ? "" : "s");
    
    if (pistasDoSuspeito >= 2) {
        printf("🎉 PARABÉNS! CASO RESOLVIDO! 🎉\n\n");
        printf("Você coletou evidências suficientes para sustentar\n");
        printf("sua acusação. %s foi considerado culpado!\n\n", suspeitoAcusado);
        printf("Com %d pistas apontando para o suspeito, o caso\n", pistasDoSuspeito);
        printf("foi encerrado com sucesso. Excelente trabalho, detetive!\n");
    } else {
        printf("❌ CASO NÃO RESOLVIDO ❌\n\n");
        printf("Evidências insuficientes para sustentar a acusação.\n");
        printf("São necessárias pelo menos 2 pistas apontando para\n");
        printf("o mesmo suspeito. Você coletou apenas %d pista.\n\n", pistasDoSuspeito);
        printf("Continue explorando a mansão para encontrar mais\n");
        printf("evidências antes de fazer uma acusação!\n");
    }
    
    printf("========================================\n");
}

/*
 * Função: explorarSalas
 * Propósito: Permite a navegação interativa com coleta automática de pistas
 * Parâmetros: salaAtual - ponteiro para a sala onde o jogador está
 * Retorno: void
 */
void explorarSalas(Sala* salaAtual) {
    char opcao;
    
    while (salaAtual != NULL) {
        printf("\n=== Você está na: %s ===\n", salaAtual->nome);
        
        // Verifica se há uma pista na sala atual
        if (strlen(salaAtual->pista) > 0) {
            printf("🔍 PISTA ENCONTRADA: %s\n", salaAtual->pista);
            
            // Adiciona a pista à árvore BST
            raizPistas = inserirPista(raizPistas, salaAtual->pista);
            
            // Busca o suspeito associado
            char* suspeito = encontrarSuspeito(salaAtual->pista);
            if (suspeito != NULL) {
                printf("   Esta pista aponta para: %s\n", suspeito);
            }
            printf("   (Pista adicionada ao seu inventário)\n");
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
        printf("(s) - Finalizar exploração e fazer julgamento\n");
        
        printf("\nEscolha uma opção: ");
        scanf(" %c", &opcao);
        
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
                verificarSuspeitoFinal();
                return;
                
            default:
                printf("Opção inválida! Use 'e' para esquerda, 'd' para direita ou 's' para finalizar.\n");
                break;
        }
    }
}

/*
 * Função: inicializarTabelaHash
 * Propósito: Inicializa a tabela hash e popula com associações pista-suspeito
 * Parâmetros: void
 * Retorno: void
 */
void inicializarTabelaHash() {
    // Inicializa todos os índices como NULL
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
    
    // Popula a tabela hash com associações pista-suspeito
    inserirNaHash("Mapa da mansão encontrado", "Mordomo");
    inserirNaHash("Pegadas suspeitas no tapete", "Jardineiro");
    inserirNaHash("Livro com páginas rasgadas", "Bibliotecária");
    inserirNaHash("Faca com manchas estranhas", "Cozinheiro");
    inserirNaHash("Carta misteriosa na gaveta", "Mordomo");
    inserirNaHash("Documento confidencial", "Secretária");
    inserirNaHash("Chave enterrada no solo", "Jardineiro");
    inserirNaHash("Frasco de remédio vazio", "Enfermeira");
    inserirNaHash("Joia valiosa escondida", "Cozinheiro");
}

/*
 * Função: liberarMemoriaSalas
 * Propósito: Libera recursivamente toda a memória alocada para a árvore de salas
 * Parâmetros: sala - ponteiro para o nó raiz da árvore
 * Retorno: void
 */
void liberarMemoriaSalas(Sala* sala) {
    if (sala != NULL) {
        liberarMemoriaSalas(sala->esquerda);
        liberarMemoriaSalas(sala->direita);
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
        liberarMemoriaBST(raiz->esquerda);
        liberarMemoriaBST(raiz->direita);
        free(raiz);
    }
}

/*
 * Função: liberarMemoriaHash
 * Propósito: Libera toda a memória alocada para a tabela hash
 * Parâmetros: void
 * Retorno: void
 */
void liberarMemoriaHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }
}

/*
 * Função: main
 * Propósito: Função principal que inicializa o jogo e coordena a execução
 * Retorno: 0 se execução bem-sucedida
 */
int main() {
    // Apresentação do jogo
    printf("========================================\n");
    printf("    DETECTIVE QUEST - VERSÃO FINAL     \n");
    printf("========================================\n");
    printf("Bem-vindo ao desafio final! Sua missão:\n");
    printf("• Explore a mansão e colete pistas\n");
    printf("• Associe as evidências aos suspeitos\n");
    printf("• Faça uma acusação baseada nas provas\n");
    printf("• Resolva o mistério da mansão!\n");
    printf("========================================\n");
    printf("REGRA: Você precisa de pelo menos 2 pistas\n");
    printf("apontando para o mesmo suspeito para vencer!\n");
    printf("========================================\n");
    
    // Inicializa a tabela hash com associações pista-suspeito
    inicializarTabelaHash();
    
    // Criação da estrutura da mansão com pistas associadas
    Sala* hall = criarSala("Hall de Entrada", "Mapa da mansão encontrado");
    
    Sala* salaEstar = criarSala("Sala de Estar", "Pegadas suspeitas no tapete");
    Sala* biblioteca = criarSala("Biblioteca", "Livro com páginas rasgadas");
    
    Sala* cozinha = criarSala("Cozinha", "Faca com manchas estranhas");
    Sala* quarto1 = criarSala("Quarto Principal", "Carta misteriosa na gaveta");
    Sala* escritorio = criarSala("Escritório", "Documento confidencial");
    Sala* jardim = criarSala("Jardim", "Chave enterrada no solo");
    
    Sala* despensa = criarSala("Despensa", "");
    Sala* banheiro = criarSala("Banheiro", "Frasco de remédio vazio");
    Sala* closet = criarSala("Closet", "Joia valiosa escondida");
    Sala* varanda = criarSala("Varanda", "");
    
    // Montagem da árvore
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    
    salaEstar->esquerda = cozinha;
    salaEstar->direita = quarto1;
    
    biblioteca->esquerda = escritorio;
    biblioteca->direita = jardim;
    
    cozinha->esquerda = despensa;
    cozinha->direita = banheiro;
    
    quarto1->esquerda = closet;
    quarto1->direita = varanda;
    
    // Inicia a exploração
    explorarSalas(hall);
    
    // Libera toda a memória alocada
    liberarMemoriaSalas(hall);
    liberarMemoriaBST(raizPistas);
    liberarMemoriaHash();
    
    printf("\nObrigado por jogar Detective Quest!\n");
    return 0;
}