#include "Mesh.h"

using namespace std;
using namespace glm;

// Placeholder for the pending index of a GPU object
constexpr GLuint NONE = numeric_limits<GLuint>::max();

Mesh::Mesh()
 : mVAO(NONE)
 , mVBO(NONE)
 , mCBO(NONE)
{
}

Mesh::~Mesh()
{
	unload();
}

void
Mesh::draw() const
{
	glDrawArrays(
	  mPrimitive,
	  0,
	  size()); // primitive graphic, first index and number of elements to be rendered
}

void
Mesh::load()
{
	assert(mVBO == NONE); // not already loaded

	if (vVertices.size() > 0) { // transfer data
		glGenBuffers(1, &mVBO);
		glGenVertexArrays(1, &mVAO);

		glBindVertexArray(mVAO);
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBufferData(GL_ARRAY_BUFFER, vVertices.size() * sizeof(vec3), vVertices.data(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);

		if (vColors.size() > 0) {             // upload colors
			glGenBuffers(1, &mCBO);

			glBindBuffer(GL_ARRAY_BUFFER, mCBO);
			glBufferData(GL_ARRAY_BUFFER, vColors.size() * sizeof(vec4), vColors.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(vec4), nullptr);
			glEnableVertexAttribArray(1);
		}
	}
}

void
Mesh::unload()
{
	if (mVAO != NONE) {
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		mVAO = NONE;
		mVBO = NONE;

		if (mCBO != NONE) {
			glDeleteBuffers(1, &mCBO);
			mCBO = NONE;
		}
	}
}

void
Mesh::render() const
{
	assert(mVAO != NONE);

	glBindVertexArray(mVAO);
	draw();
}

Mesh*
Mesh::createRGBAxes(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINES;

	mesh->mNumVertices = 6;
	mesh->vVertices.reserve(mesh->mNumVertices);

	// X axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(l, 0.0, 0.0);
	// Y axis vertices
	mesh->vVertices.emplace_back(0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, l, 0.0);
	// Z axis vertices
	mesh->vVertices.emplace_back(0.0, 0.0, 0.0);
	mesh->vVertices.emplace_back(0.0, 0.0, l);

	mesh->vColors.reserve(mesh->mNumVertices);
	// X axis color: red  (Alpha = 1 : fully opaque)
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	// Y axis color: green
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	// Z axis color: blue
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateRegularPolygon(GLuint num, GLdouble r, GLuint offset)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_LINE_LOOP;

	mesh->mNumVertices = num;
	double angle = 90.0;
	// Dividimos un círculo como un pastel y ponemos vértices en cada intersección
	// entre la línea y la circunferencia.
	for (GLuint i = 0; i < num; ++i)
	{
		double x = offset + r * cos(glm::radians(angle));
		double y = 0 + r * sin(glm::radians(angle));
		mesh->vVertices.emplace_back(x, y, 0.0);
		angle += 360 / num;
	
	}

	return mesh;
}
Mesh*
Mesh::generateRGBTriangle(GLdouble h, GLuint offset)
{
	Mesh* mesh = generateRegularPolygon(3, h, offset);
	mesh->mPrimitive = GL_TRIANGLE_STRIP;
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}

Mesh*
Mesh::generateRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLE_STRIP;

	mesh->mNumVertices = 4;
	mesh->vVertices.emplace_back(w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(-w / 2, h / 2, 0.0);
	mesh->vVertices.emplace_back(w / 2, -h / 2, 0.0);
	mesh->vVertices.emplace_back(-w / 2, -h / 2, 0.0);

	return mesh;
}

Mesh*
Mesh::generateRGBRectangle(GLdouble w, GLdouble h)
{
	Mesh* mesh = generateRectangle(w,h);
	mesh->vColors.emplace_back(1.0, 0.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 1.0, 0.0, 1.0);
	mesh->vColors.emplace_back(0.0, 0.0, 1.0, 1.0);

	return mesh;
}


Mesh*
Mesh::generateCube(GLdouble l)
{
	Mesh* mesh = new Mesh();

	mesh->mPrimitive = GL_TRIANGLES;

	mesh->mNumVertices = 36;

	//Cara 1

	mesh->vVertices.emplace_back(l, -l, l);
	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(-l, -l, l);

	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(-l, l, l);
	mesh->vVertices.emplace_back(-l, -l, l);

	//Cara derecha 1

	mesh->vVertices.emplace_back(l, -l, -l);
	mesh->vVertices.emplace_back(l, l, -l);
	mesh->vVertices.emplace_back(l, l, l);

	mesh->vVertices.emplace_back(l, -l, -l);
	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(l, -l, l);

	//Cara arriba

	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(l, l, -l);
	mesh->vVertices.emplace_back(-l, l, -l);

	mesh->vVertices.emplace_back(l, l, l);
	mesh->vVertices.emplace_back(-l, l, -l);
	mesh->vVertices.emplace_back(-l, l, l);

	//Cara opuesta a 1

	mesh->vVertices.emplace_back(-l, -l, -l);
	mesh->vVertices.emplace_back(-l, l, -l);
	mesh->vVertices.emplace_back(l, l, -l);

	mesh->vVertices.emplace_back(l, l, -l);
	mesh->vVertices.emplace_back(l, -l, -l);
	mesh->vVertices.emplace_back(-l, -l, -l);

	//Cara izquierda 1

	mesh->vVertices.emplace_back(-l, -l, l);
	mesh->vVertices.emplace_back(-l, l, l);
	mesh->vVertices.emplace_back(-l, l, -l);

	mesh->vVertices.emplace_back(-l, -l, l);
	mesh->vVertices.emplace_back(-l, l, -l);
	mesh->vVertices.emplace_back(-l, -l, -l);

	//Cara abajo
	mesh->vVertices.emplace_back(-l, -l, -l);
	mesh->vVertices.emplace_back(l, -l, -l);
	mesh->vVertices.emplace_back(l, -l, l);

	mesh->vVertices.emplace_back(-l, -l, -l);
	mesh->vVertices.emplace_back(l, -l, l);
	mesh->vVertices.emplace_back(-l, -l, l);

	return mesh;
}
