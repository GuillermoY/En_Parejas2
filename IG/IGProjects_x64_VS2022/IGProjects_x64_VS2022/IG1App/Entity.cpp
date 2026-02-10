#include "Entity.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

void
Abs_Entity::upload(const mat4& modelViewMat) const
{
	mShader->setUniform("modelView", modelViewMat);
}

Abs_Entity::~Abs_Entity()
{
	delete mMesh;
	mMesh = nullptr;
}

void
Abs_Entity::load()
{
	mMesh->load();
}

void
Abs_Entity::unload()
{
	mMesh->unload();
}

EntityWithColors::EntityWithColors()
{
	mShader = Shader::get("vcolors");
}

void
EntityWithColors::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);
		mMesh->render();
	}
}

void
SingleColorEntity::render(mat4 const& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat;
		mShader->use();
		upload(aMat);
		mShader->setUniform("color", mColor); // Cargamos color en la GPU 
		mMesh->render();
	}
}

RGBAxes::RGBAxes(GLdouble l)
{
	mMesh = Mesh::createRGBAxes(l);
}

RegularPolygon::RegularPolygon(GLuint num, GLdouble r, GLuint offset, glm::vec4 color) : SingleColorEntity(color)
{
	mMesh = Mesh::generateRegularPolygon(num, r, offset);
}

RGBTriangle::RGBTriangle(GLdouble h, GLuint offset) : EntityWithColors()
{
	mMesh = Mesh::generateRGBTriangle(h, offset);
}

RGBRectangle::RGBRectangle(GLdouble w, GLdouble h)
{
	mMesh = Mesh::generateRGBRectangle(w, h);
}
Cube::Cube(GLdouble l)
{
	mMesh = Mesh::generateCube(l);
}

/// <summary>
/// Apartado 9:
/// Redefinimos el render para RGBRectangle, que, usando culling, hacemos que la trasera
/// está rellena y la delantera use líneas
/// </summary>
void RGBRectangle::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE); 
		glFrontFace(GL_CCW); // El sentido de la cara frontal es Counter ClockWise (Antihorario)
		
		// Quitamos cara delantera y rellenamos la trasera
		glCullFace(GL_FRONT); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		mMesh->render();

		// Quitamos cara trasera y ponemos modo l�nea a la delantera
		glCullFace(GL_BACK); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		// Habilitamos las dos caras, cada una con su relleno particular
		glDisable(GL_CULL_FACE); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	}
}
void Cube::render(const glm::mat4& modelViewMat) const
{
	if (mMesh != nullptr) {
		mat4 aMat = modelViewMat * mModelMat; // glm matrix multiplication
		mShader->use();
		upload(aMat);

		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW); // El sentido de la cara frontal es Counter ClockWise (Antihorario)

		glCullFace(GL_FRONT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		mMesh->render();


		glCullFace(GL_BACK);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		mMesh->render();

		glDisable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	}
}

/// <summary>
/// Apartado 13:
/// Sumamos 1 a la rotaci�n del tri�ngulo en sentido horario, que se rotar� despu�s de 
/// trasladarse en la direcci�n antihoraria cuando va circulando sobre la circunferencia
/// </summary>
void RGBTriangle::update()
{
	selfRotation += 1;
	double y = 0 + orbitDiameter * sin(glm::radians(angle));
	angle += 1.0f;
	mModelMat = glm::mat4(1.0);
	mModelMat = translate(mModelMat, glm::vec3(0, y, 0.0));
	mModelMat = rotate(mModelMat, glm::radians(selfRotation), glm::vec3(0, 0, -1.0f));
}
