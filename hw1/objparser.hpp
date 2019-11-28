//
//  objparser.hpp
//  hw1
//
//  Created by Mingxun Song on 10/10/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//

#ifndef objparser_hpp
#define objparser_hpp

#include <stdio.h>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>

bool objParse(std::string objFilename,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec3> & out_normals
              );

#endif /* objparser_hpp */
