#!/bin/zsh

autoload -Uz vcs_info

zstyle ':vcs_info:*' enable git
zstyle ':vcs_info:git:*' check-for-changes true
zstyle ':vcs_info:git:*' stagedstr '+'
zstyle ':vcs_info:git:*' unstagedstr '*'
zstyle ':vcs_info:git:*' formats "%F{1}%b%u%c%{$reset_color%}"
setopt prompt_subst

from_home="%F{2}%~%{$reset_color%}"
carr_return=$'\n'
# chevron="$"
chevron=""
# chevron="@"
# chevron="ο"
cmd_prompt="%(?:%F{2}${chevron} :%F{1}${chevron} )"
git_info="\$vcs_info_msg_0_"

typeset -g LAST_DIR=""
function precmd() {
  if [[ $PWD != $LAST_DIR ]]; then
    vcs_info
    LAST_DIR=$PWD
  fi
}
precmd_functions+=( precmd )

PROMPT="${from_home} ${git_info}${carr_return}${cmd_prompt}"
