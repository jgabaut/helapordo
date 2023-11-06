#!/bin/bash

#Check if status is clean
if output=$(git status --untracked-files=no --porcelain) && [ -z "$output" ]; then
	printf "\033[1;32m[OK]\e[0m    Status is clean\n"
else
	printf "\033[1;31m[ERR]\e[0m    Status is dirty. Quitting.\n"
	exit 1
fi
SRC_DIR="src"

hlpd_SOURCES=("$SRC_DIR"/*.c "$SRC_DIR"/*.h)
CONFIG_FILE=".astylerc"

printf "Running \033[1;34mastyle -s4 --style=linux\e[0m for all source files.\n"
for src_i in "${hlpd_SOURCES[@]}"; do {
    astyle --options="$CONFIG_FILE" $src_i >/dev/null
    printf "Ran astyle on [\033[1;35m$src_i\e[0m], status was [\033[1;33m$?\e[0m]\n"
}
done
printf "Removing all \033[1;34msrc/*.orig\e[0m files.\n"
for FILE in ${hlpd_SOURCES[@]}; do {
    if [[ -f "$FILE.orig" ]] ; then {
      rm "$FILE.orig" && printf "Removed [\033[1;36m$FILE.orig\e[0m]\n"
    } else {
      printf "[INFO]    File [\033[1;33m$FILE.orig\e[0m] not found.\n"
    }
    fi
}
done
printf "\033[1;33mDone.\e[0m\n"
