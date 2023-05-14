# PyTessel

[![C/C++ CI](https://github.com/ifilot/pytessel/actions/workflows/build.yml/badge.svg)](https://github.com/ifilot/pytessel/actions/workflows/build.yml)
[![Anaconda-Server Badge](https://anaconda.org/ifilot/pytessel/badges/version.svg)](https://anaconda.org/ifilot/pytessel)
[![PyPI](https://img.shields.io/pypi/v/pytessel?color=green)](https://pypi.org/project/pytessel/)
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

## Purpose

Python package for building isosurfaces from 3D scalar fields

## Installation

### Anaconda

[![Anaconda version](https://anaconda.org/ifilot/pytessel/badges/version.svg)](https://anaconda.org/ifilot/pytessel)
[![Anaconda last release](https://anaconda.org/ifilot/pytessel/badges/latest_release_date.svg)](https://anaconda.org/ifilot/pytessel)
[![Anaconda platforms](https://anaconda.org/ifilot/pytessel/badges/platforms.svg)](https://anaconda.org/ifilot/pytessel)
[![Anaconda downloads](https://anaconda.org/ifilot/pytessel/badges/downloads.svg)](https://anaconda.org/ifilot/pytessel)


```
conda install -c ifilot pyqint
```

### PyPi

[![PyPI](https://img.shields.io/pypi/v/pytessel?color=green)](https://pypi.org/project/pytessel/)
[![PyPI - Downloads](https://img.shields.io/pypi/dm/pypi)](https://pypi.org/project/pytessel/)
![PyPI - Python Version](https://img.shields.io/pypi/pyversions/pytessel)

```
pip install pytessel
```

## Usage

```python
from pytessel import PyTessel
import numpy as np

def main():
    pytessel = PyTessel()

    # generate some data
    x = np.linspace(0, 10, 50)
    # the grid is organized with z the slowest moving index and x the fastest moving index
    grid = np.flipud(np.vstack(np.meshgrid(x, x, x, indexing='ij')).reshape(3,-1)).T

    R = [5,5,5]
    scalarfield = np.reshape(np.array([gaussian(r,R) for r in grid]), (len(x),len(x),len(x)))
    unitcell = np.diag(np.ones(3) * 10.0)

    vertices, normals, indices = pytessel.marching_cubes(scalarfield.flatten(), scalarfield.shape, unitcell.flatten(), 0.1)

    pytessel.write_ply('test.ply', vertices, normals, indices)

def gaussian(r, R):
    return np.exp(-(r-R).dot((r-R)))

if __name__ == '__main__':
    main()
```

The isosurface is written to `test.ply` and can, for example, be opened using `ctmviewer`.

![Isosurface of Gaussian](img/ply_sphere.png)
