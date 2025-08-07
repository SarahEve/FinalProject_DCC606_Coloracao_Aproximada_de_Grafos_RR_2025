#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define MAX_VERTICES 1000

// Estrutura para representar o grafo usando lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct {
    No* adj[MAX_VERTICES];
    int vertices;
} Grafo;

// Função para criar um novo nó
No* criar_no(int v) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v;
    novo_no->proximo = NULL;
    return novo_no;
}

// Função para inicializar o grafo
void inicializar_grafo(Grafo* g, int vertices) {
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        g->adj[i] = NULL;
    }
}

// Função para adicionar uma aresta ao grafo
void adicionar_aresta(Grafo* g, int u, int v) {
    // Adiciona v à lista de adjacência de u
    No* novo_no = criar_no(v);
    novo_no->proximo = g->adj[u];
    g->adj[u] = novo_no;
    
    // Adiciona u à lista de adjacência de v (grafo não direcionado)
    novo_no = criar_no(u);
    novo_no->proximo = g->adj[v];
    g->adj[v] = novo_no;
}

// Função para verificar se uma cor é segura para um vértice
bool eh_cor_segura(Grafo* g, int vertice, int cor, int* cores) {
    No* atual = g->adj[vertice];
    
    while (atual != NULL) {
        if (cores[atual->vertice] == cor) {
            return false;
        }
        atual = atual->proximo;
    }
    return true;
}

// Algoritmo guloso de coloração de grafos
void coloracao_gulosa(Grafo* g, int* cores) {
    // Inicializa o resultado (cor -1 significa não colorido)
    for (int i = 0; i < g->vertices; i++) {
        cores[i] = -1;
    }
    
    // Atribui a primeira cor (0) ao primeiro vértice
    cores[0] = 0;
    printf("Passo 1: Vértice 0 colorido com cor 0\n");
    
    // Array temporário para marcar cores disponíveis
    bool disponivel[MAX_VERTICES];
    
    // Colorir os vértices restantes
    for (int u = 1; u < g->vertices; u++) {
        // Inicializar todas as cores como disponíveis
        for (int i = 0; i < g->vertices; i++) {
            disponivel[i] = true;
        }
        
        // Marcar as cores dos vértices adjacentes como indisponíveis
        No* atual = g->adj[u];
        while (atual != NULL) {
            if (cores[atual->vertice] != -1) {
                disponivel[cores[atual->vertice]] = false;
            }
            atual = atual->proximo;
        }
        
        // Encontrar a primeira cor disponível
        int cor;
        for (cor = 0; cor < g->vertices; cor++) {
            if (disponivel[cor]) {
                break;
            }
        }
        
        cores[u] = cor;
        printf("Passo %d: Vértice %d colorido com cor %d\n", 
           u + 1, u, cor);
    }
}

// Função para imprimir o resultado
void imprimir_resultado(int* cores, int vertices) {
    printf("\nColoração dos vértices:\n");
    for (int i = 0; i < vertices; i++) {
        printf("Vértice %d -> Cor %d\n", i, cores[i]);
    }
    
    // Encontrar o número de cores utilizadas
    int cor_maxima = 0;
    for (int i = 0; i < vertices; i++) {
        if (cores[i] > cor_maxima) {
            cor_maxima = cores[i];
        }
    }

    printf("\nColoração por cor:\n");
    for (int cor = 0; cor < cor_maxima + 1; cor++) {
        printf("Cor %d: ", cor);
        for (int i = 0; i < vertices; i++) {
            if (cores[i] == cor) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    printf("\nNúmero total de cores utilizadas: %d\n", cor_maxima + 1);
}



int main() {
    setlocale(LC_ALL, "");
    int vertices;

    printf("=== ALGORITMO GULOSO PARA COLORAÇÃO DE GRAFOS ===\n\n");
    printf("Digite o número de vértices: ");
    scanf("%d", &vertices);
    

    Grafo grafo;
    inicializar_grafo(&grafo, vertices);
    
    int u, v;
    printf("Digite as arestas (u v). Digite -1 -1 para terminar:\n");
    
    while (1) {
        scanf("%d %d", &u, &v);
        if (u == -1 && v == -1) break;
        if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
            printf("Vértices inválidos! Use valores entre 0 e %d\n", vertices - 1);
            continue;
        }
        if (u == v) {
            printf("Auto-loops não são permitidos!\n");
            continue;
        }
        adicionar_aresta(&grafo, u, v);
    }
    
    int* cores = (int*)malloc(vertices * sizeof(int));
    
    printf("\n=== EXECUTANDO ALGORITMO GULOSO ===\n");
    coloracao_gulosa(&grafo, cores);
    
    imprimir_resultado(cores, vertices);
    
    return 0;
}