import unittest
from pytessel import PyTessel
import numpy as np

class TestIntegrals(unittest.TestCase):

    def testIsosurfaceGaussian(self):
        """
        Test Isosurface Generation of a Gaussian
        """
        pytessel = PyTessel()

        # generate some data
        x = np.linspace(0, 10, 20)
        # the grid is organized with z the slowest moving index and x the fastest moving index
        grid = np.flipud(np.vstack(np.meshgrid(x, x, x, indexing='ij')).reshape(3,-1)).T

        R = [5,5,5]
        scalarfield = np.reshape(np.array([gaussian(r,R) for r in grid]), (len(x),len(x),len(x)), order='F')
        unitcell = np.diag(np.ones(3) * 10.0)

        vertices, normals, indices = pytessel.marching_cubes(scalarfield.flatten(), scalarfield.shape, unitcell.flatten(), 0.1)

        self.assertEqual(len(vertices), 144)
        self.assertEqual(len(normals),  144)
        self.assertEqual(len(indices),  852)

def gaussian(r, R):
    return np.exp(-(r-R).dot((r-R)))

if __name__ == '__main__':
    unittest.main()
