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

#include "isosurface_mesh.h"

/**
 * @brief      build isosurface mesh object
 *
 * @param[in]  _sf   pointer to scalar field
 * @param[in]  _is   pointer to isosurface
 */
IsoSurfaceMesh::IsoSurfaceMesh(const std::shared_ptr<const ScalarField>& _sf,
                               const std::shared_ptr<const IsoSurface>& _is) :
    sf(_sf),
    is(_is) {
}

/**
 * @brief      construct surface mesh
 *
 * @param[in]  center  whether to center structure
 */
void IsoSurfaceMesh::construct_mesh(bool center_mesh) {
   // grab center
    this->center = this->sf->get_mat_unitcell() * glm::vec3(0.5, 0.5, 0.5);

    for(unsigned int i=0; i<this->is->get_triangles_ptr()->size(); i++) {
        this->texcoords.push_back(glm::vec2(0,0));
        this->texcoords.push_back(glm::vec2(0,1));
        this->texcoords.push_back(glm::vec2(1,0));

        // load all index vertices in a map; this operation needs to be done, else a SEGFAULT
        // will be thrown further down the lines
        this->get_index_vertex(is->get_triangles_ptr()->at(i).p1);
        this->get_index_vertex(is->get_triangles_ptr()->at(i).p2);
        this->get_index_vertex(is->get_triangles_ptr()->at(i).p3);
    }

    // build vertex vector from unordered map
    this->vertices.resize(this->vertices_map.size());
    for(auto it : this->vertices_map) {
        this->vertices[it.second] = it.first;
    }

    double dev = 0.01;
    this->normals.resize(this->vertices.size());

    // calculate normal vectors
    #pragma omp parallel for schedule(static)
    for(unsigned int i=0; i<this->vertices.size(); i++) {
        // get derivatives
        double dx0 = sf->get_value_interp(this->vertices[i][0] - dev, this->vertices[i][1], this->vertices[i][2]);
        double dx1 = sf->get_value_interp(this->vertices[i][0] + dev, this->vertices[i][1], this->vertices[i][2]);

        double dy0 = sf->get_value_interp(this->vertices[i][0], this->vertices[i][1] - dev, this->vertices[i][2]);
        double dy1 = sf->get_value_interp(this->vertices[i][0], this->vertices[i][1] + dev, this->vertices[i][2]);

        double dz0 = sf->get_value_interp(this->vertices[i][0], this->vertices[i][1], this->vertices[i][2] - dev);
        double dz1 = sf->get_value_interp(this->vertices[i][0], this->vertices[i][1], this->vertices[i][2] + dev);

        glm::vec3 normal((dx1 - dx0) / (2.0 * dev),
                         (dy1 - dy0) / (2.0 * dev),
                         (dz1 - dz0) / (2.0 * dev));
        normal = -glm::normalize(normal); // the negative of the gradient is the correct normal

        this->normals[i] = normal * sgn(sf->get_value_interp(this->vertices[i][0], this->vertices[i][1], this->vertices[i][2]));
    }

    // build indices in right orientation based on face normal
    for(unsigned int i=0; i<this->is->get_triangles_ptr()->size(); i++) {
        // calculate face normal
        unsigned int id1 = this->get_index_vertex(is->get_triangles_ptr()->at(i).p1);
        unsigned int id2 = this->get_index_vertex(is->get_triangles_ptr()->at(i).p2);
        unsigned int id3 = this->get_index_vertex(is->get_triangles_ptr()->at(i).p3);

        // calculate the orientation of the face with respect to the normal
        const glm::vec3 face_normal = (this->normals[id1] + this->normals[id2] + this->normals[id3]) / 3.0f;
        const glm::vec3 orientation_face = glm::normalize(glm::cross(this->vertices[id2] - this->vertices[id1], this->vertices[id3] - this->vertices[id1]));
        const float orientation = glm::dot(face_normal, orientation_face);

        // if orientation is positive, the orientation is correct, if it is negative, the orientation is incorrect and two indices should be swapped
        if(orientation > 0.0f) {
            this->indices.push_back(id1);
            this->indices.push_back(id2);
            this->indices.push_back(id3);
        } else {
            this->indices.push_back(id2);
            this->indices.push_back(id1);
            this->indices.push_back(id3);
        }
    }

    // center structure
    if(center_mesh) {
        glm::vec3 sum = this->sf->get_mat_unitcell() * glm::vec3(0.5f, 0.5f, 0.5f);

        #pragma omp parallel for
        for(unsigned int i=0; i<this->vertices.size(); i++) {
           this->vertices[i] -= sum;
        }
    }
}

/**
 * @brief      get the index of a vertex from unordered map
 *
 * @param[in]  v     vertex coordinates
 *
 * @return     the index
 */
unsigned int IsoSurfaceMesh::get_index_vertex(const glm::vec3 v) {
    auto got = this->vertices_map.find(v);
    if(got != this->vertices_map.end()) {
        return got->second;
    } else {
        this->vertices_map.emplace(v, this->vertices_map.size());
        return this->get_index_vertex(v);
    }
}

std::vector<float> IsoSurfaceMesh::get_vertices() const {
    return std::vector<float>(&this->vertices[0][0], &this->vertices[0][0] + this->vertices.size() * 3);
}

std::vector<float> IsoSurfaceMesh::get_normals() const {
    return std::vector<float>(&this->normals[0][0], &this->normals[0][0] + this->normals.size() * 3);
}

const std::vector<unsigned int>& IsoSurfaceMesh::get_indices() const {
    return this->indices;
}
