#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:${SCRIPT_DIR}/lib/shared/
$@
