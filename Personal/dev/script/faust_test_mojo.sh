#!/usr/bin/env bash

# Single C++ / Mojo vector impulse test.
# Regenerates the custom C++ reference before comparing Mojo.

set -u

NAME="${1:?usage: $0 <test-name>}"
FAUST_HOME="/Users/manuelfarzini/Personal/dev/repo/faust"
CXX="${CXX:-clang++}"

TMP="tmp"
TESTS="${FAUST_HOME}/tests/impulse-tests"
ARCH_DIR="${FAUST_HOME}/architecture/mojo"
REF_DIR="${TESTS}/reference/_custom"

FAUST="${FAUST_HOME}/build/bin/faust"
ARCH_CPP="${TESTS}/archs/impulsearch.cpp"
ARCH_MOJO="${FAUST_HOME}/architecture/mojo/impulse.mojo"
SRC="${TESTS}/dsp/${NAME}.dsp"
REF="${REF_DIR}/${NAME}_vec4.ir"
COMP="${TESTS}/filesCompare"

CPP="${TMP}/${NAME}.cpp"
CPP_BIN="${TMP}/${NAME}_cpp"
CPP_LOG="${TMP}/${NAME}_cpp.log"
REF_TMP="${TMP}/${NAME}_vec4.ir.partial"

OUT="${TMP}/${NAME}.mojo"
BIN="${TMP}/${NAME}_mojo"
IR="${TMP}/${NAME}_mojo.ir"
LOG="${TMP}/${NAME}_mojo.log"
CMP="${TMP}/${NAME}_mojo.cmp"
DIF="${TMP}/${NAME}_mojo.dif"

FAUST_FLAGS=(
    -double
    -vec
    -vs 4
    -dfs
    -mcd 4
    -I "${TESTS}/dsp"
    -I "${FAUST_HOME}/libraries"
    -i
    -A "${FAUST_HOME}/architecture"
)

die() {
    echo "error: $*" >&2
    exit 1
}

[[ -x "$FAUST" ]] || die "missing faust binary: $FAUST"
[[ -d "$ARCH_DIR" ]] || die "missing Mojo architecture dir: $ARCH_DIR"
[[ -f "$SRC" ]] || die "missing dsp file: $SRC"
[[ -f "$ARCH_CPP" ]] || die "missing C++ architecture: $ARCH_CPP"
[[ -f "$ARCH_MOJO" ]] || die "missing Mojo architecture: $ARCH_MOJO"
[[ -x "$COMP" ]] || die "missing filesCompare binary: $COMP"
command -v "$CXX" >/dev/null || die "missing C++ compiler: $CXX"
command -v mojo >/dev/null || die "missing mojo compiler"

mkdir -p "$TMP" "$REF_DIR" || die "cannot create output directories"

# Remove previous test artifacts, keeping the existing reference.
rm -f -- "$OUT" "$BIN" "$IR" "$LOG" "$CMP" "$DIF" \
    "$CPP" "$CPP_BIN" "$CPP_LOG" "$REF_TMP" \
    || die "cannot remove previous test artifacts"

# Generate the C++ reference.
echo "generating C++ reference: $NAME"

"$FAUST" \
    -lang cpp \
    "${FAUST_FLAGS[@]}" \
    -a "$ARCH_CPP" \
    "$SRC" \
    -o "$CPP" \
    || die "C++ generation failed"

"$CXX" \
    -std=gnu++23 \
    -O3 \
    -fwrapv \
    -pthread \
    -DFAUSTFLOAT=float \
    -I "${FAUST_HOME}/architecture" \
    -I "${TESTS}/archs" \
    -I /usr/local/include/ap_fixed \
    "$CPP" \
    -o "$CPP_BIN" \
    || die "C++ build failed"

"$CPP_BIN" -n 60000 > "$REF_TMP" 2> "$CPP_LOG" \
    || die "C++ executable failed; see $CPP_LOG"

[[ -s "$REF_TMP" ]] || die "C++ generated an empty reference"

# Replace the reference only after successful execution.
mv -f "$REF_TMP" "$REF" || die "cannot save reference: $REF"
echo "reference saved: $REF"

# Generate and run the Mojo candidate.
echo "generating Mojo candidate: $NAME"

"$FAUST" \
    -lang mojo \
    "${FAUST_FLAGS[@]}" \
    -a "$ARCH_MOJO" \
    "$SRC" \
    -o "$OUT" \
    || die "Mojo generation failed"

mojo build \
    -O3 \
    -D DFAUST=DType.float32 \
    -I "$ARCH_DIR" \
    "$OUT" \
    -o "$BIN" \
    || die "Mojo build failed"

"$BIN" > "$IR" 2> "$LOG" \
    || die "Mojo executable failed; see $LOG"

echo "first file:  C++ reference"
echo "second file: Mojo candidate"

if ! "$COMP" "$REF" "$IR" > "$CMP" 2>&1; then
    diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

    echo "impulse test failed: $NAME" >&2
    echo "compare output: $CMP" >&2
    echo "diff output:    $DIF" >&2
    exit 1
fi

diff --width=500000 "$REF" "$IR" > "$DIF" 2>&1 || true

echo "impulse test passed: $NAME"
