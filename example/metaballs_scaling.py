from pytessel import PyTessel
import numpy as np
import time
import matplotlib.pyplot as plt

# In this script, we sample the scaling relationship between the number of
# sample points and the execution time for both isosurface construction as well
# as scalar field generation

def main():
    """
    Build 6 .ply files of increasing quality
    """
    pytessel = PyTessel()

    nrpoints = [20,25,50,100,200,500]
    scalarfield_times = []
    isosurface_times = []

    for nrpoint in nrpoints:
        sz = 3
        
        x = np.linspace(-sz, sz, nrpoint)
        y = np.linspace(-sz, sz, nrpoint)
        z = np.linspace(-sz, sz, nrpoint)
        
        
        start = time.time()
        xx, yy, zz, field = icosahedron_field(x,y,z)
        end = time.time()
        scalarfield_times.append(end - start)
        
        unitcell = np.diag(np.ones(3) * sz * 2)
        pytessel = PyTessel()
        isovalue = 3.75
        
        start = time.time()
        vertices, normals, indices = pytessel.marching_cubes(field.flatten(), 
                                                             reversed(field.shape), 
                                                             unitcell.flatten(), 
                                                             isovalue)
        end = time.time()
        isosurface_times.append(end - start)
    
    
    fig, ax = plt.subplots(1, 2, dpi=144)
    ax[0].loglog(nrpoints, scalarfield_times, 'o', color='red', label='Data points')
    ax[1].loglog(nrpoints, isosurface_times, 'o', color='red', label='Data points')
    
    ax[0].set_xlabel('Number of data points [-]')
    ax[0].set_ylabel('Execution time [s]')
    ax[1].set_xlabel('Number of data points [-]')
    ax[1].set_ylabel('Execution time [s]')
    
    # perform linear fit
    a,b = np.polyfit(np.log(nrpoints), np.log(scalarfield_times), deg=1)
    x = np.linspace(np.min(nrpoints), np.max(nrpoints), 100)
    y = np.exp(a * np.log(x) + b)
    ax[0].plot(x, y, '--', color='black', label='Linear fit: %.2f' % a)
    ax[0].set_title('Scalar field generation' % a)
    ax[0].legend()
    
    a,b = np.polyfit(np.log(nrpoints), np.log(isosurface_times), deg=1)
    x = np.linspace(np.min(nrpoints), np.max(nrpoints), 100)
    y = np.exp(a * np.log(x) + b)
    ax[1].plot(x, y, '--', color='black', label='Linear fit: %.2f' % a)
    ax[1].set_title('Isosurface construction' % a)
    ax[1].legend()
    
    plt.tight_layout()
    plt.savefig('scaling_relation.jpg')

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
    
    zz,yy,xx = np.meshgrid(z,y,x,indexing='ij')
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