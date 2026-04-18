#ifndef INDEXMESH_H
#define INDEXMESH_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <numbers>

#include "Mesh.h"
class Mesh;

class IndexMesh : public Mesh

{
private:

public:
	IndexMesh();
	~IndexMesh() {};
	void buildNormalVectors();

	void load() override;
	void unload() override;
	static IndexMesh* generateByRevolution(const std::vector<glm::vec2>& profile, GLuint nSamples, GLfloat angleMax = 2 * std::numbers::pi);
protected:
	void draw() const override;

	std::vector<GLuint> vIndexes;
	GLuint mIBO;

	//explicit RGBCube(GLdouble l);
};
#endif // INDEXMESH_H