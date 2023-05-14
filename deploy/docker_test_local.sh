#!/bin/bash

set -e -u -x

# Install packages and test
for PYBIN in /opt/python/cp3{7,8,9,10,11}-*/bin; do
    "${PYBIN}/python" -m pip install numpy nose
    "${PYBIN}/pip" install pytessel --no-index -f /io/wheelhouse
    (cd "$HOME"; "${PYBIN}/pytest" --verbose /io/tests/*.py)
done
