#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: vertices(vertices), indices(indices), textures(textures)
{

}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// activate proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);
		// retrieve texture number (the N in diffuse_textureN)
		std::string number;
		std::string name = textures[i].type;
		if (name == "diffuse_texture") { number = std::to_string(diffuseNr++); }
		else if (name == "specular_texture") { number = std::to_string(specularNr++); }

		shader.setInt(("material." + name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].ID);
	}
	glActiveTexture(GL_TEXTURE0);

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// Vertex positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	// Vertex normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	glEnableVertexAttribArray(1);

	// Vertex texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
