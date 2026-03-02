#ifndef STATIC_LIST_H
#define STATIC_LIST_H

/* ============================================================
 * static_list.h — Lista Encadeada Estática (vetor pré-alocado)
 * Autor: Jetro
 *
 * Implementa a lista encadeada usando um vetor de nós pré-alocado
 * e gerenciamento manual de posições livres (free list).
 * ============================================================ */

#include "student.h"

#define SLIST_MAX_SIZE 12000  /* capacidade máxima da lista */
#define SLIST_NULL     -1     /* índice nulo (equivalente a NULL) */

/* ------------------------------------------------------------ *
 *  Nó da lista estática
 *  - data : dados do aluno
 *  - next : índice do próximo nó no vetor (SLIST_NULL = fim)
 * ------------------------------------------------------------ */
typedef struct {
    Student data;
    int     next;
} SNode;

/* ------------------------------------------------------------ *
 *  Estrutura da lista estática
 *  - pool      : vetor pré-alocado de nós
 *  - head      : índice do primeiro nó da lista (-1 = vazia)
 *  - free_head : índice do primeiro nó livre
 *  - size      : quantidade de elementos ativos
 *  - capacity  : tamanho do pool
 * ------------------------------------------------------------ */
typedef struct {
    SNode pool[SLIST_MAX_SIZE];
    int   head;
    int   free_head;
    int   size;
    int   capacity;
} StaticList;

/* ------------------------------------------------------------ *
 *  Protótipos
 * ------------------------------------------------------------ */

/* Inicializa a lista e configura a free list */
void slist_init(StaticList *list);

/* Insere no início — O(1) */
int slist_insert_front(StaticList *list, Student s);

/* Insere no final — O(n) */
int slist_insert_back(StaticList *list, Student s);

/* Insere em posição específica (0-indexed) — O(n) */
int slist_insert_at(StaticList *list, int pos, Student s);

/* Remove o nó com o id informado — O(n); retorna 1 se removido */
int slist_remove_by_id(StaticList *list, int id);

/* Busca por id; retorna índice no pool ou SLIST_NULL — O(n) */
int slist_search_by_id(const StaticList *list, int id);

/* Busca por nome parcial; retorna índice ou SLIST_NULL — O(n) */
int slist_search_by_name(const StaticList *list, const char *name);

/* Imprime todos os elementos */
void slist_print(const StaticList *list);

/* Retorna o tamanho da lista */
int slist_size(const StaticList *list);

/* "Libera" a lista (reinicializa a free list, não desaloca memória) */
void slist_clear(StaticList *list);

/* Copia os dados de um array para a lista (substitui conteúdo) */
void slist_load_from_array(StaticList *list, const Student *arr, int n);

/* Exporta os dados da lista para um array (alocado dinamicamente) */
Student *slist_to_array(const StaticList *list);

#endif /* STATIC_LIST_H */
