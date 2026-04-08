#include "Shader.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_access.hpp>

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
}

void
Camera::uploadVM() const
{
	Shader::setUniform4All("modelView", mViewMat);
}

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
	mRadio = length(mEye - mLook); // Radio de la distancia entre la cámara y el objetivo
	mAng = 0.0f;
	setVM();
}

void
Camera::set3D()
{
	mEye = { 500, 500, 500 };
	mLook = { 0, 10, 0 };
	mUp = { 0, 1, 0 };
	mRadio = length(mEye - mLook); // Radio de la distancia entre la cámara y el objetivo
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
/// imagen, como apuntando con un cubo a un plano, no es lo mismo que el cambiar tamaño
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
Camera::pitchReal(GLfloat cs) {
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(cs), mRight);

	mFront = glm::vec3(mat * glm::vec4(mLook - mEye, 0.0f));
	mUp = glm::normalize(glm::cross(mRight, mFront));

	mLook = mEye + mFront;

	setVM();
}

void
Camera::yawReal(GLfloat cs) {
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(cs), mUpward);

	mFront = glm::vec3(mat * glm::vec4(mLook - mEye, 0.0f));
	mRight = glm::normalize(glm::cross(mUpward, mFront));

	mLook = mEye + mFront;

	setVM();
}

void
Camera::rollReal(GLfloat cs) {
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(cs), mFront);

	mUp = glm::vec3(mat * glm::vec4(mUp, 0.0f));
	mRight = glm::normalize(glm::cross(mFront, mUp));

	setVM();
}

//Ap 46

void 
Camera::orbit(GLdouble incAng, GLdouble incY) {
	mAng += incAng;
	mEye.x = mLook.x + cos(radians(mAng)) * mRadio;
	mEye.z = mLook.z - sin(radians(mAng)) * mRadio;
	mEye.y += incY;
	setVM();
}

//Ap 48

void
Camera::setCenital() {
	mRadio = length(mEye - mLook); // Radio de la distancia entre la cámara y el objetivo
	mLook = { 0, 0, 0 };
	mEye = {0, mRadio, 0}; // Giramos el radio de la vista y lo que se ve
	mUp = { 0, 0, -1 }; // Lo giramos hacia arriba
	setVM();
}