#!/usr/bin/env bash
set -euo pipefail

MSG="${1:-Periodic update.}"

setup() {
    git --git-dir="$HOME/.setup" --work-tree="$HOME" "$@"
}

update_setup() {
    echo "==> setup"

    setup add -u

    if setup diff --cached --quiet; then
        echo "nothing to commit"
        return
    fi

    setup commit -m "$MSG"
    setup push origin main
}

update_repo() {
    local name="$1"
    local path="$2"

    echo "==> $name"

    if [ ! -d "$path/.git" ]; then
        echo "skip: $path is not a git repo"
        return
    fi

    git -C "$path" add -u

    if git -C "$path" diff --cached --quiet; then
        echo "nothing to commit"
        return
    fi

    git -C "$path" commit -m "$MSG"
    git -C "$path" push origin main
}

update_setup
update_repo "nvim" "$HOME/.config/nvim"
update_repo "libcx" "$HOME/Personal/dev/opt/libcx"
