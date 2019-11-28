//
//  objparser.cpp
//  hw1
//
//  Created by Mingxun Song on 10/10/19.
//  Copyright © 2019 Mingxun Song. All rights reserved.
//
#include <stdio.h>
#include <string>
#include <cstring>
#include "main.h"
#include <sstream>
#include <cmath>
#include "objparser.hpp"

bool objParse(
              std::string objFilename,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec3> & out_normals
              )
{
    std::vector<unsigned int> vertexIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec3> temp_normals;
    
    std::ifstream objFile(objFilename); // The obj file we are reading.

    // Check whether the file can be opened.
    if (objFile.is_open())
    {
        std::string line; // A line in the file.

        // Read lines from the file.
        while (std::getline(objFile, line))
        {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;

            // Read the first word of the line.
            std::string label;
            ss >> label;

            // If the line is about vertex (starting with a "v").
            if (label == "v")
            {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                ss >> point.x >> point.y >> point.z;

                // Process the point. For example, you can save it to a.
                temp_vertices.push_back(point);
            // If the line is about normal (starting with a "vn").
            }else if(label == "vn")
            {
                glm::vec3 normal;
                ss >> normal.x >> normal.y >> normal.z;
                temp_normals.push_back(normal);
                
            // If the line is about face (starting with a "f").
            }else if(label == "f")
            {
                std::string vertexIndex[3];
                std::string normalIndex[3];
                
                std::string delimiter = "//";
                ss >> vertexIndex[0] >> vertexIndex[1] >> vertexIndex[2];
                for(unsigned int i = 0; i < 3; i++){
                    vertexIndex[i] = vertexIndex[i].substr(0, vertexIndex[i].find(delimiter));
                    normalIndex[i] = vertexIndex[i];
                    vertexIndices.push_back(std::stoi(vertexIndex[i]));
                    normalIndices.push_back(std::stoi(normalIndex[i]));
                }
                
                
            }
        }
    }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
        return false;
    }
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int normalIndex = normalIndices[i];
        
        // Get the attributes thanks to the index
        glm::vec3 vertex = temp_vertices[ vertexIndex-1 ];
        glm::vec3 normal = temp_normals[ normalIndex-1 ];
        
        // Put the attributes in buffers
        out_vertices.push_back(vertex);
        out_normals .push_back(normal);
    
    }
    objFile.close();
    return true;
}

