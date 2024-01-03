#!/bin/sh
#  SPDX-License-Identifier: GPL-3.0-only
#  Script to link binary for anvil
#    Copyright (C) 2023  jgabaut
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, version 3 of the License.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <https://www.gnu.org/licenses/>.

script_version="0.2"

print_impls() {
    printf "Impl list:\n"
    printf "  repo_amboso\n  repo_invil\n  path_anvil\n  cargo_invil\n\n"
}

prompt_impls() {
    hint="$1"
    if [ ! -z "$hint" ] ; then {
        printf "\nPress Enter to use {%s} as requested anvil impl or select anvil implementation:\n" "$hint"
    } else {
        printf "\nSelect anvil implementation:\n"
    }
    fi
    print_impls
    printf "Choice:  \n"
}

usage() {
    prog="$1"
    printf "Usage:    %s [-i] <impl>\n" "$prog"
    print_impls
}

if [ "$#" -gt 3 ] ; then {
    usage "$0"
    exit 1
}
fi

if [ -f "./anvil" ] ; then {
    printf "[INFO]    ./anvil ready\n"
    exit 0
}
fi

is_interactive=0

if [ "$1" = "-i" ] ; then {
    is_interactive=1
    printf "%s v%s\n" "$0" "$script_version"
}
fi

if [ "$is_interactive" -eq 0 ] ; then {
    impl_q="$1"
    if [ "$impl_q" = "repo_amboso" ] ; then {
        ln -s ./amboso/amboso ./anvil
        exit "$?"
    } elif [ "$impl_q" = "repo_invil" ] ; then {
        (cd invil || { printf "[ERROR]    Failed cd to invil\n"; exit 1; } ; cargo build --release)
        if [ "$?" -eq 0 ] ; then {
            ln -s ./invil/target/release/invil ./anvil
            exit "$?"
        } else {
            printf "[ERROR]    Failed building invil.\n"
            printf "[ERROR]    Fallback to repo_amboso.\n"
            exit "$("$0" repo_amboso)"
        }
        fi
    } elif [ "$impl_q" = "path_anvil" ] ; then {
        ln -s /usr/local/bin/anvil ./anvil
        exit "$?"
    } elif [ "$impl_q" = "cargo_invil" ] ; then {
        ln -s "$HOME"/.cargo/bin/invil ./anvil
        exit "$?"
    } else {
        printf "[ERROR]    Invalid impl query: {%s}.\n" "$impl_q"
        exit 1
    }
    fi
} else {
    while read -p "$(prompt_impls "$2")" line; do {
        if [ -z "$line" ] ; then {
            line="$2"
        }
        fi
        impl_q="$line"
        printf "[INFO]    Requested impl: {%s}\n" "$impl_q"
        if [ "$impl_q" = "repo_amboso" ] ; then {
            ln -s ./amboso/amboso ./anvil
            exit "$?"
        } elif [ "$impl_q" = "repo_invil" ] ; then {
            (cd invil || { printf "[ERROR]    Failed cd to invil\n"; exit 1; } ; cargo build --release)
            if [ "$?" -eq 0 ] ; then {
                ln -s ./invil/target/release/invil ./anvil
                exit "$?"
            } else {
                printf "[ERROR]    Failed building invil.\n"
                printf "[ERROR]    Fallback to repo_amboso.\n"
                exit "$("$0" repo_amboso)"
            }
            fi
        } elif [ "$impl_q" = "path_anvil" ] ; then {
            ln -s /usr/local/bin/anvil ./anvil
            exit "$?"
        } elif [ "$impl_q" = "cargo_invil" ] ; then {
            ln -s "$HOME"/.cargo/bin/invil ./anvil
            exit "$?"
        } else {
            printf "[ERROR]    Invalid impl query: {%s}.\n" "$impl_q"
        }
        fi
    }
    done
}
fi
