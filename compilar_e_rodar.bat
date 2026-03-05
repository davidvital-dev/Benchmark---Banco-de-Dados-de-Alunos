@echo off
echo.
echo ================================================
echo  Compilando Sistema de Banco de Dados de Alunos
echo ================================================
echo.

if not exist build mkdir build
if not exist results mkdir results

echo Compilando...
gcc -Wall -O2 -Isrc src/student.c src/dynamic_list.c src/static_list.c src/sorting.c src/benchmark.c src/main.c -o student_db.exe -lm

if %ERRORLEVEL% NEQ 0 (
    echo.
    echo ERRO: Compilacao falhou. Verifique se o gcc esta instalado.
    echo Baixe em: https://winlibs.com
    pause
    exit /b 1
)

echo Compilado com sucesso!
echo.
echo ================================================
echo  Executando Benchmark (pode demorar alguns min)
echo ================================================
echo.

student_db.exe --only-bench

echo.
echo CSV salvo em: results\resultados_benchmark.csv
echo.
pause
