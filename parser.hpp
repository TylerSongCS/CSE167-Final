#ifndef parser_hpp
#define parser_hpp

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "PointCloud.h"

typedef struct parse_result parse_result_t;
typedef struct triangles triangles_t;

struct triangles {
    unsigned int num_triangles;
    std::vector<unsigned int> vertexIndices;
    std::vector<unsigned int> uvIndices;
    std::vector<unsigned int> normalIndices;
};

struct parse_result {
    std::vector<glm::vec3> point_cloud;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    triangles_t triangles;
};

parse_result_t parse_objectfile(std::string filename);

#endif /* parser_hpp */
