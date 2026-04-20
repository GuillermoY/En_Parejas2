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
	// Dibuja la geometr�a
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
/// Creamos un nuevo m�todo est�tico generateByRevolution en la 
/// clase Mesh con los siguientes atributos :
/// </summary>
/// <param name="profile">  perfil original en el plano XY </param>
/// <param name="nSamples">  n�mero de rotaciones (muestras) que se toman </param>
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
	// Genera los v�rtices de las muestras
	GLdouble theta1 = angleMax / nSamples;
	for (int i = 0; i <= nSamples; ++i) { // muestra i-�sima
		GLdouble c = cos(i * theta1), s = sin(i * theta1);
		for (auto p : profile) // rota el perfil
			mesh->vVertices.emplace_back(p.x * c, p.y, -p.x * s);
	}
	for (int i = 0; i < nSamples; ++i) // caras i a i + 1
		for (int j = 0; j < tamPerfil - 1; ++j) { // una cara
			if (profile[j].x != 0.0)
				// triángulo inferior (orden invertido)
				for (auto [s, t] : { pair{i, j}, {i + 1, j}, {i, j + 1} })
					mesh->vIndexes.push_back(s * tamPerfil + t);
			if (profile[j + 1].x != 0.0)
				// triángulo superior (orden invertido)
				for (auto [s, t] : { pair {i, j + 1}, {i + 1, j}, {i + 1, j + 1} })
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
	// Recorrer tri�ngulos (3 �ndices por cara)
	for (size_t i = 0; i < vIndexes.size(); i += 3) { // De 3 en 3 v�rtices, como tri�ngulos
		GLuint i0 = vIndexes[i];
		GLuint i1 = vIndexes[i + 1];
		GLuint i2 = vIndexes[i + 2];

		const glm::vec3 v0 = vVertices[i0];
		const glm::vec3 v1 = vVertices[i1];
		const glm::vec3 v2 = vVertices[i2];
		// El vector n normal a una cara formada por los v�rtices de
		// �ndices ind0, ind1 e ind2 se puede calcular :
		// Usando el producto vectorial. Sea vi el v�rtice de �ndice indi
		glm::vec3 normal = glm::cross(v1 - v0, v2 - v0);

		// Acumular en los v�rtices (suavizado)
		vNormals[i0] += normal; // suma la normal del tri�ngulo
		vNormals[i1] += normal; // a todos sus v�rtices
		vNormals[i2] += normal;
	}

	// Normalizar las normales por v�rtice
	// nj = normalize(nj) para j = 1, ... , n
	for (auto& n : vNormals) {
		n = glm::normalize(n);
	}
}
// ---- AP 61 ----
IndexMesh*
IndexMesh::generateIndexedBox8(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;

	mesh->vVertices = {
		{-l, -l, -l}, { l, -l, -l}, { l,  l, -l}, {-l,  l, -l},
		{-l, -l,  l}, { l, -l,  l}, { l,  l,  l}, {-l,  l,  l}
	};

	mesh->vIndexes = {
		0, 2, 1,  0, 3, 2,   // Z-
		4, 5, 6,  4, 6, 7,   // Z+
		0, 7, 3,  0, 4, 7,   // X-
		1, 2, 6,  1, 6, 5,   // X+
		0, 1, 5,  0, 5, 4,   // Y-
		3, 7, 6,  3, 6, 2    // Y+
	};

	mesh->mNumVertices = (GLuint)mesh->vVertices.size();
	mesh->buildNormalVectors();
	return mesh;
}

// ---- AP 64 ----
// 24 vértices (4 por cara, sin compartir) con normales introducidas a mano.
// Cada cara tiene exactamente la normal perpendicular correcta.
IndexMesh*
IndexMesh::generateIndexedBox(GLdouble l)
{
	IndexMesh* mesh = new IndexMesh();
	mesh->mPrimitive = GL_TRIANGLES;

	// Cara Z+ (normal: 0,0,+1)
	mesh->vVertices.insert(mesh->vVertices.end(), { {-l,-l, l},{ l,-l, l},{ l, l, l},{-l, l, l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { 0,0,1 });
	// Cara Z- (normal: 0,0,-1)
	mesh->vVertices.insert(mesh->vVertices.end(), { { l, l,-l},{-l, l,-l},{-l,-l,-l},{ l,-l,-l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { 0,0,-1 });
	// Cara X+ (normal: +1,0,0)
	mesh->vVertices.insert(mesh->vVertices.end(), { { l,-l, l},{ l,-l,-l},{ l, l,-l},{ l, l, l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { 1,0,0 });
	// Cara X- (normal: -1,0,0)
	mesh->vVertices.insert(mesh->vVertices.end(), { {-l, l,-l},{-l, l, l},{-l,-l, l},{-l,-l,-l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { -1,0,0 });
	// Cara Y+ (normal: 0,+1,0)
	mesh->vVertices.insert(mesh->vVertices.end(), { {-l, l, l},{ l, l, l},{ l, l,-l},{-l, l,-l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { 0,1,0 });
	// Cara Y- (normal: 0,-1,0)
	mesh->vVertices.insert(mesh->vVertices.end(), { {-l,-l, l},{ l,-l, l},{ l,-l,-l},{-l,-l,-l} });
	mesh->vNormals.insert(mesh->vNormals.end(), 4, { 0,-1,0 });

	// 2 triángulos por cara × 6 caras = 36 índices
	for (GLuint i = 0; i < 6; ++i) {
		GLuint b = i * 4;
		mesh->vIndexes.insert(mesh->vIndexes.end(), { b,b + 1,b + 2, b,b + 2,b + 3 });
	}

	mesh->mNumVertices = (GLuint)mesh->vVertices.size();
	return mesh;
}