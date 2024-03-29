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

#include "isosurface.h"

/**************
 *    CUBE    *
 **************/

/*
 *          z
 *          |
 *          4------5
 *         /|     /|
 *        7-|----6 |
 *        | 0----|-1--- y (j)
 *        |/     |/
 *        3 ---- 2
 *       /
 *      x (i)
 *
 */

Cube::Cube(unsigned int _i, unsigned int _j, unsigned int _k, const ScalarField &_vp) {

    this->i = _i;
    this->j = _j;
    this->k = _k;
    this->cubidx = 0;

    values[0] = _vp.get_value(_i, _j, _k);
    values[1] = _vp.get_value(_i, _j+1, _k);
    values[2] = _vp.get_value(_i+1, _j+1, _k);
    values[3] = _vp.get_value(_i+1, _j, _k);

    values[4] = _vp.get_value(_i, _j, _k+1);
    values[5] = _vp.get_value(_i, _j+1, _k+1);
    values[6] = _vp.get_value(_i+1, _j+1, _k+1);
    values[7] = _vp.get_value(_i+1, _j, _k+1);
}

void Cube::set_cube_index(float _isovalue) {
    if (this->values[0] < _isovalue) this->cubidx |= (1 << 0);
    if (this->values[1] < _isovalue) this->cubidx |= (1 << 1);
    if (this->values[2] < _isovalue) this->cubidx |= (1 << 2);
    if (this->values[3] < _isovalue) this->cubidx |= (1 << 3);
    if (this->values[4] < _isovalue) this->cubidx |= (1 << 4);
    if (this->values[5] < _isovalue) this->cubidx |= (1 << 5);
    if (this->values[6] < _isovalue) this->cubidx |= (1 << 6);
    if (this->values[7] < _isovalue) this->cubidx |= (1 << 7);
}

unsigned int Cube::get_cube_index() const {
    return this->cubidx;
}

float Cube::get_value_from_vertex(unsigned int _p) const {
    return this->values[_p];
}

Vec3 Cube::get_position_from_vertex(unsigned int _p) const {
    Vec3 c;
    if(_p == 0) {
        c.x = (float)this->i;
        c.y = (float)this->j;
        c.z = (float)this->k;
    } else if(_p == 1) {
        c.x = (float)this->i;
        c.y = (float)this->j+1;
        c.z = (float)this->k;
    } else if(_p == 2) {
        c.x = (float)this->i+1;
        c.y = (float)this->j+1;
        c.z = (float)this->k;
    } else if(_p == 3) {
        c.x = (float)this->i+1;
        c.y = (float)this->j;
        c.z = (float)this->k;
    } else if(_p == 4) {
        c.x = (float)this->i;
        c.y = (float)this->j;
        c.z = (float)this->k+1;
    } else if(_p == 5) {
        c.x = (float)this->i;
        c.y = (float)this->j+1;
        c.z = (float)this->k+1;
    } else if(_p == 6) {
        c.x = (float)this->i+1;
        c.y = (float)this->j+1;
        c.z = (float)this->k+1;
    } else if(_p == 7) {
        c.x = (float)this->i+1;
        c.y = (float)this->j;
        c.z = (float)this->k+1;
    } else {
        c.x = 0;
        c.y = 0;
        c.z = 0;
    }

    return c;
}

/*********************
 *    TETRAHEDRON    *
 *********************/

Tetrahedron::Tetrahedron(unsigned int _i, unsigned int _j, unsigned int _k,
    const ScalarField &_vp, unsigned int _pos) {
    this->i = _i;
    this->j = _j;
    this->k = _k;
    this->tetidx = 0;

/*
 *          z
 *          |
 *          4------5
 *         /|     /|
 *        7-|----6 |
 *        | 0----|-1--- y (j)
 *        |/     |/
 *        3 ---- 2
 *       /
 *      x (i)
 *
 */

    switch(_pos) {
        case 0: // 0 2 3 7
            values[0] = _vp.get_value(_i    , _j    , _k);
            values[1] = _vp.get_value(_i+1, _j+1, _k);
            values[2] = _vp.get_value(_i+1, _j    , _k);
            values[3] = _vp.get_value(_i+1, _j    , _k+1);
            pos[0].x = _i     ; pos[0].y = _j     ; pos[0].z = _k     ;
            pos[1].x = _i+1   ; pos[1].y = _j+1 ; pos[1].z = _k     ;
            pos[2].x = _i+1   ; pos[2].y = _j     ; pos[2].z = _k     ;
            pos[3].x = _i+1   ; pos[3].y = _j     ; pos[3].z = _k+1 ;
        break;
        case 1: // 0 2 6 7
            values[0] = _vp.get_value(_i,     _j,     _k);
            values[1] = _vp.get_value(_i+1, _j+1, _k);
            values[2] = _vp.get_value(_i+1, _j+1, _k+1);
            values[3] = _vp.get_value(_i+1, _j ,    _k+1);
            pos[0].x = _i     ; pos[0].y = _j     ; pos[0].z = _k     ;
            pos[1].x = _i+1   ; pos[1].y = _j+1 ; pos[1].z = _k     ;
            pos[2].x = _i+1   ; pos[2].y = _j+1 ; pos[2].z = _k+1 ;
            pos[3].x = _i+1   ; pos[3].y = _j     ; pos[3].z = _k+1 ;
        break;
        case 2: // 0 4 6 7
            values[0] = _vp.get_value(_i, _j, _k);
            values[1] = _vp.get_value(_i, _j, _k+1);
            values[2] = _vp.get_value(_i+1, _j+1, _k+1);
            values[3] = _vp.get_value(_i+1, _j ,    _k+1);
            pos[0].x = _i     ; pos[0].y = _j     ; pos[0].z = _k     ;
            pos[1].x = _i     ; pos[1].y = _j     ; pos[1].z = _k+1 ;
            pos[2].x = _i+1   ; pos[2].y = _j+1 ; pos[2].z = _k+1 ;
            pos[3].x = _i+1   ; pos[3].y = _j     ; pos[3].z = _k+1 ;
        break;
        case 3: // 0 6 1 2
            values[0] = _vp.get_value(_i, _j, _k);
            values[1] = _vp.get_value(_i+1, _j+1, _k+1);
            values[2] = _vp.get_value(_i, _j+1, _k);
            values[3] = _vp.get_value(_i+1, _j+1, _k);
            pos[0].x = _i     ; pos[0].y = _j     ; pos[0].z = _k     ;
            pos[1].x = _i+1   ; pos[1].y = _j+1 ; pos[1].z = _k+1 ;
            pos[2].x = _i     ; pos[2].y = _j+1 ; pos[2].z = _k     ;
            pos[3].x = _i+1   ; pos[3].y = _j+1 ; pos[3].z = _k     ;
        break;
        case 4: // 0 6 1 4
            values[0] = _vp.get_value(_i, _j, _k);
            values[1] = _vp.get_value(_i+1, _j+1, _k+1);
            values[2] = _vp.get_value(_i, _j+1, _k);
            values[3] = _vp.get_value(_i, _j, _k+1);
            pos[0].x = _i     ; pos[0].y = _j     ; pos[0].z = _k     ;
            pos[1].x = _i+1   ; pos[1].y = _j+1 ; pos[1].z = _k+1 ;
            pos[2].x = _i     ; pos[2].y = _j+1 ; pos[2].z = _k     ;
            pos[3].x = _i     ; pos[3].y = _j     ; pos[3].z = _k+1 ;
        break;
        case 5: // 5 6 1 4
            values[0] = _vp.get_value(_i, _j+1, _k+1);
            values[1] = _vp.get_value(_i+1, _j+1, _k+1);
            values[2] = _vp.get_value(_i, _j+1, _k);
            values[3] = _vp.get_value(_i, _j, _k+1);
            pos[0].x = _i     ; pos[0].y = _j+1 ; pos[0].z = _k+1 ;
            pos[1].x = _i+1   ; pos[1].y = _j+1 ; pos[1].z = _k+1 ;
            pos[2].x = _i     ; pos[2].y = _j+1 ; pos[2].z = _k     ;
            pos[3].x = _i     ; pos[3].y = _j   ; pos[3].z = _k+1 ;
        break;
        default:
            exit(-1);
    }
}

void Tetrahedron::set_tetrahedron_index(float _isovalue) {
    if (this->values[0] < _isovalue) this->tetidx |= (1 << 0);
    if (this->values[1] < _isovalue) this->tetidx |= (1 << 1);
    if (this->values[2] < _isovalue) this->tetidx |= (1 << 2);
    if (this->values[3] < _isovalue) this->tetidx |= (1 << 3);
}

unsigned int Tetrahedron::get_tetrahedron_index() const {
    return this->tetidx;
}

float Tetrahedron::get_value_from_vertex(unsigned int _p) const {
    return this->values[_p];
}

const Vec3& Tetrahedron::get_position_from_vertex(unsigned int _p) const {
    return pos[_p];
}

/**************
 *  TRIANGLE  *
 **************/

Triangle::Triangle(const Vec3 &_p1, const Vec3 &_p2, const Vec3 &_p3) {
    this->p1 = _p1;
    this->p2 = _p2;
    this->p3 = _p3;
}

void Triangle::transform_to_real(const ScalarField &_vp) {
    p1 = _vp.grid_to_realspace(p1.x, p1.y, p1.z);
    p2 = _vp.grid_to_realspace(p2.x, p2.y, p2.z);
    p3 = _vp.grid_to_realspace(p3.x, p3.y, p3.z);
}

float Triangle::get_x(unsigned int i) const {
    switch(i) {
        case 0:
            return p1.x;
        case 1:
            return p2.x;
        case 2:
            return p3.x;
        default:
            return p1.x;
    }
}

float Triangle::get_y(unsigned int i) const {
    switch(i) {
        case 0:
            return p1.y;
        case 1:
            return p2.y;
        case 2:
            return p3.y;
        default:
            return p1.y;
    }
}

float Triangle::get_z(unsigned int i) const {
    switch(i) {
        case 0:
            return p1.z;
        case 1:
            return p2.z;
        case 2:
            return p3.z;
        default:
            return p1.z;
    }
}

/**************
 * ISOSURFACE *
 **************/

/**
 * @brief      default constructor
 *
 * @param      _sf   pointer to ScalarField object
 */
IsoSurface::IsoSurface(const std::shared_ptr<ScalarField>& _vp) :
    vp_ptr(_vp) {
    this->isovalue = 0;
    this->vp_ptr->copy_grid_dimensions(this->grid_dimensions);
}

/**
 * @brief      generate isosurface using marching cubes algorithm
 *
 * @param[in]  _isovalue  The isovalue
 */
void IsoSurface::marching_cubes(float _isovalue) {
    this->isovalue = _isovalue;
    this->sample_grid_with_cubes(_isovalue);
    this->construct_triangles_from_cubes(_isovalue);

    #pragma omp parallel for
    for(unsigned int i=0; i < this->triangles.size(); i++) {
        triangles[i].transform_to_real(*this->vp_ptr);
    }
}

/**
 * @brief      generate isosurface using marching tetrahedra algorithm
 *
 * @param[in]  _isovalue  The isovalue
 */
void IsoSurface::marching_tetrahedra(float _isovalue) {
    this->isovalue = _isovalue;
    this->sample_grid_with_tetrahedra(_isovalue);
    this->construct_triangles_from_tetrahedra(_isovalue);
    for(std::vector<Triangle>::iterator it = triangles.begin();
        it != triangles.end(); ++it) {
        it->transform_to_real(*this->vp_ptr);
    }
}

/**
 * @brief      Gets the triangles pointer.
 *
 * @return     The triangles pointer.
 */
const std::vector<Triangle>* IsoSurface::get_triangles_ptr() const {
    return &this->triangles;
}

void IsoSurface::sample_grid_with_cubes(float _isovalue) {
    std::mutex push_back_mutex;

    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i = 0; i < this->grid_dimensions[2] - 1; i++) {
        for(unsigned int j = 0; j < this->grid_dimensions[1] - 1; j++) {
            for(unsigned int k = 0; k < this->grid_dimensions[0] - 1; k++) {
                Cube cub(k, j, i, *this->vp_ptr);
                cub.set_cube_index(_isovalue);
                if(!(cub.get_cube_index() == (unsigned int)0 ||
                    cub.get_cube_index() == (unsigned int)255)) {
                    push_back_mutex.lock();
                    this->cube_table.push_back(cub);
                    push_back_mutex.unlock();
                }
            }
        }
    }
}

void IsoSurface::sample_grid_with_tetrahedra(float _isovalue) {
    for(unsigned int i = 0; i < this->grid_dimensions[2] - 1; i++) {
        for(unsigned int j = 0; j < this->grid_dimensions[1] - 1; j++) {
            for(unsigned int k = 0; k < this->grid_dimensions[0] - 1; k++) {
                for(unsigned int l=0; l<6; l++) {
                    Tetrahedron tet(k, j, i, *this->vp_ptr, l);
                    tet.set_tetrahedron_index(_isovalue);
                    if(!(tet.get_tetrahedron_index() == (unsigned int)0 ||
                             tet.get_tetrahedron_index() == (unsigned int)15)) {
                        this->tetrahedra_table.push_back(tet);
                    }
                }
            }
        }
    }
}

void IsoSurface::construct_triangles_from_cubes(float _isovalue) {
    std::mutex push_back_mutex;

    #pragma omp parallel for schedule(dynamic)
    for(unsigned int i=0; i < cube_table.size(); i++) {

        uint8_t cubeindex = cube_table[i].get_cube_index();
        Vec3 vertices_list[12];

        /* Find the vertices where the surface intersects the cube, perform
        an interpolation of 2 (Vec3) coordinates and 2 values and the isovalue,
        return one (Vec3) coordinate as the result */
        if (edge_table[cubeindex] & (1 << 0))
            vertices_list[0] =
                this->interpolate_from_cubes(cube_table[i], 0, 1, _isovalue);
        if (edge_table[cubeindex] & (1 << 1))
            vertices_list[1] =
                this->interpolate_from_cubes(cube_table[i], 1, 2, _isovalue);
        if (edge_table[cubeindex] & (1 << 2))
            vertices_list[2] =
                this->interpolate_from_cubes(cube_table[i], 2, 3, _isovalue);
        if (edge_table[cubeindex] & (1 << 3))
            vertices_list[3] =
                this->interpolate_from_cubes(cube_table[i], 3, 0, _isovalue);
        if (edge_table[cubeindex] & (1 << 4))
            vertices_list[4] =
                this->interpolate_from_cubes(cube_table[i], 4, 5, _isovalue);
        if (edge_table[cubeindex] & (1 << 5))
            vertices_list[5] =
                this->interpolate_from_cubes(cube_table[i], 5, 6, _isovalue);
        if (edge_table[cubeindex] & (1 << 6))
            vertices_list[6] =
                this->interpolate_from_cubes(cube_table[i], 6, 7, _isovalue);
        if (edge_table[cubeindex] & (1 << 7))
            vertices_list[7] =
                this->interpolate_from_cubes(cube_table[i], 7, 4, _isovalue);
        if (edge_table[cubeindex] & (1 << 8))
            vertices_list[8] =
                this->interpolate_from_cubes(cube_table[i], 0, 4, _isovalue);
        if (edge_table[cubeindex] & (1 << 9))
            vertices_list[9] =
                this->interpolate_from_cubes(cube_table[i], 1, 5, _isovalue);
        if (edge_table[cubeindex] & (1 << 10))
            vertices_list[10] =
                this->interpolate_from_cubes(cube_table[i], 2, 6, _isovalue);
        if (edge_table[cubeindex] & (1 << 11))
            vertices_list[11] =
                this->interpolate_from_cubes(cube_table[i], 3, 7, _isovalue);

        /* finally construct the triangles using the triangle table */
        for(unsigned int i=0; triangle_table[cubeindex][i] != -1; i += 3) {
            Triangle triangle(
                    vertices_list[triangle_table[cubeindex][i]],
                    vertices_list[triangle_table[cubeindex][i+1]],
                    vertices_list[triangle_table[cubeindex][i+2]]);
            /* push the triangle to the list */
            push_back_mutex.lock();
            this->triangles.push_back(triangle);
            push_back_mutex.unlock();
        }
    }
}

void IsoSurface::construct_triangles_from_tetrahedra(float _isovalue) {
    for(unsigned int i=0; i < tetrahedra_table.size(); i++) {

        unsigned int tetidx = tetrahedra_table[i].get_tetrahedron_index();
        Vec3 p[3];

        switch(tetidx) {
            case 0x0E:
            case 0x01:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 1, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 2, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 3, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                break;
            case 0x0D:
            case 0x02:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 0, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 3, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 2, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                break;
            case 0x0C:
            case 0x03:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 3, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 2, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 3, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));

                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 2, _isovalue);
                this->triangles.push_back(Triangle(p[2], p[0], p[1]));

                break;
            case 0x0B:
            case 0x04:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 2, 0, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 2, 1, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 2, 3, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                break;
            case 0x0A:
            case 0x05:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 1, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 2, 3, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 3, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 2, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[2], p[1]));
                break;
            case 0x09:
            case 0x06:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 1, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 1, 3, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 2, 3, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 0, 2, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
                break;
            case 0x07:
            case 0x08:
                p[0] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 3, 0, _isovalue);
                p[1] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 3, 2, _isovalue);
                p[2] = this->interpolate_from_tetrahedra(tetrahedra_table[i], 3, 1, _isovalue);
                this->triangles.push_back(Triangle(p[0], p[1], p[2]));
        break;
        }
    }
}

Vec3 IsoSurface::interpolate_from_cubes(const Cube &_cub, unsigned int _p1,
    unsigned int _p2, float _isovalue) {
    float v1 = _cub.get_value_from_vertex(_p1);
    float v2 = _cub.get_value_from_vertex(_p2);

    Vec3 p1 = _cub.get_position_from_vertex(_p1);
    Vec3 p2 = _cub.get_position_from_vertex(_p2);

    Vec3 p;
    float mu;

    if(std::abs(_isovalue-v1) < PRECISION_LIMIT)
        return p1;
    if(std::abs(_isovalue-v2) < PRECISION_LIMIT)
        return p2;
    if(std::abs(v1-v2) < PRECISION_LIMIT)
        return p1;

    mu = (_isovalue - v1) / (v2 - v1);

    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;
}

Vec3 IsoSurface::interpolate_from_tetrahedra(const Tetrahedron &_tet,
    unsigned int _p1, unsigned int _p2, float _isovalue) {
    float v1 = _tet.get_value_from_vertex(_p1);
    float v2 = _tet.get_value_from_vertex(_p2);

    Vec3 p1 = _tet.get_position_from_vertex(_p1);
    Vec3 p2 = _tet.get_position_from_vertex(_p2);

    Vec3 p;
    float mu;

    if(std::abs(_isovalue-v1) < PRECISION_LIMIT)
        return p1;
    if(std::abs(_isovalue-v2) < PRECISION_LIMIT)
        return p2;
    if(std::abs(v1-v2) < PRECISION_LIMIT)
        return p1;

    mu = (_isovalue - v1) / (v2 - v1);

    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;
}
