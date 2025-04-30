#!/bin/sh

REF=$1
INPUT=$2
OUTPUT=$3

echo '~~~diff' > "${OUTPUT}"
diff -u "${REF}" "${INPUT}" >> "${OUTPUT}"
echo '~~~' >> "${OUTPUT}"