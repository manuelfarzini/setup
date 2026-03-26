#!/usr/bin/env bash

set -euo pipefail

BACKUP_DIR="$HOME/Personal/dev/dotfiles"

SOURCE_ITEMS=(
  # ~
  # "$HOME/.tmux"
  "$HOME/.clang-format"
  "$HOME/.clangd"
  "$HOME/.clang-tidy"
  "$HOME/.gitconfig"
  "$HOME/.gitignore_global"
  "$HOME/.hgignore_global"
  "$HOME/.zprofile"
  # ~/Personal/dev
  "$HOME/Personal/dev/bin"
  "$HOME/Personal/dev/fonts"
  "$HOME/Personal/dev/include"
  "$HOME/Personal/dev/misc"
  "$HOME/Personal/dev/utils"
  # ~/Personal/dev/opt
  "$HOME/Personal/dev/opt/libcx"
  # ~/.config
  "$HOME/.config/aerospace/"
  "$HOME/.config/ghostty/"
  "$HOME/.config/karabiner/"
  # "$HOME/.config/nvim/"
  "$HOME/.config/skhd/"
  "$HOME/.config/stylua/"
  # "$HOME/.config/tmux/"
  "$HOME/.config/yabai/"
  "$HOME/.config/yazi/"
  "$HOME/.config/zsh/"
  # Alfred
  "$HOME/Library/Application Support/Alfred/Alfred.alfredpreferences"
)

EXCLUDE_ITEMS=(
  "Fonts_Main"
  ## git
  ".git/"
  ".hgignore_global"
  ".gitignore_global"
  ## opt
  # "unreal_engine"
  "zzz"
  ## Misc
  "*.bak"
  "*.ssh"
  ".DS_Store"
  ## build
  "output"
  "build" 
  "doc"
  ".cache"
  # zsh
  ".zsh_sessions/"
  ".zcompcache/"
  ".zcompdump"
  ".zsh_history"
)

build_exclude_opts() {
  local opts=()
  for item in "${EXCLUDE_ITEMS[@]}"; do
    opts+=("--exclude=$item")
  done
  printf '%s\n' "${opts[@]}"
}

IFS=$'\n' read -r -d '' -a EXCLUDE_OPTS < <(build_exclude_opts && printf '\0')

CONFIG_ROOT="$HOME/.config"
DEV_ROOT="$HOME/Personal/dev"

for SRC_ITEM in "${SOURCE_ITEMS[@]}"; do
  if [[ -e "$SRC_ITEM" ]]; then
    # normalize: strip trailing slash
    src="${SRC_ITEM%/}"

    if [[ -d "$src" ]]; then
      # Decide DEST_DIR based on prefix
      if [[ "$src" == "$CONFIG_ROOT/"* || "$src" == "$CONFIG_ROOT" ]]; then
        # ~/.config/... -> $BACKUP_DIR/.config/...
        rel="${src#$CONFIG_ROOT/}"
        DEST_DIR="$BACKUP_DIR/.config/$rel"
      elif [[ "$src" == "$DEV_ROOT/"* || "$src" == "$DEV_ROOT" ]]; then
        # ~/Personal/dev/... -> $BACKUP_DIR/dev/...
        rel="${src#$DEV_ROOT/}"  # bin, opt/olib, ...
        DEST_DIR="$BACKUP_DIR/dev/$rel"
      else
        # everything else flattened
        DEST_DIR="$BACKUP_DIR/$(basename "$src")"
      fi

      mkdir -p "$DEST_DIR"
      rsync -au "${EXCLUDE_OPTS[@]}" "$src/" "$DEST_DIR/"

    else
      # single files still go directly in BACKUP_DIR
      cp -p "$src" "$BACKUP_DIR/"
    fi
  else
    echo "err: $SRC_ITEM does not exist."
  fi
done
 
# Remove some residual garbage
rm -rf "$BACKUP_DIR/olib/test/output"

echo "Backup done."


