#include "Water.h"
#include "stb_image.h"

Water::Water(std::string dudv, std::string normal, int h)
{
	// DUDV Texture Grab
	unsigned int tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	int width, height, nrChannels;
	unsigned char* data = stbi_load(dudv.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load dudvMap" << std::endl;
	}
	stbi_image_free(data);
	dudv_map = tex;

	// Normal Map Texture Grab
	unsigned int tex2;
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	int width2, height2, nrChannels2;
	unsigned char* data2 = stbi_load(normal.c_str(), &width2, &height2, &nrChannels2, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load normalMap" << std::endl;
	}
	stbi_image_free(data2);
	normal_map = tex2;

	// Location of the water level
	height = h;

	 // Water Polygon
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-1000, -height, 1000),
		//glm::vec3(-100, -100, 100),
		//glm::vec3(100, -100, 100),
		glm::vec3(1000, -height, 1000),
		glm::vec3(-1000, -height, -1000),
		//glm::vec3(-100, -100, -100),
		//glm::vec3(100, -100, -100),
		glm::vec3(1000, -height, -1000)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		glm::ivec3(1, 2, 0),
		glm::ivec3(1, 3, 2),
	};

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	reflectionFrameBuffer = createFrameBuffer();
	reflectionTexture = createTextureAttachment(1200, 800);
	unbindFrameBuffer(1200, 800);

	refractionFrameBuffer = createFrameBuffer();
	refractionTexture = createTextureAttachment(300, 200);
	unbindFrameBuffer(1200, 800);

	// Animate Distortions
	animation_offset = 0;
}

Water::~Water()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
	glDeleteFramebuffers(1, &reflectionFrameBuffer);
	glDeleteTextures(1, &reflectionTexture);
	glDeleteFramebuffers(1, &refractionFrameBuffer);
	glDeleteTextures(1, &refractionTexture);
}

void Water::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Water::draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 projection, glm::vec3 eye)
{
	glUseProgram(shaderProgram);
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
	GLuint eyeLoc = glGetUniformLocation(shaderProgram, "eye");
	GLuint timeLoc = glGetUniformLocation(shaderProgram, "time");
	GLuint lightLoc = glGetUniformLocation(shaderProgram, "light");

	glUniform1i(glGetUniformLocation(shaderProgram, "reflectionTexture"), 0);
	glUniform1i(glGetUniformLocation(shaderProgram, "refractionTexture"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "dudvTexture"), 2);
	glUniform1i(glGetUniformLocation(shaderProgram, "normalTexture"), 3);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, dudv_map);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, normal_map);

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform3f(eyeLoc, eye.x, eye.y, eye.z);
	glUniform3f(lightLoc, 0.0f, 99.0f, -99.0f);
	glUniform1f(timeLoc, animation_offset);

	animation_offset += 0.002;

	draw();
}

void Water::update()
{

}

void Water::unbindFrameBuffer(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

void Water::bindReflectionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(reflectionFrameBuffer, 1200, 800);
}

void Water::bindRefractionFrameBuffer() {//call before rendering to this FBO
	bindFrameBuffer(refractionFrameBuffer, 300, 200);
}

GLuint Water::getReflectionTexture() {//get the resulting texture
	return reflectionTexture;
}

GLuint Water::getRefractionTexture() {//get the resulting texture
	return refractionTexture;
}

void Water::bindFrameBuffer(int frameBuffer, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glViewport(0, 0, width, height);
}

GLuint Water::createFrameBuffer()
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	return frameBuffer;
}

GLuint Water::createTextureAttachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
	return texture;
}
