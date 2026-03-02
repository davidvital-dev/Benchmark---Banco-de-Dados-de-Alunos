/* ============================================================
 * student.c — Implementacao da estrutura de dados do aluno
 * Autor: David
 * ============================================================ */

#include "student.h"
#include <time.h>
#include <math.h>

/* ------------------------------------------------------------ *
 *  Dados estaticos para geracao aleatoria
 * ------------------------------------------------------------ */
static const char *FIRST_NAMES[] = {
    "Ana", "Bruno", "Carlos", "Diana", "Eduardo",
    "Fernanda", "Gabriel", "Helena", "Igor", "Julia",
    "Kevin", "Laura", "Marcos", "Natalia", "Otavio",
    "Patricia", "Rafael", "Sofia", "Thiago", "Valentina",
    "Wallace", "Ximena", "Yuri", "Zara", "Artur",
    "Beatriz", "Caua", "Debora", "Emanuel", "Fabio"
};
static const int N_FIRST = 30;

static const char *LAST_NAMES[] = {
    "Silva", "Santos", "Oliveira", "Souza", "Rodrigues",
    "Ferreira", "Alves", "Pereira", "Lima", "Gomes",
    "Costa", "Ribeiro", "Martins", "Carvalho", "Almeida",
    "Lopes", "Sousa", "Fernandes", "Vieira", "Barbosa"
};
static const int N_LAST = 20;

static const char *COURSES[] = {
    "Ciencia da Computacao",
    "Engenharia de Software",
    "Sistemas de Informacao",
    "Engenharia da Computacao",
    "Analise e Des. de Sistemas"
};
static const int N_COURSES = 5;

/* ------------------------------------------------------------ *
 *  Utilitarios internos
 * ------------------------------------------------------------ */
static float rand_float(float min, float max) {
    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}

static int rand_int(int min, int max) {
    return min + rand() % (max - min + 1);
}

/* ------------------------------------------------------------ *
 *  Geracao de aluno aleatorio
 * ------------------------------------------------------------ */
Student student_random(int id) {
    Student s;
    s.id  = id;
    s.age = rand_int(17, 35);
    s.gpa = roundf(rand_float(0.0f, 10.0f) * 10.0f) / 10.0f;

    /* Nome: primeiro + ultimo */
    snprintf(s.name, MAX_NAME_LEN, "%s %s",
             FIRST_NAMES[rand() % N_FIRST],
             LAST_NAMES[rand() % N_LAST]);

    strncpy(s.course, COURSES[rand() % N_COURSES], MAX_COURSE_LEN - 1);
    s.course[MAX_COURSE_LEN - 1] = '\0';

    return s;
}

/* ------------------------------------------------------------ *
 *  Geracao de array segundo o tipo de entrada
 * ------------------------------------------------------------ */
Student *student_generate_array(int n, InputType type) {
    if (n <= 0) return NULL;

    Student *arr = (Student *)malloc(n * sizeof(Student));
    if (!arr) {
        fprintf(stderr, "[ERRO] Falha ao alocar array de alunos (n=%d)\n", n);
        exit(EXIT_FAILURE);
    }

    /* Gera n alunos com IDs unicos */
    for (int i = 0; i < n; i++) {
        arr[i] = student_random(i + 1);
    }

    /* Ajusta conforme o tipo de entrada */
    if (type == INPUT_SORTED) {
        /*
         * Parcialmente ordenado: ordena por GPA e embaralha ~10% dos elementos.
         * Representa uma lista com pequenas atualizacoes recentes.
         */
        /* Ordenacao simples por GPA para criar base parcialmente ordenada */
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                if (arr[i].gpa > arr[j].gpa) {
                    Student tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }
            }
        }
        /* Embaralha ~10% como "novos cadastros" */
        int swaps = n / 10 > 0 ? n / 10 : 1;
        for (int k = 0; k < swaps; k++) {
            int i = rand() % n;
            int j = rand() % n;
            Student tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    } else if (type == INPUT_REVERSE) {
        /*
         * Inversamente ordenado: maior GPA primeiro.
         * Pior caso para varios algoritmos de ordenacao.
         */
        for (int i = 0; i < n - 1; i++) {
            for (int j = i + 1; j < n; j++) {
                if (arr[i].gpa < arr[j].gpa) {
                    Student tmp = arr[i];
                    arr[i] = arr[j];
                    arr[j] = tmp;
                }
            }
        }
    }
    /* INPUT_RANDOM: ja esta aleatorio */

    return arr;
}

/* ------------------------------------------------------------ *
 *  Liberacao de memoria
 * ------------------------------------------------------------ */
void student_free_array(Student *arr) {
    free(arr);
}

/* ------------------------------------------------------------ *
 *  Debug / impressao
 * ------------------------------------------------------------ */
void student_print(const Student *s) {
    if (!s) return;
    printf("[%06d] %-30s | GPA: %4.1f | Idade: %2d | %s\n",
           s->id, s->name, s->gpa, s->age, s->course);
}

/* ------------------------------------------------------------ *
 *  Funcoes de comparacao
 * ------------------------------------------------------------ */
int student_cmp_gpa(const Student *a, const Student *b) {
    if (a->gpa < b->gpa) return -1;
    if (a->gpa > b->gpa) return  1;
    return 0;
}

int student_cmp_gpa_desc(const Student *a, const Student *b) {
    return student_cmp_gpa(b, a);
}

int student_cmp_name(const Student *a, const Student *b) {
    return strcmp(a->name, b->name);
}

int student_cmp_age(const Student *a, const Student *b) {
    return a->age - b->age;
}

int student_cmp_id(const Student *a, const Student *b) {
    return a->id - b->id;
}

CmpFunc student_get_cmp(SortCriteria criteria) {
    switch (criteria) {
        case SORT_BY_GPA:       return student_cmp_gpa;
        case SORT_BY_GPA_DESC:  return student_cmp_gpa_desc;
        case SORT_BY_NAME:      return student_cmp_name;
        case SORT_BY_AGE:       return student_cmp_age;
        case SORT_BY_ID:        return student_cmp_id;
        default:                return student_cmp_gpa;
    }
}

/* ------------------------------------------------------------ *
 *  Nomes para logs
 * ------------------------------------------------------------ */
const char *sort_criteria_name(SortCriteria c) {
    switch (c) {
        case SORT_BY_GPA:      return "GPA_ASC";
        case SORT_BY_GPA_DESC: return "GPA_DESC";
        case SORT_BY_NAME:     return "NAME";
        case SORT_BY_AGE:      return "AGE";
        case SORT_BY_ID:       return "ID";
        default:               return "UNKNOWN";
    }
}

const char *input_type_name(InputType t) {
    switch (t) {
        case INPUT_RANDOM:  return "Aleatorio";
        case INPUT_SORTED:  return "Parcialmente_Ordenado";
        case INPUT_REVERSE: return "Inversamente_Ordenado";
        default:            return "UNKNOWN";
    }
}
