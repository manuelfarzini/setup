#!/usr/bin/env bash
set -euo pipefail

setup() {
    git --git-dir="$HOME/.setup" --work-tree="$HOME" "$@"
}

pull_setup() {
    echo "==> setup"
    setup pull --ff-only origin main
}

pull_repo() {
    local name="$1"
    local path="$2"

    echo "==> $name"

    if ! git -C "$path" rev-parse --git-dir &>/dev/null; then
        echo "skip: $path is not a git repo"
        return
    fi

    git -C "$path" pull --ff-only origin main
}

pull_setup
pull_repo "nvim" "$HOME/.config/nvim"
pull_repo "libcx" "$HOME/Personal/dev/opt/libcx"
