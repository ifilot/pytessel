# distutils: language = c++

from libcpp.vector cimport vector
from libcpp.string cimport string

# IsoSurface
cdef extern from "isosurface.cpp":
    pass

# Isosurface Mesh
cdef extern from "isosurface_mesh.cpp":
    pass

# Scalar Field
cdef extern from "scalar_field.cpp":
    pass

# Scalar Field class
cdef extern from "scalar_field.h":
    cdef cppclass ScalarField:
        ScalarField(vector[float], vector[uint], vector[float]) except +

# Isosurface class
cdef extern from "isosurface.h":
    cdef cppclass IsoSurface:
        IsoSurface(ScalarField *_sf) except +
        void marching_cubes(float) except+

# Isosurface Mesh class
cdef extern from "isosurface_mesh.h":
    cdef cppclass IsoSurfaceMesh:
        IsoSurfaceMesh(const ScalarField *, const IsoSurface*) except +
        IsoSurfaceMesh(vector[float], vector[float], vector[uint]) except +
        void construct_mesh(bool) except+
        vector[float] get_vertices() except+
        vector[float] get_normals() except+
        vector[uint] get_indices() except+
