#include "parser.hpp"

parse_result_t parse_objectfile(std::string filename) {
    
    std::cout << "... loading " << filename;
    
    std::ifstream objFile(filename); // The obj file we are reading.
    parse_result_t parse_result;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    int curr_index = 0;

    std::vector<glm::vec3> vertex_indices;
    std::vector<glm::vec3> normal_indices;
    
    triangles_t triangles;
    triangles.num_triangles = 0;
    
    // Check whether the file can be opened.
    if (objFile.is_open()) {
        std::string line; // A line in the file.
        
        // Read lines from the file.
        while (std::getline(objFile, line)) {
            // Turn the line into a string stream for processing.
            std::stringstream ss;
            ss << line;
            
            // Read the first word of the line.
            std::string label;
            ss >> label;
            
            // If the line is about vertex (starting with a "v").
            if (label == "v") {
                // Read the later three float numbers and use them as the
                // coordinates.
                glm::vec3 point;
                float x, y, z;
                ss >> x >> y >> z;
                
                points.push_back(glm::vec3(x, y, z));
            } else if (label == "vn") {
                
                glm::vec3 vn;
                float x, y, z;
                ss >> x >> y >> z;
                
                /*
                x *= x;
                y *= y;
                z *= z;
                
                
                float normal = sqrt(x + y + z);
                x /= normal;
                y /= normal;
                z /= normal;
                */
                
                normals.push_back(glm::vec3(x, y, z));
                
            } else if (label == "f") {
                
                // http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
                // https://stackoverflow.com/questions/21120699/c-obj-file-parser
                
                int v1, v2, v3;
                int vn1, vn2, vn3;
                const char* chh = line.c_str();
                sscanf(chh, "f %d//%d %d//%d %d//%d",&v1,&vn1,&v2,&vn2,&v3,&vn3);
                
                glm::ivec3 face = glm::ivec3(v1-1, v2-1, v3-1);
                
                indices.push_back(face);
                
                glm::vec3 v = glm::vec3(v1-1, v2-1, v3-1);
                glm::vec3 f = glm::vec3(vn1-1, vn2-1, vn3-1);
                vertex_indices.push_back(v);
                normal_indices.push_back(f);
                
                triangles.num_triangles++;
            }
        }
    }
    else {
        std::cerr << "Can't open the file " << filename << std::endl;
    }
    
    objFile.close();
    
    std::cout << " | Done\n";
    
    parse_result.point_cloud = points;
    parse_result.normals = normals;
    parse_result.indices = indices;
    
    parse_result.triangles = triangles;
    
    return parse_result;
}
