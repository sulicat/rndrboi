#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

SRC_DIR=$1
DEST_DIR=$2

if [ ! -d "$DEST_DIR" ]; then
    mkdir $DEST_DIR
fi

rm -rf $DEST_DIR/*

for filepath in $SRC_DIR*; do
    filename="${filepath##*/}"
    ${SCRIPT_DIR}/third_party/glslc/glslc $filepath -o ${DEST_DIR}${filename}.spv
done
