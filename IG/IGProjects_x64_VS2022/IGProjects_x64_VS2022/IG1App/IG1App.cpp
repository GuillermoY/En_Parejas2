#include "IG1App.h"

#include <iostream>

using namespace std;

// static single instance (singleton pattern)
IG1App IG1App::s_ig1app; // default constructor (constructor with no parameters)

// Print OpenGL errors and warnings
void GLAPIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity,
                   GLsizei length, const GLchar* message, const void* userParam)
{
	const char* prefix = (type == GL_DEBUG_TYPE_ERROR)
		? "\x1b[31m[ERROR]\x1b[0m "
		: "\x1b[33m[WARNING]\x1b[0m ";
	cout << prefix << message << endl;
}

void
IG1App::close()
{
	cout << "Closing GLFW...\n";
	glfwSetWindowShouldClose(mWindow, true); // stops main loop
}

void IG1App::run()
{
	if (mWindow == 0) // if not initialized
		init();

	mNextUpdate = glfwGetTime() + FRAME_DURATION; // Iniciamos el mNextUpdate con el tiempo actual mas la duraci�n del frame

	while (!glfwWindowShouldClose(mWindow)) {
		double time = glfwGetTime(); // Obtenemos el tiempo actual

		if (mNeedsRedisplay) {
			display(); 
			mNeedsRedisplay = false;
		}

		if (mUpdateEnabled)
		{
			if (time > mNextUpdate)
			{
				mScenes[mCurrentScene]->update();
				mNeedsRedisplay = true; // representamos la actualizaci�n
				mNextUpdate += FRAME_DURATION;
			}

			double timeout = mNextUpdate - time;

			glfwPollEvents();
			//glfwWaitEventsTimeout(timeout);
		}
		else
		{
			glfwWaitEvents();
		}
	}

	destroy();
}

void
IG1App::init()
{
	// create an OpenGL Context
	iniWinOpenGL();

	// create the scene after creating the context
	// allocate memory and resources
	mViewPort = new Viewport(mWinW, mWinH);
	mCamera = new Camera(mViewPort);

	// En el apartado 6 he metido Scene1 para cuando le das al 1 carga la escena 1.
	mScenes.push_back(new Scene);
	mScenes.push_back(new Scene1);
	// Lo mismo para las siguientes escenas
	mScenes.push_back(new Scene2);
	mScenes.push_back(new Scene3);
	mScenes.push_back(new Scene4);
	mCamera->set2D();
	for (int i = 0; i < mScenes.size(); ++i)
	{
		mScenes[i]->init();
	}
	mScenes[mCurrentScene]->load();
}

void
IG1App::iniWinOpenGL()
{ // Initialization
	cout << "Starting GLFW...\n";
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // enable on macOS
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	// Create window with its associated OpenGL context, return window's pointer
	mWindow = glfwCreateWindow(mWinW, mWinH, "IG1App", nullptr, nullptr);

	if (mWindow == nullptr) {
		glfwTerminate();
		throw std::logic_error("cannot create GLFW window");
	}

	glfwMakeContextCurrent(mWindow);

	// Initialize OpenGL extension library
	if (GLenum err = glewInit(); err != GLEW_OK) {
		glfwTerminate();
		throw std::logic_error("Error while loading extensions: "s +
		                       reinterpret_cast<const char*>(glewGetErrorString(err)));
	}

	// Callback registration
	glfwSetWindowSizeCallback(mWindow, s_resize);
	glfwSetCharCallback(mWindow, s_key);
	glfwSetKeyCallback(mWindow, s_specialkey);
	glfwSetWindowRefreshCallback(mWindow, s_display);
	//AP 51
	glfwSetMouseButtonCallback(mWindow, s_mouse);
	glfwSetCursorPosCallback(mWindow, s_motion);
	glfwSetScrollCallback(mWindow, s_mouseWheel);

	// Error message callback (all messages)
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0u, 0, GL_TRUE);
	glDebugMessageCallback(debugCallback, nullptr);

	cout << glGetString(GL_VERSION) << '\n';
	cout << glGetString(GL_VENDOR) << '\n';
}

void
IG1App::destroy()
{ // release memory and resources
	for (Scene* scene : mScenes)
		delete scene;
	mScenes.clear();

	delete mCamera;
	mCamera = nullptr;
	delete mViewPort;
	mViewPort = nullptr;

	glfwTerminate();
}

void
IG1App::display() const
{ // double buffering

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clears the back buffer

	if (m2Vistas)
	{
		Camera auxCam = *mCamera;
		Viewport auxVp = *mViewPort;
		mViewPort->setSize(mWinW / 2, mWinH);

		//Vista 3D
		auxCam.setSize(mViewPort->width(), mViewPort->height());
		mViewPort->setPos(0, 0);
		auxCam.set3D();
		mScenes[mCurrentScene]->render(auxCam);

		//Vista cenital
		Camera auxCamTop = *mCamera;
		auxCamTop.setSize(mViewPort->width(), mViewPort->height());
		mViewPort->setPos(mWinW / 2, 0);
		auxCamTop.setCenital();
		auxCamTop.changePrj();
		mScenes[mCurrentScene]->render(auxCamTop);
		*mViewPort = auxVp;
	}
	else
	{
		mScenes[mCurrentScene]->render(*mCamera); // uploads the viewport and camera to the GPU
	}

	glfwSwapBuffers(mWindow); // swaps the front and back buffer
}

void
IG1App::resize(int newWidth, int newHeight)
{
	mWinW = newWidth;
	mWinH = newHeight;

	// Resize Viewport to the new window size
	mViewPort->setSize(newWidth, newHeight);

	// Resize Scene Visible Area such that the scale is not modified
	mCamera->setSize(mViewPort->width(), mViewPort->height());
}

void
IG1App::key(unsigned int key)
{
	bool need_redisplay = true;

	switch (key) {
		case '+':
			mCamera->setScale(+0.01); // zoom in  (increases the scale)
			break;
		case '-':
			mCamera->setScale(-0.01); // zoom out (decreases the scale)
			break;
		case 'l':
			mCamera->set3D();
			break;
		case 'o':
			mCamera->set2D();
			break;
		case 'U':
			mUpdateEnabled = !mUpdateEnabled; // Al pulsar la 'u' pausamos o reanudaremos el update
			mNextUpdate = glfwGetTime() + FRAME_DURATION; // Iniciamos el mNextUpdate para la siguiente vez que se actualizar
			break;
		case 'u':
			// Apartado 12.3: Al mantener la 'u' se actualiza el update
			mScenes[mCurrentScene]->update();
			break;
		case 'a':
			mCamera->moveLR(-5);
			break;
		case 'd':
			mCamera->moveLR(+5);
			break;
		case 'w':
			mCamera->moveUD(+5);
			break;
		case 's':
			mCamera->moveUD(-5);
			break;
		case 'W':
			mCamera->moveFB(+5);
			break;
		case 'S':
			mCamera->moveFB(-5);
			break;
		case 'p':
			mCamera->changePrj();
			break;
		case 'k': // AP 49
			alterRenderViews();
			break;
		default:
			if (key >= '0' && key <= '9') {
				if (changeScene(key - '0')) break;
				cout << "[NOTE] There is no scene " << char(key) << ".\n";
			}
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

void
IG1App::specialkey(int key, int scancode, int action, int mods)
{
	// Only interested in press events
	if (action == GLFW_RELEASE)
		return;

	bool need_redisplay = true;

	// Handle keyboard input
	// (key reference: https://www.glfw.org/docs/3.4/group__keys.html)
	switch (key) {
		case GLFW_KEY_ESCAPE:                     // Escape key
			glfwSetWindowShouldClose(mWindow, true); // stops main loop
			break;
		case GLFW_KEY_RIGHT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(+5);
			else
				mCamera->yawReal(+10);
				//mCamera->pitch(-1); // rotates -1 on the X axis
				//mCamera->pitch(1); // rotates 1 on the X axis
			break;
		case GLFW_KEY_LEFT:
			if (mods == GLFW_MOD_CONTROL)
				mCamera->rollReal(-5);
			else
				mCamera->yawReal(-10);
				//mCamera->yaw(1); // rotates 1 on the Y axis
				//mCamera->yaw(-1); // rotate -1 on the Y axis
			break;
		case GLFW_KEY_UP:
			mCamera->pitchReal(+10);
			//mCamera->roll(1); // rotates 1 on the Z axis
			break;
		case GLFW_KEY_DOWN:
			mCamera->pitchReal(-10);
			//mCamera->roll(-1); // rotates -1 on the Z axis
			break;
		default:
			need_redisplay = false;
			break;
	} // switch

	if (need_redisplay)
		mNeedsRedisplay = true;
}

bool
IG1App::changeScene(size_t sceneNr)
{
	// Check whether the scene exists
	if (sceneNr >= mScenes.size())
		return false;

	// Change only if a different scene
	if (sceneNr != mCurrentScene) {
		mScenes[mCurrentScene]->unload();
		mCurrentScene = sceneNr;
		mScenes[mCurrentScene]->load();
	}

	return true;
}

//AP 49
void
IG1App::alterRenderViews()
{
	m2Vistas = !m2Vistas;
}

//AP 51
///
/// Se genera cuando se presiona o se suelta (action) un botón
/// del ratón(button) (con alguna tecla de modificación pulsada,
/// mods).La posición se puede consultar con glfwGetCursorPos .
///
void 
IG1App::mouse(int button, int state, int mods) {
	if (state == GLFW_PRESS) {
		//1 Guarda en mBot el valor de button
		mMouseButt = button;
		//2 Guarda en mCoord la posición(x, y) del ratón.
		glfwGetCursorPos(mWindow, &mMouseCoord.x, &mMouseCoord.y);
	}
	else
	{
		mMouseButt = -1;
	}
}

//Se genera cuando el ratón se mueve y recibe su posición (x, y)
//en coordenadas de la ventana.
void 
IG1App::motion(double x, double y) {
	//1 Guarda en una variable auxiliar mp la diferencia entre
	//mCoord y (x, y)
	glm::dvec2 mp = { mMouseCoord.x-x ,mMouseCoord.y-y};
	//2 Guarda en mCoord la posición(x, y) del ratón
	
	//glfwGetCursorPos(mWindow, &mMouseCoord.x, &mMouseCoord.y);
	mMouseCoord = { x,y };

	//3 Si mBot es el botón izquierdo, la cámara orbita
	//(mp.x * 0.05, mp.y)
	if (mMouseButt==GLFW_MOUSE_BUTTON_LEFT)
	{
		mCamera->orbit(mp.x*0.05,-mp.y);
	}
	//4 Si mBot es el botón derecho, la cámara se desplaza
	//moveUD() y moveLR() según mp
	else if (mMouseButt == GLFW_MOUSE_BUTTON_RIGHT)
	{
		mCamera->moveLR(mp.x);
		mCamera->moveUD(-mp.y);
	}
	//5 mNeedsRedisplay = true;
	mNeedsRedisplay = true;
}

// Se genera cuando se mueve la rueda del ratón o equivalente
// en dx unidades horizontales y dy verticales
void 
IG1App::mouseWheel(double dx, double dy) {
	//	1 Averigua si algún modificador está pulsado con
//	glfwGetKey()
	if (glfwGetKey(mWindow, GLFW_KEY_RIGHT_CONTROL) ||
		glfwGetKey(mWindow, GLFW_KEY_LEFT_CONTROL))
	{
		mCamera->setScale(dy);
	}
	//	2 Si no hay ninguna, la cámara se mueve con moveFB(),
	//	según el valor de dy
	else
	{
		mCamera->moveFB(dy);
	}
	//	3 Si está pulsada la tecla Ctrl(GLFW_MOD_CONTROL), la cámara
	//	cambia la escala con setScale(), según el valor de dy
	//	4 mNeedsRedisplay = true;
	mNeedsRedisplay = true;
}