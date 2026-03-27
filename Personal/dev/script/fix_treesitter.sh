#!/usr/bin/env bash
set -euo pipefail

CPP_FOLDS="$HOME/.local/share/nvim/lazy/nvim-treesitter/runtime/queries/cpp/folds.scm"
C_FOLDS="$HOME/.local/share/nvim/lazy/nvim-treesitter/runtime/queries/c/folds.scm"

# Remove the exact line "  (namespace_definition)" from cpp folds.scm
if [[ -f "$CPP_FOLDS" ]]; then
  perl -0777 -i -pe 's/^\h*\(namespace_definition\)\R//m' "$CPP_FOLDS"
else
  echo "Missing: $CPP_FOLDS" >&2
fi

# Remove the exact line "  (preproc_ifdef)" from c folds.scm
if [[ -f "$C_FOLDS" ]]; then
  perl -0777 -i -pe 's/^\h*\(preproc_ifdef\)\R//m' "$C_FOLDS"
else
  echo "Missing: $C_FOLDS" >&2
fi

