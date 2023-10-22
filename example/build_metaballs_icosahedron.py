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