import numpy as np
from pytessel import PyTessel
import os

ROOT = os.path.dirname(__file__)

def generate_gyroid(
    n=192,
    periods=2,
):
    """
    Generate a gyroid scalar field on a cubic grid.
    """
    x = np.linspace(0, 2 * np.pi * periods, n, endpoint=False)
    X, Y, Z = np.meshgrid(x, x, x, indexing="ij")

    field = (
        np.sin(X) * np.cos(Y)
        + np.sin(Y) * np.cos(Z)
        + np.sin(Z) * np.cos(X)
    )

    return field.astype(np.float32)


if __name__ == "__main__":
    t = PyTessel()

    n = 256
    grid = generate_gyroid(n=n, periods=2)

    vertices, normals, indices = t.marching_cubes(
        grid.ravel(),
        dimensions=(n, n, n),
        unitcell=(1.0, 0.0, 0.0,
                  0.0, 1.0, 0.0,
                  0.0, 0.0, 1.0),
        isovalue=0.01,
    )

    t.write_ply(os.path.join(ROOT,"gyroid.ply"), vertices, normals, indices)
    t.write_stl(os.path.join(ROOT, "gyroid.stl"), vertices, normals, indices)
