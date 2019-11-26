#include "OBJ.hpp"

OBJ::OBJ(float size, std::string filename)
{
    m_size = size;
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    
    parse_result_t parse_result = parse_objectfile(filename);
    num_triangles = parse_result.triangles.num_triangles;

    std::vector<glm::vec3> vertices = parse_result.point_cloud;
    std::vector<glm::ivec3> indices = parse_result.indices;
    std::vector<glm::vec3> normals = parse_result.normals;

    // create array object and buffers
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbos[0]);
    glGenBuffers(1, &vbos[1]);
    glGenBuffers(1, &ebo);

    // bind vertex array buffer
    glBindVertexArray(vao);

    // bind vbo 0
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
   
    // set buffer data
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // enable layout location 0
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    // bind vbo 1
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);

    // enable layout location 1
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float) * 3, indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

OBJ::~OBJ()
{
    // Delete the VBOs and the VAO.
    glDeleteBuffers(2, vbos);
    glDeleteVertexArrays(1, &vao);
}

void OBJ::draw()
{

    model = glm::mat4(1.0f);
    model = glm::scale(glm::vec3(m_size / 2.f));
    model = glm::translate(glm::mat4(1.0f), position) * model;

    // Bind to the VAO.
    glBindVertexArray(vao);
    // Draw triangles using the indices in the second VBO, which is an
    // elemnt array buffer.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, (unsigned int)1);
    glDrawElements(GL_TRIANGLES, num_triangles * 3, GL_UNSIGNED_INT, 0);

    // Unbind from the VAO.
    glBindVertexArray(0);
}

void OBJ::update()
{
    // Spin the cube by 1 degree.
    //spin(0.1f);
}

void OBJ::spin(float deg)
{
    // Update the model matrix by multiplying a rotation matrix
    //model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJ::set_position(glm::vec3 pos) {
 
    position = pos;
}

void OBJ::set_cubemapTexture(unsigned int ct) {
    //ct2 = ct;
}
