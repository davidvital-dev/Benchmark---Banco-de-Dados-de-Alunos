# Banco de Dados de Alunos — Análise de Desempenho de Algoritmos de Ordenação

Trabalho da disciplina de **Estruturas de Dados** — Engenharia de Software  
Universidade Federal do Cariri (UFCA)  
Prof. Weskley Vinícius Fernandes Mauricio

## Repositório

https://github.com/davidvital-dev/Benchmark---Banco-de-Dados-de-Alunos

## Integrantes

- David Josué Vital Santos
- Ângelo Gabriel Alves Freire Duarte
- Jetro Kepler Gomes Alencar Gonzaga Viana
- José Luiz de Lima Mendes
- Dorian Dayvid Gomes Feitosa

## Descrição

Implementação e análise comparativa de cinco algoritmos de ordenação — Bubble Sort, Insertion Sort, Selection Sort, Quick Sort e Merge Sort — aplicados a um banco de dados de alunos sobre duas estruturas de dados: lista encadeada dinâmica e lista encadeada estática.

## Estrutura do Projeto

```
student-db/
├── src/
│   ├── student.h / student.c          # Estrutura Student e funções auxiliares
│   ├── dynamic_list.h / dynamic_list.c # Lista encadeada dinâmica
│   ├── static_list.h / static_list.c  # Lista encadeada estática
│   ├── sorting.h / sorting.c          # Algoritmos de ordenação
│   ├── benchmark.h / benchmark.c      # Módulo de benchmark
│   └── main.c                         # Ponto de entrada
├── results/
│   └── resultados_benchmark.csv       # Resultados dos experimentos
├── artigo/
│   └── artigo_final_v2.tex            # Artigo em formato IEEE
├── Makefile
├── compilar_e_rodar.bat
└── README.md
```

## Compilação e Execução

### Linux / macOS
```bash
make
./student_db
```

### Windows
```bash
compilar_e_rodar.bat
```

Ou manualmente:
```bash
gcc -Wall -O2 -I src src/student.c src/dynamic_list.c src/static_list.c src/sorting.c src/benchmark.c src/main.c -lm -o student_db
./student_db
```

## Experimentos

O benchmark executa automaticamente 90 experimentos:
- **5 algoritmos** × **2 estruturas** × **3 tamanhos** (N = 100, 1.000, 10.000) × **3 tipos de entrada** (Aleatório, Parcialmente Ordenado, Inversamente Ordenado)
- **100 repetições** por experimento, reportando a média

Os resultados são salvos em `results/resultados_benchmark.csv`.

## Geração dos Gráficos

Após executar o programa e gerar o CSV, rode o script Python para gerar os gráficos:

```bash
pip install pandas matplotlib
python graficos.py
```

Os 6 gráficos serão salvos automaticamente em `artigo/`.

## Resultados

O **Merge Sort** foi escolhido como algoritmo ideal, com tempo entre 1,4 ms e 2,9 ms para N = 10.000 independentemente do tipo de entrada. A lista estática apresentou ganhos de até 89% sobre a dinâmica para algoritmos O(n²).