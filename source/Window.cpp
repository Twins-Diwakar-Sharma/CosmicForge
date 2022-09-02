#include "Window.h"

int Window::width = 1366;
int Window::height = 768;


Window::Window()
{
    if(!glfwInit())
    {
        std::cerr << "Failed to init glfw" << std::endl;
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    win = glfwCreateWindow(width, height, title, NULL, NULL);
    if(win == NULL)
    {
        std::cerr<< " Broken Window :("<<std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(win);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cerr << "Failed to initialize glad" <<std::endl;
    glViewport(0,0,width,height);

    glfwSetFramebufferSizeCallback(win,resize);

    glfwSetWindowPos(win,0,0);
    glfwSetInputMode(win,GLFW_CURSOR,GLFW_CURSOR_HIDDEN);

}

void Window::resize(GLFWwindow* w, int width, int height)
{
    Window::width = width;
    Window::height = height;
    glViewport(0,0,width,height);
    proj::setPerspective(proj::fov,proj::near,proj::far,(float)width/height);
}

bool Window::close()
{
    return glfwWindowShouldClose(win);
}

void Window::terminate()
{
    glfwTerminate();
}

void Window::swap()
{
    glfwSwapBuffers(win);
}

float Window::getAspect()
{
    return (float)Window::width/Window::height;
}

void Window::handleKey(float& t, float& s, float val)
{
    if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(win, true);

	if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS)
		t += val;
	else if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS)
		t -= val;

	if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS)
		s -= val;
	else if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS)
		s += val;

	glfwPollEvents();
}

void Window::handleMouse(float& rx, float& ry)
{
	double x, y;
	glfwGetCursorPos(win, &x, &y);
	ry += -(x - width / 2) * senstivity; 
	rx += -(y - height / 2) * senstivity;
	
	glfwSetCursorPos(win, (double)width / 2, (double)height / 2);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::handleHold(bool& hold)
{
    if(glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        hold = true;
    if(glfwGetKey(win, GLFW_KEY_H) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        hold = false;
    
}

void Window::handleWireframe(bool& wireframe)
{
    if(glfwGetKey(win, GLFW_KEY_O) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        wireframe = true;
    if(glfwGetKey(win, GLFW_KEY_O) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        wireframe = false;
}

void Window::handleTerrainUpdate(bool& terrainUpdate)
{
    if(glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
        terrainUpdate = true;
    if(glfwGetKey(win, GLFW_KEY_L) == GLFW_PRESS && glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        terrainUpdate = false;
}

int Window::getWidth()
{
    return Window::width;
}

int Window::getHeight()
{
    return Window::height;
}