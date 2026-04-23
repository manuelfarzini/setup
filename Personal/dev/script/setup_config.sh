# Brew
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
eval "$(/opt/homebrew/bin/brew shellenv)"

# Environment
xcode-select --install
brew install --cask ghostty
brew install --cask aerospace
brew install nvim git llvm
