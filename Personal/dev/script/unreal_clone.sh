#!/bin/bash
set -euo pipefail

# --------- argomenti ---------------------------------------------------------
TEMPLATE_NAME="$1"      # ex: unreal_base
NEW_NAME="$2"           # ex: demo_1
# -----------------------------------------------------------------------------

# directory di lavoro (quella dove lanci lo script)
ROOT_DIR="$(pwd)"
TEMPLATE_DIR="$ROOT_DIR/$TEMPLATE_NAME"
DEST_DIR="$ROOT_DIR/$NEW_NAME"

if [[ ! -d "$TEMPLATE_DIR" ]]; then
  echo "Template project not found: $TEMPLATE_DIR"
  exit 1
fi
if [[ -e "$DEST_DIR" ]]; then
  echo "Destination already exists: $DEST_DIR"
  exit 1
fi

echo "📁  Cloning $TEMPLATE_DIR  →  $DEST_DIR"
cp -R "$TEMPLATE_DIR" "$DEST_DIR"


# ---------- 1) rinomina cartella del modulo C++ ------------------------------
if [[ -d "$DEST_DIR/Source/$TEMPLATE_NAME" ]]; then
  mv "$DEST_DIR/Source/$TEMPLATE_NAME" "$DEST_DIR/Source/$NEW_NAME"
fi

# ---------- 2) rinomina file che contengono il nome vecchio ------------------
echo "Renaming filenames…"
find "$DEST_DIR" -type f -name "*$TEMPLATE_NAME*" | while read -r FILE ; do
  NEW_FILE="$(echo "$FILE" | sed "s/$TEMPLATE_NAME/$NEW_NAME/g")"
  mv "$FILE" "$NEW_FILE"
done

# file di progetto principali
[[ -f "$DEST_DIR/$TEMPLATE_NAME.uproject"    ]] && mv "$DEST_DIR/$TEMPLATE_NAME.uproject"    "$DEST_DIR/$NEW_NAME.uproject"
[[ -f "$DEST_DIR/$TEMPLATE_NAME.sln"         ]] && mv "$DEST_DIR/$TEMPLATE_NAME.sln"         "$DEST_DIR/$NEW_NAME.sln"
[[ -d "$DEST_DIR/$TEMPLATE_NAME.xcworkspace" ]] && mv "$DEST_DIR/$TEMPLATE_NAME.xcworkspace" "$DEST_DIR/$NEW_NAME.xcworkspace"


# ---------- 3) sostituzione riferimenti interni nei file di testo ------------
echo "✏️  Rewriting textual references…"
find "$DEST_DIR" \( -name "*.cpp" -o -name "*.h" -o -name "*.cs" -o -name "*.ini" -o -name "*.uproject" \) \
     -exec sed -i '' -e "s/$TEMPLATE_NAME/$NEW_NAME/g" {} +

# ---------- 4) done ----------------------------------------------------------
echo "Clone complete."
echo "   New project: $DEST_DIR"
echo
echo "Per compilarlo:"
echo "/Users/Shared/Epic\\ Games/UE_5.2/Engine/Build/BatchFiles/Mac/Build.sh \\"
echo "  ${NEW_NAME}Editor Mac Development $DEST_DIR/${NEW_NAME}.uproject"

