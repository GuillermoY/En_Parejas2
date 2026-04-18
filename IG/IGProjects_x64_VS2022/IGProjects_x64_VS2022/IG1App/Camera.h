#ifndef _H_Camera_H_
#define _H_Camera_H_

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "Viewport.h"
#include "Shader.h"
class Camera
{
public:
	explicit Camera(Viewport* vp);
	~Camera() = default;

	// viewPort
	Viewport const& viewPort() const { return *mViewPort; };

	// view matrix
	glm::mat4 const& viewMat() const { return mViewMat; };

	void set2D();
	void set3D();

	void pitch(GLfloat a); // rotates a degrees on the X axis
	void yaw(GLfloat a);   // rotates a degrees on the Y axis
	void roll(GLfloat a);  // rotates a degrees on the Z axis

	// projection matrix
	glm::mat4 const& projMat() const { return mProjMat; };

	// sets scene visible area size
	void setSize(GLdouble xw, GLdouble yh);
	// updates the scale factor
	void setScale(GLdouble s);

	// transfers its viewport, the view matrix and projection matrix to the GPU
	void upload() const;

	//AP 40
	void moveLR(GLfloat cs); // Izquierda / Derecha [a,d]
	void moveFB(GLfloat cs); // Alante / Atrás [w,s]
	void moveUD(GLfloat cs); // Arriba / Abajo [W,S]

	//AP 41
	void changePrj(); // cambia proyección ortogonal a perspectiva

	//AP 45 Inclinaciones
	void pitchReal(GLfloat cs); // Arriba / Abajo [flecha arriba, abajo] 
	void yawReal(GLfloat cs); // Left / Right [<- , -> si no tiene la tecla "Control"]
	void rollReal(GLfloat cs); // Alante / Atrás [<- , -> si tiene la tecla "Control"]

	//AP 46
	void orbit(GLdouble incAng, GLdouble incY); // Desplaza la cámara a lo largo de una circunferencia 
												// Situada sobre el plano XZ

	//AP 48
	void setCenital();

protected:
	glm::vec3 mEye = {0.0, 0.0, 500.0}; // camera's position
	glm::vec3 mLook = {0.0, 0.0, 0.0};  // target's position
	glm::vec3 mUp = {0.0, 1.0, 0.0};    // the up vector

	//Ap 38
	glm::vec3 mRight, mUpward, mFront;
	void setAxes();

	//Ap 46
	GLdouble mRadio, mAng; // Atributos para gestionar radio y ángulo de la circunferencia


	glm::mat4 mViewMat;   // view matrix = inverse of modeling matrix
	void uploadVM() const; // transfers viewMat to the GPU

	glm::mat4 mProjMat;   // projection matrix
	void uploadPM() const; // transfers projMat to the GPU

	GLfloat xRight, xLeft, yTop, yBot;     // size of scene visible area
	GLfloat mNearVal = 1, mFarVal = 10000; // view volume
	GLfloat mScaleFact = 1;                // scale factor
	bool bOrto = true;                      // orthogonal or perspective projection

	Viewport* mViewPort; // the viewport

	void setVM();
	void setPM();

	// AP 58

};

#endif //_H_Camera_H_
