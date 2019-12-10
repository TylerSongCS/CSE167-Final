#include "terrain.hpp"


Terrain::Terrain() {

    init();
    generate_terrain();
    calculate_normals();
    calculate_indices();
    calculate_texture_coordinates();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
	glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texture_coords.size(), texture_coords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * z_buffer.size(), z_buffer.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float) * 3, indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Terrain::~Terrain() {

}

void Terrain::init() {

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbos[0]);
    glGenBuffers(1, &vbos[1]);
    glGenBuffers(1, &vbos[2]);
    glGenBuffers(1, &ebo);   
    
    rows = 1000;
    cols = 1000;

    height = cols;
    width = rows;
}

void Terrain::generate_terrain() {

    PerlinNoise *pn = new PerlinNoise(0.05, 0.2f, 45.f, 5, rand() % 100); 

    glm::vec3 v;
    for (float x = 0.0; x < rows; x++) {
        for (float y = 0.0; y < cols; y++) {
            
            float z = pn->GetHeight(x, y);

            v = glm::vec3(x, y, z);
            vertices.push_back(v);
            z_buffer.push_back(glm::vec2(z, z));
        }
    }
}

void Terrain::calculate_normals() {

    std::vector<std::vector<glm::vec3>> v_normals[2];

	for (int i = 0; i < 2; i++)
		v_normals[i] = std::vector< std::vector<glm::vec3> >(height - 1, std::vector<glm::vec3>(width - 1));

	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {
            
			glm::vec3 v_triangle0[] = {
				vertices[h * width + w],
				vertices[(h + 1) * width + w],
				vertices[(h + 1) * width + (w + 1)]
			};
			glm::vec3 v_triangle1[] = {
				vertices[(h + 1) * width + (w + 1)],
				vertices[h * width + (w + 1)],
				vertices[h * width + w]
			};

			v_normals[0][h][w] = calculate_surface_normal(v_triangle0[0], v_triangle0[1], v_triangle0[2]);
			v_normals[1][h][w] = calculate_surface_normal(v_triangle1[0], v_triangle1[1], v_triangle1[2]);
		}
	}

	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {

			glm::vec3 f_norm;

			if (h != 0 && w != 0)
				for (int k = 0; k < 2; k++)
					f_norm += v_normals[k][h - 1][w - 1];

			if (h != 0 && w != width - 1)
				f_norm += v_normals[0][h - 1][w];

			if (h != height - 1 && w != height - 1)
				for (int k = 0; k < 2; k++)
					f_norm += v_normals[k][h][w];

			if (h != height - 1 && w != 0)
				f_norm += v_normals[1][h][w - 1];

			normals.push_back(glm::normalize(f_norm));
		}
	}
}

void Terrain::calculate_indices() {
    
	for (int h = 0; h < height - 1; h++) {
		for (int w = 0; w < width - 1; w++) {
			indices.push_back(h * width + w);
			indices.push_back((h + 1) * width + w);
			indices.push_back((h + 1) * width + (w + 1));

			indices.push_back((h + 1) * width + (w + 1));
			indices.push_back(h * width + (w + 1));
			indices.push_back(h * width + w);
		}
	}
}

void Terrain::calculate_texture_coordinates() {

    for (glm::vec3 vertex: vertices) {

        glm::vec3 mv = glm::normalize(vertex * 0.5f + 0.5f); // map to [0,1]
        glm::vec2 uv = glm::vec2(mv.x, mv.y);
        
        texture_coords.push_back(uv);
    }
}

glm::vec3 Terrain::calculate_surface_normal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    
    glm::vec3 U = p2 - p1;
    glm::vec3 V = p3 - p1;

    return glm::normalize(glm::cross(U, V));
}

void Terrain::draw(GLuint shader, glm::mat4 P, glm::mat4 V, std::vector<unsigned int> textureIDs) {

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(0.0f, 1.0f, 0.0f);
    
    model = glm::translate(model, glm::vec3(-500.0f, -10.0f, 950.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));

    GLuint projectionLoc = glGetUniformLocation(shader, "projection");
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    GLuint viewLoc = glGetUniformLocation(shader, "view");
    GLuint colorLoc = glGetUniformLocation(shader, "color");
    
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(P));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(V));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    // load textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureIDs[0]);
    GLuint tex_loc = glGetUniformLocation(shader, "sand_texture");
    glUniform1i(tex_loc, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    tex_loc = glGetUniformLocation(shader, "grass_texture");
    glUniform1i(tex_loc, 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureIDs[2]);
    tex_loc = glGetUniformLocation(shader, "snow_texture");
    glUniform1i(tex_loc, 2);
    
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, textureIDs[3]);
    tex_loc = glGetUniformLocation(shader, "rock_texture");
    glUniform1i(tex_loc, 3);
    // draw terrain
    glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
    
    // reset
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_BLEND);
}

bool Terrain::check_collision(glm::vec3 position) {

    int z_terrain;
    int pos_y = round(position.y);
    int pos_x = round(position.x);
    int pos_z = round(position.z);

    //std::cout << "POS: " << position.x << " " << position.y << " " << position.z << "\n";

    for (std::vector<glm::vec3>::const_iterator vert = vertices.begin(); vert != vertices.end(); vert++) {

        int trans_x = vert->x * cos(-90.0f) - vert->y * sin(-90.0f);
        int trans_y = vert->x * sin(-90.0f) + vert->y * cos(-90.0f);

        if (pos_x == trans_x && pos_z == trans_y) {

            std::cout << pos_y << " " << vert->z << "\n";

            if (pos_y < vert->z)
                return true;
        }
    }

    return false;
}

void Terrain::regenerate() {

    vertices.clear();
    normals.clear();
    indices.clear();
    texture_coords.clear();
    z_buffer.clear();

    generate_terrain();
    calculate_normals();
    calculate_indices();
    calculate_texture_coordinates();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbos[0]);
    glGenBuffers(1, &vbos[1]);
    glGenBuffers(1, &vbos[2]);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid *)0);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texture_coords.size(), texture_coords.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * z_buffer.size(), z_buffer.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float) * 3, indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
