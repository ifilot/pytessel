#!/bin/bash

set -e -u -x

# Install packages and test
for PYBIN in /opt/python/cp3[7,8,9]*/bin; do
    "${PYBIN}/python" -m pip install numpy nose
    "${PYBIN}/pip" install pytessel --no-index -f ./wheelhouse
    ("${PYBIN}/pytest" --verbose ./tests/*.py)
done
