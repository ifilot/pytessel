PyTessel: isosurface generation tool
====================================

.. image:: https://img.shields.io/pypi/v/pytessel?color=green
   :target: https://pypi.org/project/pytessel/
.. image:: https://img.shields.io/badge/License-GPLv3-blue.svg
   :target: https://www.gnu.org/licenses/gpl-3.0
.. image:: https://github.com/ifilot/pytessel/actions/workflows/build_wheels.yml/badge.svg
   :target: https://github.com/ifilot/pytessel/actions/workflows/build_wheels.yml

:program:`PyTessel` is a Python package for building isosurfaces from 3D scalar
fields. It is originally designed to create isosurfaces of (molecular) orbitals
and electron densities, but is agnotistic with respect to data input. The
majority of the instructions of :program:`PyTessel` are coded in C++ and are
coupled to a number of Python routines using the `Cython <https://cython.org/>`_
extension. :program:`PyTessel` makes use of shared-memory parallelization
via `OpenMP <https://www.openmp.org/>`_ ensuring efficient use is made of
trivial parallelization strategies. For more information about the algorithm,
please consult the :ref:`background` section.

To construct an isosurface, :program:`PyTessel` requires a scalar field and a
description of the unit cell wherein the scalar field resides. A very succinct
example of its operation is illustrated by the script below

.. code:: python

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

The isosurface is represented by thee arrays, containing vertex coordinates,
the normals at those vertices and a list of indices containing the polygons
(here triangles) that compose the isosurface. :program:`PyTessel` offers the
option to save the isosurface as a `.ply file <https://en.wikipedia.org/wiki/PLY_(file_format)>`_.

:program:`PyTessel` has been developed at the Eindhoven University of Technology,
Netherlands. :program:`PyTessel` and its development are hosted on `github
<https://github.com/ifilot/pytessel>`_.  Bugs and feature
requests are ideally submitted via the `github issue tracker
<https://github.com/ifilot/pytessel/issues>`_.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   installation
   background
   user_interface
   examples
   community_guidelines

Indices and tables
------------------

* :ref:`genindex`
* :ref:`search`
