#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define MAX_VERTICES 100
#define MAX_COLORS 50

// Estrutura para um nó da lista de adjacência
typedef struct No {
    int vertice;
    struct No* proximo;
} No;

// Estrutura do grafo usando lista de adjacência
typedef struct {
    No* adj[MAX_VERTICES];  // Array de ponteiros para listas
    int vertices;
} Grafo;

// Inicializa o grafo
void inicializar_grafo(Grafo* g, int vertices) {
    g->vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        g->adj[i] = NULL;
    }
}

// Cria um novo nó
No* criar_no(int vertice) {
    No* novo_no = (No*)malloc(sizeof(No));
    novo_no->vertice = vertice;
    novo_no->proximo = NULL;
    return novo_no;
}

// Adiciona uma aresta ao grafo
void adicionar_aresta(Grafo* g, int u, int v) {
    // Adiciona v na lista de u
    No* novo_no = criar_no(v);
    novo_no->proximo = g->adj[u];
    g->adj[u] = novo_no;
    
    // Adiciona u na lista de v (grafo não direcionado)
    novo_no = criar_no(u);
    novo_no->proximo = g->adj[v];
    g->adj[v] = novo_no;
}

// Verifica se dois vértices são adjacentes
bool sao_adjacentes(Grafo* g, int u, int v) {
    No* atual = g->adj[u];
    while (atual != NULL) {
        if (atual->vertice == v) {
            return true;
        }
        atual = atual->proximo;
    }
    return false;
}

// Calcula o grau de um vértice
int calcular_grau(Grafo* g, int vertice) {
    int grau = 0;
    No* atual = g->adj[vertice];
    
    while (atual != NULL) {
        grau++;
        atual = atual->proximo;
    }
    
    return grau;
}

// Calcula o grau de saturação de um vértice
int calcular_grau_saturacao(Grafo* g, int vertice, int* cores) {
    bool cores_usadas[MAX_COLORS] = {false};
    int grau_sat = 0;
    
    // Percorre a lista de adjacência do vértice
    No* atual = g->adj[vertice];
    while (atual != NULL) {
        int vizinho = atual->vertice;
        
        // Se o vizinho já está colorido e sua cor não foi contada
        if (cores[vizinho] != -1 && !cores_usadas[cores[vizinho]]) {
            cores_usadas[cores[vizinho]] = true;
            grau_sat++;
        }
        
        atual = atual->proximo;
    }
    
    return grau_sat;
}

// Encontra a menor cor disponível para um vértice
int encontrar_menor_cor_disponivel(Grafo* g, int vertice, int* cores) {
    bool cores_usadas[MAX_COLORS] = {false};
    
    // Percorre a lista de vizinhos e marca cores usadas
    No* atual = g->adj[vertice];
    while (atual != NULL) {
        int vizinho = atual->vertice;
        if (cores[vizinho] != -1) {
            cores_usadas[cores[vizinho]] = true;
        }
        atual = atual->proximo;
    }
    
    // Encontra a primeira cor disponível
    for (int cor = 0; cor < MAX_COLORS; cor++) {
        if (!cores_usadas[cor]) {
            return cor;
        }
    }
    
    return -1; // Erro: não encontrou cor disponível
}

// Encontra o próximo vértice a ser colorido usando DSatur
int encontrar_proximo_vertice_dsatur(Grafo* g, int* cores, bool* colorido) {
    int max_grau_sat = -1;
    int max_grau = -1;
    int proximo_vertice = -1;
    
    for (int i = 0; i < g->vertices; i++) {
        if (!colorido[i]) {
            int grau_sat = calcular_grau_saturacao(g, i, cores);
            int grau = calcular_grau(g, i);
            
            // Escolhe o vértice com maior grau de saturação
            // Em caso de empate, escolhe o com maior grau
            if (grau_sat > max_grau_sat || 
                (grau_sat == max_grau_sat && grau > max_grau)) {
                max_grau_sat = grau_sat;
                max_grau = grau;
                proximo_vertice = i;
            }
        }
    }
    
    return proximo_vertice;
}

// Algoritmo DSatur para coloração de grafos
int dsatur_colorir_grafo(Grafo* g, int* cores) {
    bool colorido[MAX_VERTICES] = {false};
    int vertices_coloridos = 0;
    int max_cor_usada = -1;
    
    // Inicializa todas as cores como -1 (não colorido)
    for (int i = 0; i < g->vertices; i++) {
        cores[i] = -1;
    }
    
    // Primeiro vértice: escolhe o de maior grau
    int primeiro_vertice = 0;
    int max_grau = calcular_grau(g, 0);
    for (int i = 1; i < g->vertices; i++) {
        int grau = calcular_grau(g, i);
        if (grau > max_grau) {
            max_grau = grau;
            primeiro_vertice = i;
        }
    }
    
    // Colore o primeiro vértice com cor 0
    cores[primeiro_vertice] = 0;
    colorido[primeiro_vertice] = true;
    vertices_coloridos++;
    max_cor_usada = 0;
    
    printf("Passo 1: Vértice %d colorido com cor %d (grau: %d)\n", 
           primeiro_vertice, cores[primeiro_vertice], max_grau);
    
    // Colore os demais vértices
    while (vertices_coloridos < g->vertices) {
        int proximo = encontrar_proximo_vertice_dsatur(g, cores, colorido);
        
        if (proximo == -1) break; // Erro
        
        int cor = encontrar_menor_cor_disponivel(g, proximo, cores);
        cores[proximo] = cor;
        colorido[proximo] = true;
        vertices_coloridos++;
        
        if (cor > max_cor_usada) {
            max_cor_usada = cor;
        }
        
        int grau_sat = calcular_grau_saturacao(g, proximo, cores);
        int grau = calcular_grau(g, proximo);
        
        printf("Passo %d: Vértice %d colorido com cor %d (grau saturação: %d, grau: %d)\n", 
               vertices_coloridos, proximo, cor, grau_sat, grau);
    }
    
    return max_cor_usada + 1; // Número total de cores usadas
}

// Verifica se a coloração é válida
bool verificar_coloracao(Grafo* g, int* cores) {
    for (int i = 0; i < g->vertices; i++) {
        No* atual = g->adj[i];
        while (atual != NULL) {
            int vizinho = atual->vertice;
            if (vizinho > i && cores[i] == cores[vizinho]) {
                return false;
            }
            atual = atual->proximo;
        }
    }
    return true;
}

// Imprime a coloração do grafo
void imprimir_coloracao(Grafo* g, int* cores, int num_cores) {
    printf("\n=== RESULTADO DA COLORAÇÃO ===\n");
    printf("Número de cores utilizadas: %d\n\n", num_cores);
    
    for (int i = 0; i < g->vertices; i++) {
        printf("Vértice %d: Cor %d\n", i, cores[i]);
    }
    
    printf("\nColoração por cor:\n");
    for (int cor = 0; cor < num_cores; cor++) {
        printf("Cor %d: ", cor);
        for (int i = 0; i < g->vertices; i++) {
            if (cores[i] == cor) {
                printf("%d ", i);
            }
        }
        printf("\n");
    }

    printf("\nNúmero total de cores utilizadas: %d\n", num_cores);
}


int main() {
    setlocale(LC_ALL, "");
    Grafo g;
    int cores[MAX_VERTICES];
    int vertices;
    
    printf("=== ALGORITMO DSATUR PARA COLORAÇÃO DE GRAFOS ===\n\n");
    
    printf("Digite o número de vértices: ");
    scanf("%d", &vertices); 
    
    inicializar_grafo(&g, vertices);
    
    printf("Digite as arestas (u v). Digite -1 -1 para terminar:\n");
    int u, v;
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
        adicionar_aresta(&g, u, v);
    }
    
    printf("\n=== EXECUTANDO ALGORITMO DSATUR ===\n");
    int num_cores = dsatur_colorir_grafo(&g, cores);
    
    imprimir_coloracao(&g, cores, num_cores);
    
    return 0;
}