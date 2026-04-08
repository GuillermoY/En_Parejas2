#ifndef _H_IG1App_H_
#define _H_IG1App_H_

#include <GL/glew.h>     // OpenGL Extension Wrangler
#include <GLFW/glfw3.h>  // OpenGL Library
#include <glm/glm.hpp>   // OpenGL Mathematics

#include "Camera.h"
#include "Scene.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Viewport.h"

class IG1App
{
public:
	// static single instance (singleton pattern)
	static IG1App s_ig1app;

	IG1App(IG1App const& J) = delete;         // no copy constructor
	void operator=(IG1App const& J) = delete; // no copy assignment

	// Viewport position and size
	Viewport const& viewPort();
	// Camera position, view volume and projection
	Camera const& camera();
	// Graphics objects of the scene
	Scene const& scene();

	// Change to the given scene
	bool changeScene(size_t sceneNr);

	void run();   // the main event processing loop
	void close(); // the application

protected:
	IG1App() = default;
	~IG1App() { close(); };

	void init();
	void iniWinOpenGL();
	void destroy();

	void display() const;                      // the scene
	void resize(int newWidth, int newHeight);  // the viewport (without changing the scale)
	void key(unsigned int codepoint);          // keypress event
	void specialkey(int key, int scancode, int action, int mods); // special keypress event

	// static callbacks
	static void s_display(GLFWwindow*) { s_ig1app.display(); };
	static void s_resize(GLFWwindow*, int newWidth, int newHeight) { s_ig1app.resize(newWidth, newHeight); };
	static void s_key(GLFWwindow* win, unsigned int codepoint) { s_ig1app.key(codepoint); };
	static void s_specialkey(GLFWwindow* win, int key, int scancode, int action, int mods) { s_ig1app.specialkey(key, scancode, action, mods); };
	//AP 51 static callbacks
	static void s_mouse(GLFWwindow*, int button, int state, int mods) { s_ig1app.mouse(button,state,mods); };
	static void s_motion(GLFWwindow*, double x, double y) { s_ig1app.motion(x,y); };
	static void s_mouseWheel(GLFWwindow*, double dx, double dy) { s_ig1app.mouseWheel(dx,dy); };

	// Viewport position and size
	Viewport* mViewPort = nullptr;
	// Camera position, view volume and projection
	Camera* mCamera = nullptr;
	// Graphics objects are in the scenes
	std::vector<Scene*> mScenes;
	size_t mCurrentScene = 0;

	bool mNeedsRedisplay = true;   // main event processing loop
	GLFWwindow* mWindow = nullptr; // window's handle
	int mWinW = 800;               // window's width
	int mWinH = 600;               // window's height
	const double FRAME_DURATION = 0.01f; // Cada frame durará 0.01 s
	bool mUpdateEnabled = false; // Empezamos la escena con el update en falso
	double mNextUpdate;

	//AP 49
	void alterRenderViews();
	bool m2Vistas=false;
	Camera* mLeftCamera = nullptr;
	Camera* mRightCamera = nullptr;
	Viewport* mLeftViewPort = nullptr;
	Viewport* mRightViewPort = nullptr;

	Camera* mMainCam;
	bool mLeftSide;


	//AP 50
	glm::dvec2 mMouseCoord;
	int mMouseButt=-1;

	//AP 51 
	void mouse(int button, int state, int mods);
	void motion(double x, double y);
	void mouseWheel(double dx, double dy);

	//AP 52
	size_t mLeftScene = 4;
	size_t mRightScene = 2;
};

inline Viewport const&
IG1App::viewPort()
{
	//return *mViewPort;
	if (!m2Vistas)
	{
		return *mViewPort;
	}
	else
	{
		if (mLeftSide)
		{
			return *mLeftViewPort;
		}
		else
		{
			return *mRightViewPort;
		}
	}
}

inline Camera const&
IG1App::camera()
{
	return *mCamera;
}

inline Scene const&
IG1App::scene()
{
	return *mScenes[mCurrentScene];
}

#endif //_H_IG1App_H_
