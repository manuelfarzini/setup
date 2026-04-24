NAME="$1"
FAUST="/Users/manuelfarzini/Personal/dev/repo/faust"

mkdir -p tmp

rm -f "tmp/${NAME}.mojo"
rm -f "tmp/${NAME}_mojo"
rm -f "tmp/${NAME}_mojo.ir"
rm -f "tmp/${NAME}_mojo.cmp"
rm -f "tmp/${NAME}_mojo.dif"

"${FAUST}/build/bin/faust" \
    -lang mojo -I dsp -double -i \
    -a "${FAUST}/tests/impulse-tests/archs/impulsemojo.mojo" \
    "${FAUST}/tests/impulse-tests/dsp/${NAME}.dsp" \
    -o "tmp/${NAME}.mojo" && \
mojo build -O3 "tmp/${NAME}.mojo" -o "tmp/${NAME}_mojo" && \
"./tmp/${NAME}_mojo" > "tmp/${NAME}_mojo.ir" 2>&1 && \
"${FAUST}/tests/impulse-tests/filesCompare" \
    "${FAUST}/tests/impulse-tests/reference/${NAME}.ir" \
    "tmp/${NAME}_mojo.ir" > "tmp/${NAME}_mojo.cmp" 2>&1 ; \
diff --width=500000 \
    "${FAUST}/tests/impulse-tests/reference/${NAME}.ir" \
    "tmp/${NAME}_mojo.ir" > "tmp/${NAME}_mojo.dif" 2>&1
