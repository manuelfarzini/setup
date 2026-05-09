# config
alias zp="nvim ~/.zprofile"
alias zs="nvim ~/.config/zsh/.zshrc"
alias vc="nvim ~/.config/nvim"
alias ghost="nvim ~/.config/ghostty"
alias setup='git --git-dir=$HOME/.setup --work-tree=$HOME'

# cli generic
alias cl="clear; clear; clear;"
alias md="mkdir"

# git
alias gl="git --no-pager log --graph --pretty=format:\"%h  %ad  %an %d\" --date=short --decorate"
alias glm="git --no-pager log --graph --decorate --date=short --pretty=format:'%C(auto)%h %C(bold blue)%ad%Creset %C(auto)%d%Creset %C(dim white)<%an>%Creset' --all"
alias glog="git --no-pager log"

# Eza
alias ls="eza --group-directories-first --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias ll="eza --long --group-directories-first --color=always  --no-filesize --icons=never --no-time --no-user --no-permissions"
alias la="eza --all --group-directories-first --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias lla="eza --long --all --group-directories-first --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias ld="eza -D --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias lld="eza -D --long --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias llad="eza -D --long --all --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias lf="eza -f --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias llf="eza -f --long --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias llaf="eza -f --long --all --color=always --no-filesize --icons=never --no-time --no-user --no-permissions"
alias lt1="eza --tree --level=1"
alias lt2="eza --tree --level=2"
alias lt3="eza --tree --level=3"

# python
alias python="python3"
alias pip="pip3"

# lazygit
alias lg="lazygit"

# tmux cht
alias cht="tmux-cht.sh"

# matlab script function
alias matfunc="matfunc.sh"

# cc project generator
alias ccinit="ccinit.sh"

# javaproject project generator
alias javaproj="javaproject.sh"

# backupscript
alias backupscript="backupscript.sh"

# zoxide
alias cd="z"

# g++ wrapper
alias gg="gg.sh"
alias ggg="ggg.sh"

# unreal engine build
alias uebuild='dotnet ~/personal/dev/opt/unreal_engine/UE_5.6/Engine/Binaries/DotNET/UnrealBuildTool/UnrealBuildTool.dll' 
