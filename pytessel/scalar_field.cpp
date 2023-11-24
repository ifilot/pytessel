/**************************************************************************
 *                                                                        *
 *   Author: Ivo Filot <ivo@ivofilot.nl>                                  *
 *                                                                        *
 *   PyTessel is free software:                                           *
 *   you can redistribute it and/or modify it under the terms of the      *
 *   GNU General Public License as published by the Free Software         *
 *   Foundation, either version 3 of the License, or (at your option)     *
 *   any later version.                                                   *
 *                                                                        *
 *   PyTessel is distributed in the hope that it will be useful,          *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty          *
 *   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.              *
 *   See the GNU General Public License for more details.                 *
 *                                                                        *
 *   You should have received a copy of the GNU General Public License    *
 *   along with this program.  If not, see http://www.gnu.org/licenses/.  *
 *                                                                        *
 **************************************************************************/

#include "scalar_field.h"

/**
 * @brief      constructor
 *
 * @param[in]  _filename   url to filename
 * @param[in]  _flag_is_locpot  whether this file is a locpot
 */
ScalarField::ScalarField(const std::vector<float>& _grid,
                         const std::vector<unsigned int>& _dimensions,
                         const std::vector<float>& _unitcell) {

    this->grid = _grid;
    this->grid_dimensions = {_dimensions[0], _dimensions[1], _dimensions[2]};
    for(unsigned int i=0; i<3; i++) {
        for(unsigned int j=0; j<3; j++) {
            this->unitcell[i][j] = _unitcell[i*3 + j];
        }
    }
    this->inverse(this->unitcell, &this->unitcell_inverse);
}

/*
 * float get_value_interp(x,y,z)
 *
 * Grabs a value from the 3D scalar field. Calculate the value
 * by using a trilinear interpolation.
 *
 * The trilinear interpolation algorithm has been extracted from:
 * http://paulbourke.net/miscellaneous/interpolation/
 *
 * Future algorithm can make use of a cubic interpolation.
 *
 */
float ScalarField::get_value_interp(float x, float y, float z) const {
    if(!this->is_inside(x,y,z)) {
        return 0.0f;
    }

    // cast the input to the nearest grid point
    Vec3 r = this->realspace_to_grid(x,y,z);

    // calculate value using trilinear interpolation
    float xd = remainderf(r.x, 1.0);
    float yd = remainderf(r.y, 1.0);
    float zd = remainderf(r.z, 1.0);

    if(xd < 0) xd += 1.0;
    if(yd < 0) yd += 1.0;
    if(zd < 0) zd += 1.0;

    float x0 = floor(r.x);
    float x1 = ceil(r.x);
    float y0 = floor(r.y);
    float y1 = ceil(r.y);
    float z0 = floor(r.z);
    float z1 = ceil(r.z);

    return
    this->get_value(x0, y0, z0) * (1.0 - xd) * (1.0 - yd) * (1.0 - zd) +
    this->get_value(x1, y0, z0) * xd                 * (1.0 - yd) * (1.0 - zd) +
    this->get_value(x0, y1, z0) * (1.0 - xd) * yd                 * (1.0 - zd) +
    this->get_value(x0, y0, z1) * (1.0 - xd) * (1.0 - yd) * zd                 +
    this->get_value(x1, y0, z1) * xd                 * (1.0 - yd) * zd                 +
    this->get_value(x0, y1, z1) * (1.0 - xd) * yd                 * zd                 +
    this->get_value(x1, y1, z0) * xd                 * yd                 * (1.0 - zd) +
    this->get_value(x1, y1, z1) * xd                 * yd                 * zd;
}

/**
 * @brief      test whether point is inside unit cell
 *
 * @param[in]  x     x position
 * @param[in]  y     y position
 * @param[in]  z     z position
 *
 * @return     True if inside, False otherwise.
 */
bool ScalarField::is_inside(float x, float y, float z) const {
    // cast the input to the nearest grid point
    Vec3 d = this->realspace_to_direct(x,y,z);

    if(d.x < 0 || d.x > 1.0) {
        return false;
    }
    if(d.y < 0 || d.y > 1.0) {
        return false;
    }
    if(d.z < 0 || d.z > 1.0) {
        return false;
    }

    return true;
}

/*
 * float get_value(i,j,k)
 *
 * Grabs the value at a particular grid point.
 *
 * This is a convenience function for the get_value_interp() function
 *
 */
float ScalarField::get_value(unsigned int i, unsigned int j, unsigned int k) const {
    unsigned int idx = k * this->grid_dimensions[0] * this->grid_dimensions[1] +
                       j * this->grid_dimensions[0] +
                       i;
    return this->grid[idx];
}

/*
 * Vec3 grid_to_realspace(i,j,k)
 *
 * Converts a grid point to a realspace vector. This function
 * is not being used at the moment.
 *
 */
Vec3 ScalarField::grid_to_realspace(float i, float j, float k) const {
    float dx = (float)i / (float)grid_dimensions[0];
    float dy = (float)j / (float)grid_dimensions[1];
    float dz = (float)k / (float)grid_dimensions[2];

    Vec3 r;
    r.x = this->unitcell[0][0] * dx + this->unitcell[1][0] * dy + this->unitcell[2][0] * dz;
    r.y = this->unitcell[0][1] * dx + this->unitcell[1][1] * dy + this->unitcell[2][1] * dz;
    r.z = this->unitcell[0][2] * dx + this->unitcell[1][2] * dy + this->unitcell[2][2] * dz;

    return r;
}

Vec3 ScalarField::realspace_to_direct(float x, float y, float z) const {
    Vec3 d;
    d.x = this->unitcell_inverse[0][0] * x + this->unitcell_inverse[0][1] * y + this->unitcell_inverse[0][2] * z;
    d.y = this->unitcell_inverse[1][0] * x + this->unitcell_inverse[1][1] * y + this->unitcell_inverse[1][2] * z;
    d.z = this->unitcell_inverse[2][0] * x + this->unitcell_inverse[2][1] * y + this->unitcell_inverse[2][2] * z;

    return d;
}

/*
 * Vec3 realspace_to_grid(i,j,k)
 *
 * Convert 3d realspace vector to a position on the grid. Non-integer
 * values (i.e. floating point) are given as the result.
 *
 * This is a convenience function for the get_value_interp() function
 *
 */
Vec3 ScalarField::realspace_to_grid(float i, float j, float k) const {
    Vec3 g = this->realspace_to_direct(i, j, k);

    g.x *= float(this->grid_dimensions[0]-1);
    g.y *= float(this->grid_dimensions[1]-1);
    g.z *= float(this->grid_dimensions[2]-1);

    return g;
}

void ScalarField::copy_grid_dimensions(unsigned int _grid_dimensions[]) const {
    for(unsigned int i=0; i<3; i++) {
        _grid_dimensions[i] = this->grid_dimensions[i];
    }
}

float ScalarField::get_max() const {
    return *std::max_element(this->grid.begin(), this->grid.end());
}

float ScalarField::get_min() const {
    return *std::min_element(this->grid.begin(), this->grid.end());
}

void ScalarField::inverse(const mat33& mat, mat33* invmat) {
    // computes the inverse of a matrix m
    float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
                mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0]) +
                mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);

    float invdet = 1.0 / det;

    (*invmat)[0][0] = (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) * invdet;
    (*invmat)[0][1] = (mat[0][2] * mat[2][1] - mat[0][1] * mat[2][2]) * invdet;
    (*invmat)[0][2] = (mat[0][1] * mat[1][2] - mat[0][2] * mat[1][1]) * invdet;
    (*invmat)[1][0] = (mat[1][2] * mat[2][0] - mat[1][0] * mat[2][2]) * invdet;
    (*invmat)[1][1] = (mat[0][0] * mat[2][2] - mat[0][2] * mat[2][0]) * invdet;
    (*invmat)[1][2] = (mat[1][0] * mat[0][2] - mat[0][0] * mat[1][2]) * invdet;
    (*invmat)[2][0] = (mat[1][0] * mat[2][1] - mat[2][0] * mat[1][1]) * invdet;
    (*invmat)[2][1] = (mat[2][0] * mat[0][1] - mat[0][0] * mat[2][1]) * invdet;
    (*invmat)[2][2] = (mat[0][0] * mat[1][1] - mat[1][0] * mat[0][1]) * invdet;
}
