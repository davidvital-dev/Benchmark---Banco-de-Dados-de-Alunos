#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

/* ============================================================
 * dynamic_list.h — Lista Encadeada Dinâmica (alocação em heap)
 * Autor: Luiz
 * ============================================================ */

#include "student.h"

/* ------------------------------------------------------------ *
 *  Nó da lista encadeada dinâmica
 * ------------------------------------------------------------ */
typedef struct DNode {
    Student       data;
    struct DNode *prev;
    struct DNode *next;
} DNode;

/* ------------------------------------------------------------ *
 *  Estrutura da lista
 * ------------------------------------------------------------ */
typedef struct {
    DNode *head;   /* Ponteiro para o primeiro nó */
    int    size;   /* Quantidade de elementos */
} DynamicList;

/* ------------------------------------------------------------ *
 *  Protótipos
 * ------------------------------------------------------------ */

/* Inicializa a lista (deve ser chamada antes de qualquer uso) */
void dlist_init(DynamicList *list);

/* Insere no início da lista — O(1) */
int dlist_insert_front(DynamicList *list, Student s);

/* Insere no final da lista — O(n) */
int dlist_insert_back(DynamicList *list, Student s);

/* Insere em posição específica (0-indexed) — O(n) */
int dlist_insert_at(DynamicList *list, int pos, Student s);

/* Remove o nó com o id informado — O(n); retorna 1 se removido */
int dlist_remove_by_id(DynamicList *list, int id);

/* Busca por id; retorna ponteiro para o nó ou NULL — O(n) */
DNode *dlist_search_by_id(const DynamicList *list, int id);

/* Busca por nome parcial; retorna ponteiro ou NULL — O(n) */
DNode *dlist_search_by_name(const DynamicList *list, const char *name);

/* Imprime todos os elementos */
void dlist_print(const DynamicList *list);

/* Retorna o tamanho da lista */
int dlist_size(const DynamicList *list);

/* Libera toda a memória alocada */
void dlist_free(DynamicList *list);

/* Copia os dados de um array para a lista (substitui conteúdo) */
void dlist_load_from_array(DynamicList *list, const Student *arr, int n);

/* Exporta os dados da lista para um array (alocado internamente) */
Student *dlist_to_array(const DynamicList *list);

#endif /* DYNAMIC_LIST_H */
