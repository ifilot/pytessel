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

## Getting started

In the script below, the isosurface of a threedimensional Gaussian function is
constructed. The isosurface is written to `test.ply` and can, for example,
be opened using `ctmviewer` (Linux) or `3D viewer` (Windows).

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

## Isosurface quality

In the script below, 6 different images are created of an icosahedral "metaball" using a grid
of `10x10x10`,`20x20x20`,`25x25x25`,`50x50x50`,`100x100x100`, and `200x200x200` points. The
resulting `.ply` files are rendered using [Blender](https://www.blender.org/).

```python
from pytessel import PyTessel
import numpy as np

def main():
    """
    Build 6 .ply files of increasing quality
    """
    pytessel = PyTessel()

    for nrpoints in [10,20,25,50,100,200]:
        sz = 3

        x = np.linspace(-sz, sz, nrpoints)
        y = np.linspace(-sz, sz, nrpoints)
        z = np.linspace(-sz, sz, nrpoints)

        xx, yy, zz, field = icosahedron_field(x,y,z)

        unitcell = np.diag(np.ones(3) * sz * 2)
        pytessel = PyTessel()
        isovalue = 3.75
        vertices, normals, indices = pytessel.marching_cubes(field.flatten(), field.shape, unitcell.flatten(), isovalue)

        pytessel.write_ply('icosahedron_%03i.ply' % nrpoints, vertices, normals, indices)

def icosahedron_field(x,y,z):
    """
    Produce a scalar field for the icosahedral metaballs
    """
    phi = (1 + np.sqrt(5)) / 2
    vertices = [
        [0,1,phi],
        [0,-1,-phi],
        [0,1,-phi],
        [0,-1,phi],
        [1,phi,0],
        [-1,-phi,0],
        [1,-phi,0],
        [-1,phi,0],
        [phi,0,1],
        [-phi,0,-1],
        [phi,0,-1],
        [-phi,0,1]
    ]

    xx,yy,zz = np.meshgrid(x,y,z)
    field = np.zeros_like(xx)
    for v in vertices:
        field += f(xx,yy,zz,v[0], v[1],v[2])

    return xx,yy,zz,field

def f(x,y,z,X0,Y0,Z0):
    """
    Single metaball function
    """
    return 1 / ((x - X0)**2 + (y - Y0)**2 + (z - Z0)**2)

if __name__ == '__main__':
    main()
```

![Icosahedral metaballs](https://raw.githubusercontent.com/ifilot/pytessel/master/img/metaballs_3x2.png)

## Local compilation (Linux)

```bash
python3 setup.py build_ext --inplace bdist
```
