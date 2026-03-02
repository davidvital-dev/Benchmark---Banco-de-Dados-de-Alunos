#ifndef STUDENT_H
#define STUDENT_H

/* ============================================================
 * student.h — Definição da estrutura de dados do aluno
 * Autor: David
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN   80
#define MAX_COURSE_LEN 50
#define MAX_MATRIC_LEN 12

/* Critérios de ordenação disponíveis */
typedef enum {
    SORT_BY_GPA        = 0,  /* Média acadêmica (crescente) */
    SORT_BY_GPA_DESC   = 1,  /* Média acadêmica (decrescente) */
    SORT_BY_NAME       = 2,  /* Nome (alfabético) */
    SORT_BY_AGE        = 3,  /* Idade (crescente) */
    SORT_BY_ID         = 4   /* Matrícula (crescente) */
} SortCriteria;

/* Tipos de entrada para os experimentos */
typedef enum {
    INPUT_RANDOM       = 0,
    INPUT_SORTED       = 1,  /* Parcialmente ordenado */
    INPUT_REVERSE      = 2   /* Inversamente ordenado (pior caso) */
} InputType;

/* ------------------------------------------------------------ *
 *  Estrutura principal: Aluno
 * ------------------------------------------------------------ */
typedef struct {
    int   id;                       /* Identificador único / matrícula */
    char  name[MAX_NAME_LEN];       /* Nome completo */
    float gpa;                      /* Coeficiente de Rendimento Acadêmico (0–10) */
    int   age;                      /* Idade */
    char  course[MAX_COURSE_LEN];   /* Curso */
} Student;

/* ------------------------------------------------------------ *
 *  Protótipos de funções
 * ------------------------------------------------------------ */

/* Gera um aluno com dados aleatórios */
Student student_random(int id);

/* Gera um array de N alunos segundo o tipo de entrada */
Student *student_generate_array(int n, InputType type);

/* Libera um array de alunos gerado dinamicamente */
void student_free_array(Student *arr);

/* Imprime um aluno (debug) */
void student_print(const Student *s);

/* Funções de comparação (retornam <0, 0 ou >0) */
int student_cmp_gpa(const Student *a, const Student *b);
int student_cmp_gpa_desc(const Student *a, const Student *b);
int student_cmp_name(const Student *a, const Student *b);
int student_cmp_age(const Student *a, const Student *b);
int student_cmp_id(const Student *a, const Student *b);

/* Ponteiro de função de comparação genérico */
typedef int (*CmpFunc)(const Student *, const Student *);

/* Retorna o comparador correspondente ao critério */
CmpFunc student_get_cmp(SortCriteria criteria);

/* Nomes legíveis (para logs/CSV) */
const char *sort_criteria_name(SortCriteria c);
const char *input_type_name(InputType t);

#endif /* STUDENT_H */
