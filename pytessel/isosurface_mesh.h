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

#include <fstream>
#include <set>
#include <vector>
#include <unordered_map>
#include <memory>

#include "vec3.h"
#include "isosurface.h"

// add SGN function for Windows
template <typename T> float sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

/**
 * @brief      structure to put Vec3f in a map
 */
struct KeyFuncs
{
    size_t operator()(const Vec3& k)const
    {
        return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
    }

    bool operator()(const Vec3& a, const Vec3& b)const
    {
            return a.x == b.x && a.y == b.y && a.z == b.z;
    }
};

/**
 * @brief      Class for iso surface mesh.
 */
class IsoSurfaceMesh{
private:
    std::unordered_map<Vec3, unsigned int, KeyFuncs, KeyFuncs> vertices_map;
    std::vector<Vec3> vertices;
    std::vector<Vec3> normals;
    std::vector<unsigned int> indices;

    std::shared_ptr<const ScalarField> sf;
    std::shared_ptr<const IsoSurface> is;

    Vec3 center;

public:
    /**
     * @brief      build isosurface mesh object
     *
     * @param[in]  _sf   pointer to scalar field
     * @param[in]  _is   pointer to isosurface
     */
    IsoSurfaceMesh(const std::shared_ptr<const ScalarField>& _sf,
                   const std::shared_ptr<const IsoSurface>& _is);

    IsoSurfaceMesh(const std::vector<float>& vertices,
                   const std::vector<float>& normals,
                   const std::vector<unsigned int>& indices);

    /**
     * @brief      construct surface mesh
     *
     * @param[in]  center_mesh  whether to center structure
     */
    void construct_mesh(bool center_mesh);

    std::vector<float> get_vertices() const;

    std::vector<float> get_normals() const;

    const std::vector<unsigned int>& get_indices() const;

private:
    /**
     * @brief      get the index of a vertex from unordered map
     *
     * @param[in]  v     vertex coordinates
     *
     * @return     the index
     */
    unsigned int get_index_vertex(const Vec3 v);
};
