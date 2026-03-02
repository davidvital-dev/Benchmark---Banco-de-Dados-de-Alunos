/* ============================================================
 * static_list.c — Implementacao da Lista Encadeada Estatica
 * Autor: Jetro
 * ============================================================ */

#include "static_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ *
 *  Inicializacao
 *  Encadeia todos os slots do pool na free list.
 * ------------------------------------------------------------ */
void slist_init(StaticList *list) {
    if (!list) return;

    list->head      = SLIST_NULL;
    list->size      = 0;
    list->capacity  = SLIST_MAX_SIZE;

    /* Monta a free list: pool[0] -> pool[1] -> ... -> pool[MAX-1] -> NULL */
    for (int i = 0; i < SLIST_MAX_SIZE - 1; i++) {
        list->pool[i].next = i + 1;
    }
    list->pool[SLIST_MAX_SIZE - 1].next = SLIST_NULL;
    list->free_head = 0;
}

/* ------------------------------------------------------------ *
 *  Aloca um slot da free list
 * ------------------------------------------------------------ */
static int slist_alloc(StaticList *list) {
    if (list->free_head == SLIST_NULL) return SLIST_NULL; /* cheio */
    int idx = list->free_head;
    list->free_head = list->pool[idx].next;
    list->pool[idx].next = SLIST_NULL;
    return idx;
}

/* ------------------------------------------------------------ *
 *  Devolve um slot para a free list
 * ------------------------------------------------------------ */
static void slist_dealloc(StaticList *list, int idx) {
    list->pool[idx].next = list->free_head;
    list->free_head = idx;
}

/* ------------------------------------------------------------ *
 *  Insercao
 * ------------------------------------------------------------ */
int slist_insert_front(StaticList *list, Student s) {
    if (!list) return 0;

    int idx = slist_alloc(list);
    if (idx == SLIST_NULL) {
        fprintf(stderr, "[AVISO] slist: lista cheia (capacidade=%d)\n",
                list->capacity);
        return 0;
    }

    list->pool[idx].data = s;
    list->pool[idx].next = list->head;
    list->head = idx;
    list->size++;
    return 1;
}

int slist_insert_back(StaticList *list, Student s) {
    if (!list) return 0;

    int idx = slist_alloc(list);
    if (idx == SLIST_NULL) {
        fprintf(stderr, "[AVISO] slist: lista cheia\n");
        return 0;
    }

    list->pool[idx].data = s;
    list->pool[idx].next = SLIST_NULL;

    if (list->head == SLIST_NULL) {
        list->head = idx;
    } else {
        int cur = list->head;
        while (list->pool[cur].next != SLIST_NULL) {
            cur = list->pool[cur].next;
        }
        list->pool[cur].next = idx;
    }
    list->size++;
    return 1;
}

int slist_insert_at(StaticList *list, int pos, Student s) {
    if (!list || pos < 0 || pos > list->size) return 0;
    if (pos == 0) return slist_insert_front(list, s);

    int idx = slist_alloc(list);
    if (idx == SLIST_NULL) return 0;

    list->pool[idx].data = s;

    int cur = list->head;
    for (int i = 0; i < pos - 1; i++) {
        cur = list->pool[cur].next;
    }
    list->pool[idx].next = list->pool[cur].next;
    list->pool[cur].next = idx;
    list->size++;
    return 1;
}

/* ------------------------------------------------------------ *
 *  Remocao
 * ------------------------------------------------------------ */
int slist_remove_by_id(StaticList *list, int id) {
    if (!list || list->head == SLIST_NULL) return 0;

    int prev = SLIST_NULL;
    int cur  = list->head;

    while (cur != SLIST_NULL) {
        if (list->pool[cur].data.id == id) {
            if (prev == SLIST_NULL) {
                list->head = list->pool[cur].next;
            } else {
                list->pool[prev].next = list->pool[cur].next;
            }
            slist_dealloc(list, cur);
            list->size--;
            return 1;
        }
        prev = cur;
        cur  = list->pool[cur].next;
    }
    return 0;
}

/* ------------------------------------------------------------ *
 *  Busca
 * ------------------------------------------------------------ */
int slist_search_by_id(const StaticList *list, int id) {
    if (!list) return SLIST_NULL;
    int cur = list->head;
    while (cur != SLIST_NULL) {
        if (list->pool[cur].data.id == id) return cur;
        cur = list->pool[cur].next;
    }
    return SLIST_NULL;
}

int slist_search_by_name(const StaticList *list, const char *name) {
    if (!list || !name) return SLIST_NULL;
    int cur = list->head;
    while (cur != SLIST_NULL) {
        if (strstr(list->pool[cur].data.name, name) != NULL) return cur;
        cur = list->pool[cur].next;
    }
    return SLIST_NULL;
}

/* ------------------------------------------------------------ *
 *  Impressao
 * ------------------------------------------------------------ */
void slist_print(const StaticList *list) {
    if (!list) return;
    printf("=== Lista Encadeada Estatica (%d alunos) ===\n", list->size);
    int cur = list->head;
    int i   = 1;
    while (cur != SLIST_NULL) {
        printf("[%d] ", i++);
        student_print(&list->pool[cur].data);
        cur = list->pool[cur].next;
    }
    printf("===========================================\n");
}

/* ------------------------------------------------------------ *
 *  Tamanho
 * ------------------------------------------------------------ */
int slist_size(const StaticList *list) {
    return list ? list->size : 0;
}

/* ------------------------------------------------------------ *
 *  Limpeza (nao libera memoria — reinicializa a free list)
 * ------------------------------------------------------------ */
void slist_clear(StaticList *list) {
    slist_init(list);
}

/* ------------------------------------------------------------ *
 *  Carga e exportacao
 * ------------------------------------------------------------ */
void slist_load_from_array(StaticList *list, const Student *arr, int n) {
    if (!list || !arr) return;
    slist_clear(list);
    for (int i = 0; i < n; i++) {
        slist_insert_back(list, arr[i]);
    }
}

Student *slist_to_array(const StaticList *list) {
    if (!list || list->size == 0) return NULL;

    Student *arr = (Student *)malloc(list->size * sizeof(Student));
    if (!arr) {
        fprintf(stderr, "[ERRO] slist_to_array: falha ao alocar\n");
        return NULL;
    }
    int cur = list->head;
    int i   = 0;
    while (cur != SLIST_NULL) {
        arr[i++] = list->pool[cur].data;
        cur = list->pool[cur].next;
    }
    return arr;
}
