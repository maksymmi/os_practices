#!/bin/zsh

print -P "%F{cyan}-> Починаємо збірку ~%f"

compile_target() {
    local source=$1
    local target=$2
    
    print -P "%F{yellow}Компіляція:%f $source -> $target"
    
    if g++ -O2 "$source" -o "$target"; then
        print -P "  %F{green} Зібрано%f"
    else
        print -P "  %F{red} Помилка $source%f"
        exit 1
    fi
}

compile_target "editor.cpp" "editor"
compile_target "analyzer.cpp" "analyzer"
compile_target "main.cpp" "main"
compile_target "snapshot.cpp" "snapshot"

print -P "Для запуску ->  %F{magenta}./main%f"
print -P "Для запуску знімку процесів (/proc): %F{magenta}./snapshot%f"