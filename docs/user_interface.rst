.. _user-interface:
.. index:: userinterface

User Interface
==============

:program:`PyTessel` uses two functions for the construction of isosurfaces:

* :code:`marching_cubes`
* :code:`write_ply`

Isosurface generation
---------------------

First, the isosurface needs to be generated. This is done via the :code:`marching_cubes`
function, which takes four arguments as its input

.. list-table:: Arguments of the :code:`maching_cubes` function
   :widths: 25 25 50
   :header-rows: 1

   * - Argument
     - Type
     - Description
   * - :code:`grid`
     - Numpy array
     - Scalar field encoded in a one-dimensional grid.
   * - :code:`dimensions`
     - Numpy array
     - Dimensions (nx,ny,nz)
   * - :code:`unitcell`
     - 3x3 numpy array
     - Unitcell matrix
   * - :code:`isovalue`
     - Float
     - Isovalue

.. important::

    * The scalar field needs to be encoded such that the z-coordinate is the slowest moving
      index and the x-coordinate the fastest moving index.
    * The dimensions of the scalar field are encoded in the order :code:`(nx, ny, nz)`.
    * You might be tempted to use `scalarfield.shape` to pass the dimensionality for the
      grid, but **do not do this**. The order of `scalarfield.shape` should first be
      reversed.

The output of the :code:`marching_cubes` function are three arrays, corresponding to the
vertices, the normals, and the indices of the isosurface. The isosurface corresponds to
a number of linked triangles (polygons) whose vertices are stored in the :code:`vertices`
array. For each vertex, the normal vector is encoded in the :code:`normals` array. Finally,
the triangles are stored as a triplet of indices in the :code:`indices` array. These indices
refer to the position in the :code:`vertices` and :code:`normals` array. Because multiple
triangles can use the same vertices, this is an efficient way to store the isosurface.

.. note ::

    One rarely needs to perform any operations on the :code:`vertices`, :code:`normals` and
    :code:`indices` arrays. Typically, these arrays are constructed and immediately relayed
    to the :code:`write_ply` function to store them as a file which can be used in another
    program. 

Storing the isosurface
----------------------

The isosurfaces as encoded by the :code:`vertices`, :code:`normals` and
:code:`indices` arrays can be stored as a :code:`.ply` file by means of the
:code:`write_ply` function. This function is straightforward to use and uses
the arguments as shown in the table below.

.. list-table:: Arguments of the :code:`write_ply` function
   :widths: 25 25 50
   :header-rows: 1

   * - Argument
     - Type
     - Description
   * - :code:`filename`
     - String
     - Path of :code:`.ply` file
   * - :code:`vertices`
     - Numpy array
     - Isosurface vertices
   * - :code:`normals`
     - Numpy array
     - Isosurface normals
   * - :code:`indices`
     - Numpy array
     - Polygon indices (triplets)