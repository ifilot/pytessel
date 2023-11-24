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

#pragma once

#include <string>
#include <vector>
#include <array>
#include <algorithm>

typedef float mat33[3][3];

#include "vec3.h"

class ScalarField{
private:
    std::array<unsigned int, 3> grid_dimensions;
    std::vector<float> grid;
    mat33 unitcell;
    mat33 unitcell_inverse;

public:

    /**
     * @brief      constructor
     *
     * @param[in]  _filename        url to filename
     * @param[in]  _flag_is_locpot  whether this file is a locpot
     * @param[in]  _is_bin          is binary file
     */
    ScalarField(const std::vector<float>& grid,
                const std::vector<unsigned int>& dimensions,
                const std::vector<float>& unitcell);

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
    float get_value_interp(float x, float y, float z) const;

    float get_value(unsigned int i, unsigned int j, unsigned int k) const;

    Vec3 grid_to_realspace(float i, float j, float k) const;

    Vec3 realspace_to_grid(float i, float j, float k) const;

    Vec3 realspace_to_direct(float x, float y, float z) const;

    void copy_grid_dimensions(unsigned int _grid_dimensions[]) const;

    float get_max() const;

    float get_min() const;

    /**
     * @brief      test whether point is inside unit cell
     *
     * @param[in]  x     x position
     * @param[in]  y     y position
     * @param[in]  z     z position
     *
     * @return     True if inside, False otherwise.
     */
    bool is_inside(float x, float y, float z) const;

    void inverse(const mat33& mat, mat33* invmat);

    inline const mat33& get_mat_unitcell() const {
        return this->unitcell;
    }

private:
};
