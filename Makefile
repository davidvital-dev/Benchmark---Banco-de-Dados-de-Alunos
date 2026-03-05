# ============================================================
# Makefile — Sistema de Banco de Dados de Alunos
# Compativel com Windows (MinGW) e Linux/macOS
# Autor: Dorian
# ============================================================

CC      = gcc
CFLAGS  = -Wall -Wextra -O2 -std=c11 -Isrc
LDFLAGS = -lm

# ============================================================
#  Detecta o sistema operacional
# ============================================================
ifeq ($(OS), Windows_NT)
    TARGET   = student_db.exe
    RM       = del /Q
    RMDIR    = rmdir /S /Q
    MKDIR    = mkdir
    SLASH    = \\
    # Verifica se o diretorio ja existe antes de criar
    MKDIR_BUILD   = if not exist build $(MKDIR) build
    MKDIR_RESULTS = if not exist results $(MKDIR) results
else
    TARGET   = student_db
    RM       = rm -f
    RMDIR    = rm -rf
    MKDIR    = mkdir -p
    SLASH    = /
    MKDIR_BUILD   = $(MKDIR) build
    MKDIR_RESULTS = $(MKDIR) results
endif

# ============================================================
SRCDIR = src
OBJDIR = build
RESDIR = results

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# ============================================================
.PHONY: all clean run bench demo dirs

all: dirs $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)
	@echo ""
	@echo ">>> Compilacao concluida: $(TARGET)"
	@echo "    Uso: ./$(TARGET)              (demo + benchmark)"
	@echo "         ./$(TARGET) --only-demo  (apenas demonstracao)"
	@echo "         ./$(TARGET) --only-bench (apenas benchmark)"

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

dirs:
	$(MKDIR_BUILD)
	$(MKDIR_RESULTS)

run: all
	./$(TARGET)

bench: all
	./$(TARGET) --only-bench

demo: all
	./$(TARGET) --only-demo

clean:
ifeq ($(OS), Windows_NT)
	@if exist build $(RMDIR) build
	@if exist $(TARGET) $(RM) $(TARGET)
else
	$(RMDIR) build $(TARGET)
endif
	@echo ">>> Limpeza concluida."

distclean: clean
ifeq ($(OS), Windows_NT)
	@if exist results\resultados_benchmark.csv $(RM) results\resultados_benchmark.csv
else
	$(RM) $(RESDIR)/*.csv
endif
