/* ============================================================
 * dynamic_list.c — Lista Duplamente Encadeada Dinamica
 * Autor: Luiz
 * ============================================================ */

#include "dynamic_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------ */
void dlist_init(DynamicList *list) {
    if (!list) return;
    list->head = NULL;
    list->size = 0;
}

static DNode *dnode_new(Student s) {
    DNode *node = (DNode *)malloc(sizeof(DNode));
    if (!node) {
        fprintf(stderr, "[ERRO] dlist: falha ao alocar no\n");
        return NULL;
    }
    node->data = s;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

/* ------------------------------------------------------------ */
int dlist_insert_front(DynamicList *list, Student s) {
    if (!list) return 0;
    DNode *node = dnode_new(s);
    if (!node) return 0;

    node->next = list->head;
    node->prev = NULL;
    if (list->head) list->head->prev = node;
    list->head = node;
    list->size++;
    return 1;
}

int dlist_insert_back(DynamicList *list, Student s) {
    if (!list) return 0;
    DNode *node = dnode_new(s);
    if (!node) return 0;

    if (!list->head) {
        list->head = node;
    } else {
        DNode *cur = list->head;
        while (cur->next) cur = cur->next;
        cur->next  = node;
        node->prev = cur;
    }
    list->size++;
    return 1;
}

int dlist_insert_at(DynamicList *list, int pos, Student s) {
    if (!list || pos < 0 || pos > list->size) return 0;
    if (pos == 0) return dlist_insert_front(list, s);

    DNode *node = dnode_new(s);
    if (!node) return 0;

    DNode *cur = list->head;
    for (int i = 0; i < pos - 1; i++) cur = cur->next;

    node->next = cur->next;
    node->prev = cur;
    if (cur->next) cur->next->prev = node;
    cur->next = node;
    list->size++;
    return 1;
}

/* ------------------------------------------------------------ */
int dlist_remove_by_id(DynamicList *list, int id) {
    if (!list || !list->head) return 0;
    DNode *cur = list->head;
    while (cur) {
        if (cur->data.id == id) {
            if (cur->prev) cur->prev->next = cur->next;
            else           list->head      = cur->next;
            if (cur->next) cur->next->prev = cur->prev;
            free(cur);
            list->size--;
            return 1;
        }
        cur = cur->next;
    }
    return 0;
}

/* ------------------------------------------------------------ */
DNode *dlist_search_by_id(const DynamicList *list, int id) {
    if (!list) return NULL;
    DNode *cur = list->head;
    while (cur) {
        if (cur->data.id == id) return cur;
        cur = cur->next;
    }
    return NULL;
}

DNode *dlist_search_by_name(const DynamicList *list, const char *name) {
    if (!list || !name) return NULL;
    DNode *cur = list->head;
    while (cur) {
        if (strstr(cur->data.name, name)) return cur;
        cur = cur->next;
    }
    return NULL;
}

/* ------------------------------------------------------------ */
void dlist_print(const DynamicList *list) {
    if (!list) return;
    printf("=== Lista Duplamente Encadeada Dinamica (%d alunos) ===\n", list->size);
    DNode *cur = list->head;
    int i = 1;
    while (cur) {
        printf("[%d] ", i++);
        student_print(&cur->data);
        cur = cur->next;
    }
    printf("=====================================================\n");
}

int dlist_size(const DynamicList *list) {
    return list ? list->size : 0;
}

void dlist_free(DynamicList *list) {
    if (!list) return;
    DNode *cur = list->head;
    while (cur) {
        DNode *next = cur->next;
        free(cur);
        cur = next;
    }
    list->head = NULL;
    list->size = 0;
}

/* ------------------------------------------------------------ */
void dlist_load_from_array(DynamicList *list, const Student *arr, int n) {
    if (!list || !arr) return;
    dlist_free(list);
    dlist_init(list);
    for (int i = 0; i < n; i++) dlist_insert_back(list, arr[i]);
}

Student *dlist_to_array(const DynamicList *list) {
    if (!list || list->size == 0) return NULL;
    Student *arr = (Student *)malloc(list->size * sizeof(Student));
    if (!arr) return NULL;
    DNode *cur = list->head;
    int i = 0;
    while (cur) { arr[i++] = cur->data; cur = cur->next; }
    return arr;
}
