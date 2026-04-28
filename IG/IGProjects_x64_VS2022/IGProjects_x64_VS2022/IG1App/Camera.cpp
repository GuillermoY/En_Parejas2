#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
using namespace glm;

Camera::Camera(Viewport* vp)
	: mViewMat(1.0)
	, mProjMat(1.0)
	, xRight(vp->width() / 2.0)
	, xLeft(-xRight)
	, yTop(vp->height() / 2.0)
	, yBot(-yTop)
	, mViewPort(vp)
{
	setPM();
	uploadVM();
}

void
Camera::uploadVM() const
{
	Shader::setUniform4All("modelView", mViewMat);

	// AP 58: solo si el shader ya existe (evitar crearlo antes de tiempo)
	//glm::vec4 lightDirWorld(-1.0f, -1.5f, -1.25f, 0.0f);
	//glm::vec4 lightDirView = normalize(mViewMat * lightDirWorld);

	//Shader* lightShader = Shader::get("simple_light");
	//lightShader->use();
	//lightShader->setUniform("lightDir", lightDirView);
}

// Recalculamos la matriz de vista
void
Camera::setVM()
{
	mViewMat = lookAt(mEye, mLook, mUp); // glm::lookAt defines the view matrix
	setAxes();
}

void
Camera::set2D()
{
	mEye = { 0, 0, 500 };
	mLook = { 0, 0, 0 };
	mUp = { 0, 1, 0 };
	mRadio = length(mEye - mLook); // Radio de la distancia entre la c�mara y el objetivo
	mAng = 0.0f;
	setVM();
}

void
Camera::set3D()
{
	mEye = { 500, 500, 500 };
	mLook = { 0, 10, 0 };
	mUp = { 0, 1, 0 };
	mRadio = length(mEye - mLook); // Radio de la distancia entre la c�mara y el objetivo
	mAng = 0.0f;
	setVM();
}

void
Camera::pitch(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(1.0, 0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::yaw(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(0, 1.0, 0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::roll(GLfloat a)
{
	mViewMat = rotate(mViewMat, glm::radians(a), glm::vec3(0, 0, 1.0));
	setAxes();
	// glm::rotate returns mViewMat * rotationMatrix
}

void
Camera::setSize(GLdouble xw, GLdouble yh)
{
	xRight = xw / 2.0;
	xLeft = -xRight;
	yTop = yh / 2.0;
	yBot = -yTop;
	setPM();
}

void
Camera::setScale(GLdouble s)
{
	mScaleFact -= s;
	if (mScaleFact < 0)
		mScaleFact = 0.01;
	setPM();
}

//Ap 42
void
Camera::setPM()
{
	if (bOrto) { //  if orthogonal projection
		mProjMat = ortho(xLeft * mScaleFact,
			xRight * mScaleFact,
			yBot * mScaleFact,
			yTop * mScaleFact,
			mNearVal,
			mFarVal);
		// glm::ortho defines the orthogonal projection matrix
	}
	else // if perspective projection
	{
		float aspectRatio = float(mViewPort->width()) / float(mViewPort->height());
		float top = mNearVal * tan(radians(60.0f / 2.0f));
		float right = top * aspectRatio;
		mProjMat = frustum(-right * mScaleFact,
			right * mScaleFact,
			-top * mScaleFact,
			top * mScaleFact,
			mNearVal,
			mFarVal);

		// glm::frustum defines the perspective projection matrix
	}
}

void
Camera::uploadPM() const
{
	Shader::setUniform4All("projection", mProjMat);
}

void
Camera::upload() const
{
	mViewPort->upload();
	uploadVM();
	uploadPM();
}

//Ap 38
// Calculamos los ejes desde la matriz de vista (consistencia global)
void
Camera::setAxes()
{
	mRight = row(mViewMat, 0);
	mUpward = row(mViewMat, 1);
	mFront = -row(mViewMat, 2);
}

//Ap 40
void
Camera::moveLR(GLfloat cs)
{
	mEye += mRight * cs;
	mLook += mRight * cs;
	setVM();
}

void
Camera::moveUD(GLfloat cs)
{
	mEye += mUpward * cs;
	mLook += mUpward * cs;
	setVM();
}

/// <summary>
/// No se mueve en ortogonal porque en la perspectiva se sigue viendo la misma
/// imagen, como apuntando con un cubo a un plano, no es lo mismo que el cambiar tama�o
/// </summary>
void
Camera::moveFB(GLfloat cs)
{
	mEye += mFront * cs;
	mLook += mFront * cs;
	setVM();
}

//Ap 41
void
Camera::changePrj()
{
	bOrto = !bOrto;
	setPM();
}

//Ap 45
void
Camera::pitchReal(GLfloat cs)
{
	mLook = mEye + rotate(mLook - mEye, glm::radians(cs), mRight);
	mUp = rotate(mUp, glm::radians(cs), mRight);

	setVM();
}

void
Camera::yawReal(GLfloat cs)
{
	mLook = mEye + rotate(mLook - mEye, glm::radians(cs), mUpward);
	mUp = rotate(mUp, glm::radians(cs), mUpward);

	setVM();
}

void Camera::rollReal(GLfloat cs)
{
	mUp = rotate(mUp, glm::radians(cs), mFront);

	setVM();
}
//Ap 46

void
Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	glm::vec3 forward = glm::normalize(mLook - mEye);
	glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), forward));
	mUp = glm::cross(forward, right);
	setVM();
}

//Ap 48

void
Camera::setCenital() {
	mRadio = length(mEye - mLook); // Radio de la distancia entre la camara y el objetivo
	mLook = { 0, 0, 0 };
	mEye = { 0, mRadio, 0 }; // Giramos el radio de la vista y lo que se ve
	mUp = { 0, 0, -1 }; // Lo giramos hacia arriba
	setVM();
}