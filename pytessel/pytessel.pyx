# distutils: language = c++
# cython: c_string_type=unicode, c_string_encoding=utf8

from .pytessel cimport ScalarField, IsoSurface
from libcpp.string cimport string
import numpy as np
import sys

cdef class PyTessel:

    def __cinit__(self):
        pass

    def marching_cubes(self, vector[float] grid, vector[uint] dimensions, vector[float] unitcell, float isovalue):
        cdef ScalarField* scalarfield
        cdef IsoSurface* isosurface
        cdef IsoSurfaceMesh* isosurface_mesh

        # build scalar field
        scalarfield = new ScalarField(grid, dimensions, unitcell)

        # construct isosurface
        isosurface = new IsoSurface(scalarfield)
        isosurface.marching_cubes(isovalue)

        # extract isosurface mesh
        isosurface_mesh = new IsoSurfaceMesh(scalarfield, isosurface)
        isosurface_mesh.construct_mesh(False)

        # extract data
        vertices = np.array(isosurface_mesh.get_vertices(), dtype=np.float32).reshape(-1,3)
        normals = np.array(isosurface_mesh.get_normals(), dtype=np.float32).reshape(-1,3)
        indices = np.array(isosurface_mesh.get_indices(), dtype=np.uint32)

        # clean data
        del scalarfield
        del isosurface
        del isosurface_mesh

        return vertices, normals, indices

    def write_ply(self, filename, vertices, normals, indices):
        f = open(filename, 'wb')

        f.write(b"ply\n")
        if sys.byteorder == 'little':
            f.write(b"format binary_little_endian 1.0\n")
        else:
            f.write(b"format binary_big_endian 1.0\n")

        f.write(b"comment test\n")
        f.write(b"element vertex %i\n" % vertices.shape[0])
        f.write(b"property float x\n")
        f.write(b"property float y\n")
        f.write(b"property float z\n")
        f.write(b"property float nx\n")
        f.write(b"property float ny\n")
        f.write(b"property float nz\n")
        f.write(b"element face %i\n" % int(len(indices)/3))
        f.write(b"property list uchar uint vertex_indices\n")
        f.write(b"end_header\n")

        for i in range(0, len(vertices)):
            f.write(vertices[i].tobytes())
            f.write(normals[i].tobytes())

        for i in range(0, int(len(indices)/3)):
            f.write(b'\x03')
            f.write(indices[i*3:(i+1)*3].tobytes())

        f.close()
