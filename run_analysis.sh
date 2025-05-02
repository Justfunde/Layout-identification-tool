#!/bin/bash

ANALYSIS_TOOL="build/src/LitConsoleClient/LitConsoleClient"
LARGE_LAYOUTS_DIR="LargeLayouts"
CELLS_DIR="Cells"
RESULTS_DIR="AnalysisResult"

# Чтение списка файлов в массивы
LARGE_LAYOUT_FILES=("$LARGE_LAYOUTS_DIR"/*.gds)
CELL_FILES=("$CELLS_DIR"/*.gds)

# Подсчёт количества файлов
LARGE_LAYOUT_FILES_COUNT=${#LARGE_LAYOUT_FILES[@]}
CELL_FILES_COUNT=${#CELL_FILES[@]}

echo "Large layouts count: $LARGE_LAYOUT_FILES_COUNT"
echo "Cells count: $CELL_FILES_COUNT"

# Обработка каждого файла из large layouts
for ((i = 0; i < LARGE_LAYOUT_FILES_COUNT; i++)); do
    CURRENT_LARGE_LAYOUT="${LARGE_LAYOUT_FILES[i]}"
    CURRENT_RESULT_DIR="$RESULTS_DIR/$(basename "$CURRENT_LARGE_LAYOUT")"
    
    # Создаем директорию для результатов (если её ещё нет)
    mkdir -p "$CURRENT_RESULT_DIR"
    
    # Обработка каждого файла из cells
    for ((j = 0; j < CELL_FILES_COUNT; j++)); do
        CURRENT_CELL="${CELL_FILES[j]}"
        
        # Выполняем анализ и сохраняем вывод в файл с именем текущей ячейки
        "$ANALYSIS_TOOL" "$CURRENT_LARGE_LAYOUT" "$CURRENT_CELL" > "$CURRENT_RESULT_DIR/$(basename "$CURRENT_CELL")" 2>&1
    done
done
