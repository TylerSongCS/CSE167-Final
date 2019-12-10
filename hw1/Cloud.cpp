#include "Cloud.h"

Cloud::Cloud(std::string objFilename)
{
	std::ifstream objFile(objFilename); // The obj file we are reading.
	std::vector<glm::ivec3> indices_vert;
	std::vector<glm::ivec3> indices_norm;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> normals;

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

			glm::vec3 normal;

			// If the line is about vertex (starting with a "v").
			if (label == "v")
			{
				// Read the later three float numbers and use them as the 
				// coordinates.
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;

				// Process the point. For example, you can save it to a.
				points.push_back(point);
			}

			// If the line is about a normal (starting with a "vn").
			if (label == "vn")
			{
				ss >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}

			// If the line is about a face (starting with a "f").
			if (label == "f")
			{
				glm::ivec3 point;

				std::string token;
				int parsed[3];
				int parsedNormals[3];
				int i = 0;
				while (std::getline(ss, token, ' '))
				{
					if (token.compare("") == 0) continue;

					std::stringstream parser(token.substr(0, token.find('/')));
					parser >> parsed[i];

					std::stringstream parserNormals(token.substr(token.rfind('/') + 1));
					parserNormals >> parsedNormals[i];

					i++;
				}

				point.x = (parsed[0] - 1);
				point.y = (parsed[1] - 1);
				point.z = (parsed[2] - 1);

				// Process the point. For example, you can save it to a.
				indices_vert.push_back(point);

				point.x = (parsedNormals[0] - 1);
				point.y = (parsedNormals[1] - 1);
				point.z = (parsedNormals[2] - 1);
				indices_norm.push_back(point);
			}
		}
	}
	else
	{
		std::cerr << "Can't open the file " << objFilename << std::endl;
	}
	objFile.close();

	// Offsets for centering the model
	float xOffset = 0;
	float yOffset = 0;
	float zOffset = 0;

	// Traverse through the vertices and find the maximum and minimum;
	if (!points.empty())
	{
		float xMax = points.at(0).x;
		float yMax = points.at(0).y;
		float zMax = points.at(0).z;
		float xMin = points.at(0).x;
		float yMin = points.at(0).y;
		float zMin = points.at(0).z;
		for (unsigned int i = 0; i < points.size(); i++)
		{
			if (points.at(i).x > xMax) xMax = points.at(i).x;
			if (points.at(i).y > yMax) yMax = points.at(i).y;
			if (points.at(i).z > zMax) zMax = points.at(i).z;
			if (points.at(i).x < xMin) xMin = points.at(i).x;
			if (points.at(i).y < yMin) yMin = points.at(i).y;
			if (points.at(i).z < zMin) zMin = points.at(i).z;
		}

		// Find the center points
		xOffset = (xMax + xMin) / 2;
		yOffset = (yMax + yMin) / 2;
		zOffset = (zMax + zMin) / 2;
	}

	// Center the model and find max distance
	double maxDist = 0;
	for (unsigned int i = 0; i < points.size(); i++)
	{
		points.at(i).x -= xOffset;
		points.at(i).y -= yOffset;
		points.at(i).z -= zOffset;

		float x = points.at(i).x;
		float y = points.at(i).y;
		float z = points.at(i).z;

		double distance = std::sqrt(x * x + y * y + z * z);
		if (maxDist < distance) maxDist = distance;
	}

	// Scale the model
	for (unsigned int i = 0; i < points.size(); i++)
	{
		points.at(i).x *= (float)(2 / maxDist);
		points.at(i).y *= (float)(2 / maxDist);
		points.at(i).z *= (float)(2 / maxDist);
	}

	// Reorder the indices to match with vertices and normals
	for (unsigned i = 0; i < indices_vert.size(); i++) {
		glm::ivec3 face;
		vertices.push_back(points.at(indices_vert.at(i).x));
		normal_vertices.push_back(normals.at(indices_norm.at(i).x));
		face.x = i * 3;

		vertices.push_back(points.at(indices_vert.at(i).y));
		normal_vertices.push_back(normals.at(indices_norm.at(i).y));
		face.y = i * 3 + 1;

		vertices.push_back(points.at(indices_vert.at(i).z));
		normal_vertices.push_back(normals.at(indices_norm.at(i).z));
		face.z = i * 3 + 2;

		indices.push_back(face);
	}

	// Set the model matrix to an identity matrix. 
	model = glm::mat4(1);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(3, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the points.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the normals.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normal_vertices.size(),
		normal_vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access points through it.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the third VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	// Time
	timeAnimation = 0;
	srand(time(NULL));
	setNoise(map32);
}

Cloud::~Cloud()
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Cloud::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glDrawElements(GL_TRIANGLES, indices.size() * 3, GL_UNSIGNED_INT, 0);

	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Cloud::update()
{

}

void Cloud::draw(GLuint shaderProgram, glm::mat4 view, glm::mat4 projection, float speed)
{
	overlapOctaves(map32, map256);
	expFilter(map256);

	std::vector<unsigned char> texture(256 * 256 * 4);
	for (int i = 0; i < 256; i++)         //Set cloud color value to temporary array
		for (int j = 0; j < 256; j++)
		{
			float color = map256[i * 256 + j];
			texture.at((i + (j * 256)) * 4) = 255;
			texture.at((i + (j * 256)) * 4 + 1) = 255;
			texture.at((i + (j * 256)) * 4 + 2) = 255;
			texture.at((i + (j * 256)) * 4 + 3) = color;
		}

	unsigned int ID;                 //Generate an ID for texture binding                     
	glGenTextures(1, &ID);           //Texture binding 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texture[0]);

	glUseProgram(shaderProgram);

	// Specify the values of the uniform variables we are going to use.
	GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
	GLuint timeLoc = glGetUniformLocation(shaderProgram, "Time");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::rotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::scale(glm::vec3(240.0f, 240.0f, 80.0f))));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(timeLoc, timeAnimation);

	timeAnimation += speed;

	draw();
}


void Cloud::setNoise(float* map)
{
	float temp[34][34];
	int random = rand() % 5000;

	for (int y = 1; y < 33; y++)
		for (int x = 1; x < 33; x++)
		{
			temp[x][y] = 128.0f + noise(x, y, random) * 128.0f;
		}
	for (int x = 1; x < 33; x++)
	{
		temp[0][x] = temp[32][x];
		temp[33][x] = temp[1][x];
		temp[x][0] = temp[x][32];
		temp[x][33] = temp[x][1];
	}
	temp[0][0] = temp[32][32];
	temp[33][33] = temp[1][1];
	temp[0][33] = temp[32][1];
	temp[33][0] = temp[1][32];

	for (int y = 1; y < 33; y++)
		for (int x = 1; x < 33; x++)
		{
			float center = temp[x][y] / 4.0f;
			float sides = (temp[x + 1][y] + temp[x - 1][y] + temp[x][y + 1] + temp[x][y - 1]) / 8.0f;
			float corners = (temp[x + 1][y + 1] + temp[x + 1][y - 1] + temp[x - 1][y + 1] + temp[x - 1][y - 1]) / 16.0f;

			map[((x - 1) * 32) + (y - 1)] = center + sides + corners;
		}
}

void Cloud::expFilter(float* map)
{
	float cover = 20.0f;
	float sharpness = 0.95f;

	for (int x = 0; x < 256 * 256; x++)
	{
		float c = map[x] - (255.0f - cover);
		if (c < 0)     c = 0;
		map[x] = 255.0f - ((float)(pow(sharpness, c)) * 255.0f);
	}
}

void Cloud::overlapOctaves(float* map32, float* map256)
{
	for (int x = 0; x < 256 * 256; x++)
	{
		map256[x] = 0;
	}
	for (int octave = 0; octave < 4; octave++)
		for (int x = 0; x < 256; x++)
			for (int y = 0; y < 256; y++)
			{
				float scale = 1 / pow(2, 3 - octave);
				float noise = interpolate(x * scale, y * scale, map32);
				map256[(y * 256) + x] += noise / pow(2, octave);
			}
}

float Cloud::interpolate(float x, float y, float* map)
{
	int Xint = (int)x;
	int Yint = (int)y;

	float Xfrac = x - Xint;
	float Yfrac = y - Yint;

	int X0 = Xint % 32;
	int Y0 = Yint % 32;
	int X1 = (Xint + 1) % 32;
	int Y1 = (Yint + 1) % 32;

	float bot = map[X0 * 32 + Y0] + Xfrac * (map[X1 * 32 + Y0] - map[X0 * 32 + Y0]);
	float top = map[X0 * 32 + Y1] + Xfrac * (map[X1 * 32 + Y1] - map[X0 * 32 + Y1]);

	return (bot + Yfrac * (top - bot));
}

float Cloud::noise(int x, int y, int random)
{
	int n = x + y * 57 + random * 131;
	n = (n << 13) ^ n;
	return (1.0f - ((n * (n * n * 15731 + 789221) +
		1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
}
void Cloud::changeRandom()
{
    srand(time(NULL));
    setNoise(map32);
}
