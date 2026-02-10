#ifndef _H_Entities_H_
#define _H_Entities_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Mesh.h"
#include "Shader.h"

class Abs_Entity // abstract class
{
public:
	Abs_Entity()
	  : mModelMat(1.0)  // 4x4 identity matrix
	  , mShader(nullptr) {};
	virtual ~Abs_Entity();

	Abs_Entity(const Abs_Entity& e) = delete;            // no copy constructor
	Abs_Entity& operator=(const Abs_Entity& e) = delete; // no copy assignment

	virtual void render(const glm::mat4& modelViewMat) const = 0; // abstract method
	virtual void update(){}; // Apartado 12: A�adimos un update para que los objetos se actualicen
	// modeling matrix
	glm::mat4 const& modelMat() const { return mModelMat; };
	void setModelMat(glm::mat4 const& aMat) { mModelMat = aMat; };

	// load or unload entity data into the GPU
	void load();
	void unload();

protected:
	Mesh* mMesh = nullptr; // the mesh
	glm::mat4 mModelMat;  // modeling matrix
	Shader* mShader; // shader

	// transfers modelViewMat to the GPU
	virtual void upload(const glm::mat4& mModelViewMat) const;
};

class EntityWithColors : public Abs_Entity
{
public:
	explicit EntityWithColors();
	virtual void render(const glm::mat4& modelViewMat) const override;
	virtual void update() {};
};

class RGBAxes : public EntityWithColors
{
public:
	explicit RGBAxes(GLdouble l);
};

/// <summary>
/// Apartado 3:
/// Creamos clase con mColor (por defecto blanco), que es el color de la entidad
/// 
/// Tambi�n creamos getters y setters para leer o modificar sus atributos
/// </summary>
class SingleColorEntity : public Abs_Entity
{
private:
	glm::vec4 mColor;

public:
	SingleColorEntity(glm::vec4 color = glm::vec4(1.0f))
		: mColor(color)
	{
		mShader = Shader::get("simple"); // Funcionar� con shader simple
	} 
	virtual void render(const glm::mat4& modelViewMat) const override; // Sobreescribimos render 

	glm::vec4 getColor() const { return mColor; };
	void setColor(glm::vec4 newColor) { mColor = newColor; };
};

class RegularPolygon : public SingleColorEntity // Apartado 4
{
public:
	explicit RegularPolygon(GLuint num, GLdouble r, GLuint offset, glm::vec4 color = glm::vec4(1.0f));
};

class RGBTriangle : public EntityWithColors
{
public:
	explicit RGBTriangle(GLdouble h, GLuint offset);
	void update() override;
	double angle = 0.0f;
	float selfRotation = 0.0f;
	float orbitDiameter = 400.0f;
	//static Mesh* generateRectangle(GLdouble w, GLdouble h);
};

class RGBRectangle : public EntityWithColors
{
public:
	explicit RGBRectangle(GLdouble w, GLdouble h);
	void render(const glm::mat4& modelViewMat) const override;
	//static Mesh* generateRectangle(GLdouble w, GLdouble h);
};
class Cube : public SingleColorEntity
{
public:
	explicit Cube(GLdouble l);
	void render(const glm::mat4& modelViewMat) const override;
};

#endif //_H_Entities_H_
