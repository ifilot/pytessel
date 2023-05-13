#!/bin/bash

# clean any earlier distributions
rm -rvf build/*
rm -vf dist/*.whl wheelhouse/*.whl
rm -rvf *.egg-info

# set path to root
ROOT='//d//PROGRAMMING//PYTHON//pyqint'
IMAGE='pytessel2014'

# run compilation inside Docker
winpty docker run -i -t -v $ROOT://io -w //io $IMAGE .//docker//docker_run_pypi.sh
