# -*- coding: utf-8 -*-
import unittest
from pytessel import PyTessel
import numpy as np
import sys, os
import platform

# add a reference to load the pytessel library
sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

class TestIsosurfaceMetaballIcosahedron(unittest.TestCase):

    def test_isosurface_icosahedron(self):
        """
        Test Isosurface Generation of a Gaussian
        """
        pytessel = PyTessel()

        # the test results differ for 64 bit OS and 32 bit OS (due to rounding errors in the scalar field)
        # the cause of this is mainly due to assessing when two vertices are alike, for 32 bit OS
        # the threshold is met sooner leading to fewer vertices; the number of triangles is however conserved
        # between 32 and 64 bit OS
        if platform.machine().endswith('64'):
            results = [
                [200,200,1128],
                [1312,1312,7896],
                [2168,2168,12960],
                [9224,9224,55224],
            ]
        else:
            results = [
                [192,192,1128],
                [1296,1296,7896],
                [2124,2124,12960],
                [9168,9168,55224],
            ]

        for nrpoints, result in zip([10,20,25,50], results):
            sz = 3
            
            x = np.linspace(-sz, sz, nrpoints)
            y = np.linspace(-sz, sz, nrpoints)
            z = np.linspace(-sz, sz, nrpoints)
            
            xx, yy, zz, field = icosahedron_field(x,y,z)
            
            unitcell = np.diag(np.ones(3) * sz * 2)
            pytessel = PyTessel()
            isovalue = 3.75
            vertices, normals, indices = pytessel.marching_cubes(field.flatten(), field.shape, unitcell.flatten(), isovalue)
       
            self.assertEqual(len(vertices), result[0])
            self.assertEqual(len(normals),  result[1])
            self.assertEqual(len(indices),  result[2])

def icosahedron_field(x,y,z):
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
    return 1 / ((x - X0)**2 + (y - Y0)**2 + (z - Z0)**2)

if __name__ == '__main__':
    unittest.main()