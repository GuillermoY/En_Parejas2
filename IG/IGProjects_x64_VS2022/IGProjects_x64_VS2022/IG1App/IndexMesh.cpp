#include "IndexMesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;
using namespace std;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

IndexMesh::IndexMesh()
	: mIBO(NONE)
{
}

// AP 54
void IndexMesh::draw() const
{
	glBindVertexArray(mVAO); // Enlaza el VAO
	// Dibuja la geometría
	glDrawElements(GL_TRIANGLES, vIndexes.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0); // Desactiva el VAO
}

void IndexMesh::load()
{
	Mesh::load();
	if (vVertices.size() > 0 && vIndexes.size() > 0) {
		glGenBuffers(1, &mIBO);
		glBindVertexArray(mVAO);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vIndexes.size() * sizeof(GLuint), vIndexes.data(), GL_STATIC_DRAW);
	}
}

void IndexMesh::unload()
{
	Mesh::unload();
	if (mVAO != NONE) {

		if (mIBO != 0)
		{
			glDeleteBuffers(1, &mIBO);
			mIBO = NONE;
		}
		vIndexes.clear();
	}
}

// AP 55
/// <summary>
/// Creamos un nuevo método estático generateByRevolution en la 
/// clase Mesh con los siguientes atributos :
/// </summary>
/// <param name="profile">  perfil original en el plano XY </param>
/// <param name="nSamples">  número de rotaciones (muestras) que se toman </param>
/// <param name="angleMax"></param>
/// <returns></returns>
IndexMesh* 
IndexMesh::generateByRevolution(
	const std::vector<glm::vec2>& profile, GLuint nSamples,
	GLfloat angleMax)
{
	IndexMesh* mesh = new IndexMesh;
	mesh->mPrimitive = GL_TRIANGLES;
	int tamPerfil = profile.size();
	mesh->vVertices.reserve(nSamples * tamPerfil);
	// Genera los vértices de las muestras
	GLdouble theta1 = angleMax / nSamples;
	for (int i = 0; i <= nSamples; ++i) { // muestra i-ésima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}
	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara
			if (profile[j].x != 0.0) // triángulo inferior
				for (auto [s, t] : { pair{i, j}, {i, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
			if (profile[j + 1].x != 0.0) // triángulo superior
				for (auto [s, t] : { pair{i, j + 1}, {i + 1, j + 1}, {i + 1, j} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
		}
	mesh->mNumVertices = mesh->vVertices.size();


	//AP 60
	mesh->buildNormalVectors();

	return mesh;
} // fin de Mesh::generateByRevolution

// AP 59
void 
IndexMesh::buildNormalVectors() {
	vNormals.clear();
	vNormals.resize(vVertices.size(), vec3(0));
	// Recorrer triángulos (3 índices por cara)
	for (size_t i = 0; i < vIndexes.size(); i += 3) { // De 3 en 3 vértices, como triángulos
		GLuint i0 = vIndexes[i];
		GLuint i1 = vIndexes[i + 1];
		GLuint i2 = vIndexes[i + 2];

		const glm::vec3 v0 = vVertices[i0];
		const glm::vec3 v1 = vVertices[i1];
		const glm::vec3 v2 = vVertices[i2];
		// El vector n normal a una cara formada por los vértices de
		// índices ind0, ind1 e ind2 se puede calcular :
		// Usando el producto vectorial. Sea vi el vértice de índice indi
		glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

		// Acumular en los vértices (suavizado)
		vNormals[i0] += normal; // suma la normal del triángulo
		vNormals[i1] += normal; // a todos sus vértices
		vNormals[i2] += normal;
	}

	// Normalizar las normales por vértice
	// nj = normalize(nj) para j = 1, ... , n
	for (auto& n : vNormals) {
		n = glm::normalize(n);
	}
}