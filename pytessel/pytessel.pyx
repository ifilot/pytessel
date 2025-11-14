# distutils: language = c++
# cython: c_string_type=unicode, c_string_encoding=utf8

from .pytessel cimport ScalarField, IsoSurface
from libcpp.string cimport string
from libcpp.memory cimport shared_ptr,make_shared
import numpy as np
import sys
import cython
import numpy.typing as npt

cdef class PyTessel:

    def __cinit__(self):
        pass

    @cython.embedsignature(True)
    def marching_cubes(self, vector[float] grid, vector[size_t] dimensions, vector[float] unitcell, float isovalue) -> \
            (npt.NDArray[np.float64], npt.NDArray[np.float64], npt.NDArray[np.float64]):
        """
        Perform marching cubes algorithm to generate isosurface

        Parameters
        ----------
        grid : Iterable of floats
            Scalar field as a flattened array
        dimensions : Iterable of ints
            Dimensions of the scalar field grid (nx, ny, nz)
        unitcell : Iterable of floats
            Unitcell matrix (flattened)
        isovalue : float
            Isovalue of the isosurface
               
        Returns
        -------
        vertices : (Nx3) numpy array of floats
            Triangle vertices
        normals : (Nx3) numpy array of floats
            Triangle normals (at the vertices)
        indices : numpy array of ints
            Triangle indices
        
        Notes
        -----
        * The scalar field needs to be encoded such that the z-coordinate is the slowest moving
          index and the x-coordinate the fastest moving index.
        * The dimensions of the scalar field are encoded in the order :code:`(nx, ny, nz)`.
        * You can use :code:`reversed(grid.shape)` to pass the dimensions.
        * The output of the :code:`marching_cubes` function are three arrays, corresponding to the
          vertices, the normals, and the indices of the isosurface. The isosurface corresponds to
          a number of linked triangles (polygons) whose vertices are stored in the :code:`vertices`
          array. For each vertex, the normal vector is encoded in the :code:`normals` array. Finally,
          the triangles are stored as a triplet of indices in the :code:`indices` array. These indices
          refer to the position in the :code:`vertices` and :code:`normals` array. Because multiple
          triangles can use the same vertices, this is an efficient way to store the isosurface.
        * One rarely needs to perform any operations on the :code:`vertices`, :code:`normals` and
          :code:`indices` arrays. Typically, these arrays are constructed and immediately relayed
          to the :code:`write_ply` function to store them as a file which can be used in another
          program. 
        """
        cdef shared_ptr[ScalarField] scalarfield
        cdef shared_ptr[IsoSurface] isosurface
        cdef shared_ptr[IsoSurfaceMesh] isosurface_mesh

        # build scalar field
        scalarfield = make_shared[ScalarField](grid, dimensions, unitcell)

        # construct isosurface
        isosurface = make_shared[IsoSurface](scalarfield)
        isosurface.get().marching_cubes(isovalue)

        # extract isosurface mesh
        isosurface_mesh = make_shared[IsoSurfaceMesh](scalarfield, isosurface)
        isosurface_mesh.get().construct_mesh(False)

        # extract data
        vertices = np.array(isosurface_mesh.get().get_vertices(), dtype=np.float32).reshape(-1,3)
        normals = np.array(isosurface_mesh.get().get_normals(), dtype=np.float32).reshape(-1,3)
        indices = np.array(isosurface_mesh.get().get_indices(), dtype=np.uint32)

        return vertices, normals, indices

    @cython.embedsignature(True)
    def write_ply(self, filename:str, vertices:npt.NDArray[np.float64], normals:npt.NDArray[np.float64], indices:npt.NDArray[np.uint32]) -> None:
        """Stores isosurface as .ply file

        Parameters
        ----------
        filename : str
            Path to file
        vertices : Iterable of floats
            Array of vertices
        normals : Iterable of floats
            Array of normals
        indices : Iterable of ints
            Array of triangle indices
            
        Returns
        -------
        None
        
        Notes
        -----
        * This function is designed to be used with :code:`marching_cubes`. One can directly relay the output
          of :code:`marching_cube` as the input for :code:`write_ply`.
        """
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
