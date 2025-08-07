#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define MAX_VERTICES 1000

// Estrutura para representar o grafo usando lista de adjac�ncia
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

typedef struct {
    No* adj[MAX_VERTICES];
    int vertices;
} Grafo;

// Fun��o para criar um novo n�
No* criar_no(int v) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = v;
    novo_no->proximo = NULL;
    return novo_no;
}

// Fun��o para inicializar o grafo
void inicializar_grafo(Grafo* g, int vertices) {
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        g->adj[i] = NULL;
    }
}

// Fun��o para adicionar uma aresta ao grafo
void adicionar_aresta(Grafo* g, int u, int v) {
    // Adiciona v � lista de adjac�ncia de u
    No* novo_no = criar_no(v);
    novo_no->proximo = g->adj[u];
    g->adj[u] = novo_no;
    
    // Adiciona u � lista de adjac�ncia de v (grafo n�o direcionado)
    novo_no = criar_no(u);
    novo_no->proximo = g->adj[v];
    g->adj[v] = novo_no;
}

// Fun��o para verificar se uma cor � segura para um v�rtice
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

// Algoritmo guloso de colora��o de grafos
void coloracao_gulosa(Grafo* g, int* cores) {
    // Inicializa o resultado (cor -1 significa n�o colorido)
    for (int i = 0; i < g->vertices; i++) {
        cores[i] = -1;
    }
    
    // Atribui a primeira cor (0) ao primeiro v�rtice
    cores[0] = 0;
    printf("Passo 1: V�rtice 0 colorido com cor 0\n");
    
    // Array tempor�rio para marcar cores dispon�veis
    bool disponivel[MAX_VERTICES];
    
    // Colorir os v�rtices restantes
    for (int u = 1; u < g->vertices; u++) {
        // Inicializar todas as cores como dispon�veis
        for (int i = 0; i < g->vertices; i++) {
            disponivel[i] = true;
        }
        
        // Marcar as cores dos v�rtices adjacentes como indispon�veis
        No* atual = g->adj[u];
        while (atual != NULL) {
            if (cores[atual->vertice] != -1) {
                disponivel[cores[atual->vertice]] = false;
            }
            atual = atual->proximo;
        }
        
        // Encontrar a primeira cor dispon�vel
        int cor;
        for (cor = 0; cor < g->vertices; cor++) {
            if (disponivel[cor]) {
                break;
            }
        }
        
        cores[u] = cor;
        printf("Passo %d: V�rtice %d colorido com cor %d\n", 
           u + 1, u, cor);
    }
}

// Fun��o para imprimir o resultado
void imprimir_resultado(int* cores, int vertices) {
    printf("\nColora��o dos v�rtices:\n");
    for (int i = 0; i < vertices; i++) {
        printf("V�rtice %d -> Cor %d\n", i, cores[i]);
    }
    
    // Encontrar o n�mero de cores utilizadas
    int cor_maxima = 0;
    for (int i = 0; i < vertices; i++) {
        if (cores[i] > cor_maxima) {
            cor_maxima = cores[i];
        }
    }

    printf("\nColora��o por cor:\n");
    for (int cor = 0; cor < cor_maxima + 1; cor++) {
        printf("Cor %d: ", cor);
        for (int i = 0; i < vertices; i++) {
            if (cores[i] == cor) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    printf("\nN�mero total de cores utilizadas: %d\n", cor_maxima + 1);
}



int main() {
    setlocale(LC_ALL, "");
    int vertices;

    printf("=== ALGORITMO GULOSO PARA COLORA��O DE GRAFOS ===\n\n");
    printf("Digite o n�mero de v�rtices: ");
    scanf("%d", &vertices);
    

    Grafo grafo;
    inicializar_grafo(&grafo, vertices);
    
    int u, v;
    printf("Digite as arestas (u v). Digite -1 -1 para terminar:\n");
    
    while (1) {
        scanf("%d %d", &u, &v);
        if (u == -1 && v == -1) break;
        if (u < 0 || u >= vertices || v < 0 || v >= vertices) {
            printf("V�rtices inv�lidos! Use valores entre 0 e %d\n", vertices - 1);
            continue;
        }
        if (u == v) {
            printf("Auto-loops n�o s�o permitidos!\n");
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