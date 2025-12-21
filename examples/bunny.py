import numpy as np
from scipy.ndimage import gaussian_filter
from pytessel import PyTessel
import os

ROOT = os.path.dirname(__file__)

def main():
    output_ply = os.path.join(ROOT, 'bunny.ply')

    # load data points
    data = np.load(os.path.join(ROOT, 'bunny_pointcloud.npz'))
    vertices = data["vertices"]

    print("Voxelizing...")
    grid = voxelize(vertices, n=256)

    print("Smoothing...")
    field = gaussian_filter(grid, sigma=2.0)
    field /= field.max()  # normalize

    print("Running marching cubes...")
    t = PyTessel()
    v, nrm, idx = t.marching_cubes(
        field.ravel(),
        dimensions=field.shape,
        unitcell=(1, 0, 0,
                  0, 1, 0,
                  0, 0, 1),
        isovalue=0.01,
    )

    print(f"Extracted {len(v)} vertices")

    print("Writing output...")
    t.write_ply(output_ply, v, nrm, idx)

    print(f"Done! Output written to {output_ply}")

def voxelize(points, n=256, padding=0.05):
    mins = points.min(axis=0)
    maxs = points.max(axis=0)

    size = maxs - mins
    mins -= padding * size
    maxs += padding * size

    grid = np.zeros((n, n, n), dtype=np.float32)

    idx = ((points - mins) / (maxs - mins) * (n - 1)).astype(np.int32)
    idx = np.clip(idx, 0, n - 1)

    grid[idx[:, 0], idx[:, 1], idx[:, 2]] += 1.0
    return grid

if __name__ == "__main__":
    main()
