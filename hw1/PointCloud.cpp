#include "PointCloud.h"
#include "main.h"
#include <sstream>
#include <cmath>
#include "objparser.hpp"

using namespace std;
PointCloud::PointCloud(std::string objFilename, GLfloat pointSize) 
	: pointSize(pointSize)
{
	/* 
	 *read points from an obj file.
	 */
    if(objParse(objFilename, points, normals)){
        std::cerr << "obj parse successful" << std::endl;
    }

    
    for (std::vector<glm::vec3>::iterator it = normals.begin(); it != normals.end(); ++it) {
        float red = it->x;
        float green = it->y;
        float blue = it->z;
        colors.push_back(glm::vec3(red, green, blue));
    }
    GLfloat minX;
    GLfloat maxX;
    GLfloat minY;
    GLfloat maxY;
    GLfloat minZ;
    GLfloat maxZ;
    GLboolean init = false;
    for (std::vector<glm::vec3>::iterator it = points.begin(); it != points.end(); ++it) {
        if (!init) {
            minX = it->x;
            maxX = it->x;
            minY = it->y;
            maxY = it->y;
            minZ = it->z;
            maxZ = it->z;
            init = true;
        }
        else {
            if (it->x > maxX) {
                maxX = it->x;
            }
            else if (it->x < minX) {
                minX = it->x;
            }
            if (it->y > maxY) {
                maxY = it->y;
            }
            else if (it->y < minY) {
                minY = it->y;
            }
            if (it->z > maxZ) {
                maxZ = it->z;
            }
            else if (it->z < minZ) {
                minZ = it->z;
            }
        }
    }
    GLfloat centerX = (maxX + minX) / 2.0;
    GLfloat centerY = (maxY + minY) / 2.0;
    GLfloat centerZ = (maxZ + minZ) / 2.0;
    
    /*
     * normalize the object to fit in the screen.
     */
    
    GLfloat maxDistance = 0;
    GLfloat scalar = 10;
    
    for (std::vector<glm::vec3>::iterator it = points.begin(); it != points.end(); ++it) {
        it->x = it->x - centerX;
        it->y = it->y - centerY;
        it->z = it->z - centerZ;
        GLfloat distance = sqrt(pow(it -> x, 2) + pow(it->y, 2) + pow(it->z, 2));
        if (  distance > maxDistance){
            maxDistance = distance;
        }

    }
    for (std::vector<glm::vec3>::iterator it = points.begin(); it != points.end(); ++it) {
        it->x = it->x / maxDistance;
        it->y = it->y / maxDistance;
        it->z = it->z / maxDistance;
        
        it->x = it->x * scalar;
        it->y = it->y * scalar;
        it->z = it->z * scalar;
    }
    srand (static_cast <unsigned> (time(0)));
    float low = -7;
    float high = 7;
    for (int i = 0; i < points.size(); i++) {
        glm::vec3 point2;
        point2.x = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
        point2.y = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
        point2.z = low + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(high-low)));
        

        randomPoints.push_back(point2);
        midPoints.push_back(point2);
    }
        

    weight = 0;

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);
	// Set the color. 
	color = glm::vec3(1, 1, 1);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
    glGenBuffers(1, &colorbuffer);
    glGenBuffers(1, &normalbuffer);
    
	// Bind to the VAO.
	glBindVertexArray(vao);
    

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
		points.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    
    //Bind to color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * colors.size(), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                  // attribute
        3,                                  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        3 * sizeof(GLfloat),
        0                                   // array buffer offset
    );
    
    //Bind to normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glVertexAttribPointer(
        2,                                  // attribute
        3,                                  // size
        GL_FLOAT,                           // type
        GL_FALSE,                           // normalized?
        3 * sizeof(GLfloat),
        0                                   // array buffer offset
    );

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the colorbuffer.
    glBindBuffer(GL_ARRAY_BUFFER, 1);
    // Unbind from the normalbuffer.
    glBindBuffer(GL_ARRAY_BUFFER, 2);
	// Unbind from the VAO.
	glBindVertexArray(0);
    
}
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}
PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

	// Set point size.
	glPointSize(pointSize);
    
	// Draw points
	glDrawArrays(GL_TRIANGLES, 0, points.size());
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void PointCloud::update()
{
    //beginSquareAnim();
    // Spin the cube by 1 degree.
    //spin(0.01f);

}
void PointCloud::beginSquareAnim(){
	if (weight <= 1.0) {
		lerp(weight);
		weight += 0.0015;
	}
}
void PointCloud::lerp(float weight)
{
    std::vector<glm::vec3>::iterator it = points.begin();
    std::vector<glm::vec3>::iterator it2 = randomPoints.begin();
    std::vector<glm::vec3>::iterator it3 = midPoints.begin();
    for(int i = 0; i < points.size(); ++i ){
        *(it3+i) = *(it2+i) + weight * (*(it+i) - *(it2+i));
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * midPoints.size(),
        midPoints.data(), GL_STATIC_DRAW);
}
void PointCloud::updatePointSize(GLfloat size) 
{
	pointSize = size;
}
void PointCloud::updateScale(float size)
{
    float factor = 1.005f;
    if(size < 0){
        factor = 1/factor;
    }
    glm::mat3 scale(factor);
    for (std::vector<glm::vec3>::iterator it = points.begin(); it != points.end(); ++it) {
        *it = scale * (*it);
    }
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
}

void PointCloud::updateRotation(glm::vec3 rotAxis, float rot_angle){
    
    
    model = glm::rotate(glm::radians(rot_angle), rotAxis) * model;
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

