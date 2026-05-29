# Yazi navigation
function y() 
{
	local tmp="$(mktemp -t "yazi-cwd.XXXXXX")" cwd
	yazi "$@" --cwd-file="$tmp"
	if cwd="$(command cat -- "$tmp")" && [ -n "$cwd" ] && [ "$cwd" != "$PWD" ]; then
		builtin cd -- "$cwd"
	fi
	rm -f -- "$tmp"
}

# Update dotfiles and configurations
setup_update()
{
    local msg="${1:-Periodic update.}"
    local nvim="$HOME/.config/nvim"
    setup_git() { git --git-dir="$HOME/.setup" --work-tree="$HOME" "$@" }
    git -C "$nvim" add .
    git -C "$nvim" diff --cached --quiet || git -C "$nvim" commit -m "$msg"
    git -C "$nvim" push
    setup_git add -u
    setup_git diff --cached --quiet || setup_git commit -m "$msg"
    setup_git push
}
