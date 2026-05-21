#!/usr/bin/env bash

# Single C++ backend impulse test.
# Generates a .cmp file with `filesCompare` comparison
# and a .dif file with `diff` comparison.

set -u

NAME="${1:?usage: $0 <test-name>}"
FAUST_HOME="/Users/manuelfarzini/Personal/dev/repo/faust"

TMP="tmp"
ARCH_DIR="${FAUST_HOME}/tests/impulse-tests/archs"

FAUST="${FAUST_HOME}/build/bin/faust"
ARCH="${ARCH_DIR}/impulsearch.cpp"
SRC="${FAUST_HOME}/tests/impulse-tests/dsp/${NAME}.dsp"
REF="${FAUST_HOME}/tests/impulse-tests/reference/${NAME}.ir"
COMP="${FAUST_HOME}/tests/impulse-tests/filesCompare"

OUT="${TMP}/${NAME}.cpp"
BIN="${TMP}/${NAME}_cpp"
IR="${TMP}/${NAME}_cpp.ir"
CMP="${TMP}/${NAME}_cpp.cmp"
DIF="${TMP}/${NAME}_cpp.dif"

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
[[ -d "$ARCH_DIR" ]] || die "missing architecture dir: $ARCH_DIR"
[[ -f "$SRC" ]] || die "missing dsp file: $SRC"
[[ -f "$REF" ]] || die "missing reference file: $REF"
[[ -f "$ARCH" ]] || die "missing architecture file: $ARCH"
[[ -x "$COMP" ]] || die "missing filesCompare binary: $COMP"

"$FAUST" \
    -lang cpp \
    -I "${FAUST_HOME}/tests/impulse-tests/dsp" \
    -double \
    -i \
    -a "$ARCH" \
    "$SRC" \
    -o "$OUT" \
    || die "faust generation failed"

g++ \
    -O3 \
    -fwrapv \
    -std=gnu++17 \
    -I"$ARCH_DIR" \
    "$OUT" \
    -o "$BIN" \
    || die "C++ build failed"

"$BIN" > "$IR" 2>&1 \
    || die "generated executable failed"

echo "first file:  $REF"
echo "second file: $IR"

if ! "$COMP" "$REF" "$IR" > "$CMP" 2>&1; then
    diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

    echo "impulse test failed: $NAME" >&2
    echo "generated source: $OUT" >&2
    echo "compare output:   $CMP" >&2
    echo "diff output:      $DIF" >&2
    exit 1
fi

diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

echo "impulse test passed: $NAME"
echo "generated source: $OUT"
