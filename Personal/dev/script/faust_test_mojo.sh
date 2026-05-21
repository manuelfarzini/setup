#!/usr/bin/env bash

# Single Mojo backend impulse test.
# Generates a .cmp file with `filesCompare` comparison
# and a .dif file with `diff` comparison.

set -u

NAME="${1:?usage: $0 <test-name>}"
FAUST_HOME="/Users/manuelfarzini/Personal/dev/repo/faust"

TMP="tmp"
ARCH_DIR="${FAUST_HOME}/tests/impulse-tests/archs/mojo"

FAUST="${FAUST_HOME}/build/bin/faust"
ARCH="${FAUST_HOME}/architecture/mojo/impulse.mojo"
SRC="${FAUST_HOME}/tests/impulse-tests/dsp/${NAME}.dsp"
REF="${FAUST_HOME}/tests/impulse-tests/reference/${NAME}.ir"
COMP="${FAUST_HOME}/tests/impulse-tests/filesCompare"

OUT="${ARCH_DIR}/${NAME}.mojo"
BIN="${TMP}/${NAME}_mojo"
IR="${TMP}/${NAME}_mojo.ir"
CMP="${TMP}/${NAME}_mojo.cmp"
DIF="${TMP}/${NAME}_mojo.dif"

die() {
    echo "error: $*" >&2
    exit 1
}

clean() {
    rm -f "$OUT"
    rm -f "$BIN"
    rm -f "$IR"
    rm -f "$CMP"
    rm -f "$DIF"
}

mkdir -p "$TMP"
clean

[[ -x "$FAUST" ]] || die "missing faust binary: $FAUST"
[[ -d "$ARCH_DIR" ]] || die "missing Mojo architecture dir: $ARCH_DIR"
[[ -f "$SRC" ]] || die "missing dsp file: $SRC"
[[ -f "$REF" ]] || die "missing reference file: $REF"
[[ -f "$ARCH" ]] || die "missing architecture file: $ARCH"
[[ -x "$COMP" ]] || die "missing filesCompare binary: $COMP"

"$FAUST" \
    -lang mojo \
    -I "${FAUST_HOME}/tests/impulse-tests/dsp" \
    -double \
    -i \
    -a "$ARCH" \
    "$SRC" \
    -o "$OUT" \
    || die "faust generation failed"

mojo build -O3 "$OUT" -o "$BIN" \
    || die "mojo build failed"

"$BIN" > "$IR" 2>&1 \
    || die "generated executable failed"

echo "first file:  reference"
echo "second file: candidate"

if ! "$COMP" "$REF" "$IR" > "$CMP" 2>&1; then
    diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

    echo "impulse test failed: $NAME" >&2
    echo "compare output: $CMP" >&2
    echo "diff output:    $DIF" >&2
    exit 1
fi

diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

echo "impulse test passed: $NAME"
