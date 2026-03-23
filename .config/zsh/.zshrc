### Zap installer
[ -f "${XDG_DATA_HOME:-$HOME/.local/share}/zap/zap.zsh" ] && source "${XDG_DATA_HOME:-$HOME/.local/share}/zap/zap.zsh"

### Core sources
plug "$HOME/.config/zsh/core.zsh"
plug "$HOME/.config/zsh/aliases.zsh"
plug "$HOME/.config/zsh/exports.zsh"
plug "$HOME/.config/zsh/functions.zsh"
plug "$HOME/.config/zsh/prompt.zsh"
plug "$HOME/.config/zsh/completions.zsh"

### Colors
autoload -Uz colors && colors

### Plugins
plug "zap-zsh/zsh-autosuggestions"
plug "zap-zsh/zsh-syntax-highlighting"
plug "zap-zsh/vim"
plug "zap-zsh/fzf"

### Syntax Highlighting
typeset -gA ZSH_HIGHLIGHT_STYLES
ZSH_HIGHLIGHT_STYLES[unknown-token]='fg=red'

### History Substring Search
typeset -g HISTORY_SUBSTRING_SEARCH_HIGHLIGHT_FOUND='fg=#7b8b07,bg=#f7f7f1,bold'

### Completions
zstyle ':completion:*' matcher-list '' 'm:{a-zA-Z}={A-Za-z}' 'r:|[._-]=* r:|=*' 'l:|=* r:|=*'

### Zoxide
eval "$(zoxide init zsh)"

### Fzf
source <(fzf --zsh)
autoload -U compinit; compinit
source ~/.config/zsh/plugins/fzf-tab/fzf-tab.plugin.zsh
source ~/.config/zsh/plugins/fzf-tab-source/fzf-tab-source.plugin.zsh
export FZF_DEFAULT_OPTS="
  --color=fg:#1f2333,bg:#f7f7f1,hl:#c78707
  --color=fg+:#1f2333,bg+:#f7f7f1,hl+:#7b8b07
  --color=info:#c78707,prompt:#7b8b07,pointer:#c78707
  --color=marker:#c78707,spinner:#c78707,header:#c78707"

### Bindings
bindkey -M viins -r '^[k'
bindkey -M viins -r '^[j'
bindkey -M vicmd -r '^[k'
bindkey -M vicmd -r '^[j'
bindkey -M viins '^[k' history-beginning-search-backward
bindkey -M viins '^[j' history-beginning-search-forward
bindkey -M vicmd '^[k' history-beginning-search-backward
bindkey -M vicmd '^[j' history-beginning-search-forward

### Pyenv
export PYENV_ROOT="$HOME/.pyenv"
command -v pyenv >/dev/null || export PATH="$PATH:$PYENV_ROOT/bin"
eval "$(pyenv init -)"

### Sdkman: must be at EOF
export SDKMAN_DIR="$HOME/.sdkman"
[[ -s "$HOME/.sdkman/bin/sdkman-init.sh" ]] && source "$HOME/.sdkman/bin/sdkman-init.sh"
