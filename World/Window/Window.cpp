#include "Window.h"

#include <assert.h>
#include <iostream>
#include <tools\Log.h>
#include <gui\imgui_impl_glfw_gl3.h>
#include <AutoVersion.h>

Window::Window(const glm::uvec2 &size)
{
	mSize = size;
	std::cout << "Start window creating" << std::endl;

	GLFWmonitor *monitor = nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, -1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, -1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 0);

	mWindow = (decltype(mWindow))(glfwCreateWindow(mSize.x, mSize.y, AutoVersion::GetTitle().c_str(), monitor, nullptr));

	if (!mWindow)
	{
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 0);

		mWindow = (decltype(mWindow))(glfwCreateWindow(mSize.x, mSize.y, AutoVersion::GetTitle().c_str(), monitor, nullptr));
	}

	if (!mWindow)
	{
		throw "Window not created.";
	}

	// Привязываем к glfw окну указатель на объект WindowGL.
	glfwSetWindowUserPointer(mWindow.get(), this);

	glfwSetKeyCallback(mWindow.get(), [](GLFWwindow *win, int key, int scancode, int action, int mods)
	{
		Window *window = static_cast<Window *>(glfwGetWindowUserPointer(win));
		assert(window);
		ImGui_ImplGlfwGL3_KeyCallback(win, key, scancode, action, mods);
	});

	glfwSetMouseButtonCallback(mWindow.get(), ImGui_ImplGlfwGL3_MouseButtonCallback);
	glfwSetScrollCallback(mWindow.get(), ImGui_ImplGlfwGL3_ScrollCallback);
	glfwSetCharCallback(mWindow.get(), ImGui_ImplGlfwGL3_CharCallback);
	glfwSetWindowSizeCallback(mWindow.get(), ResizeCallback);

	ResizeCallback(mWindow.get(), size.x, size.y);
	std::cout << "Window created" << std::endl;
}

Window::~Window()
{
}

void Window::WindowSystemInitialize()
{
	glfwSetErrorCallback([](int, const char* description)
	{
		std::cout << description << std::endl;
	});

	if (glfwInit() != GL_TRUE)
	{
		throw "GLFW not initialized.";
	}

	LOG(info) << "GLFW: " << glfwGetVersionString();
}

void Window::WindowSystemFinally()
{
	glfwTerminate();
}

void Window::SetCurrentContext()
{
	assert(mWindow);
	glfwMakeContextCurrent(mWindow.get());

	glfwSwapInterval(1);

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl
		<< "OpenGL version supported: " << version << std::endl;
}

bool Window::WindowShouldClose()
{
	assert(mWindow);
	return glfwWindowShouldClose(mWindow.get()) == GL_TRUE;
}

void Window::Update()
{
	assert(mWindow);
	glfwSwapBuffers(mWindow.get());

	glfwPollEvents();
}

const glm::uvec2 Window::GetSize() const
{
	return mSize;
}

const glm::uvec2 Window::GetFbSize() const
{
	return fbsize;
}

void Window::SetTitle(const std::string &title)
{
	assert(mWindow);
	glfwSetWindowTitle(mWindow.get(), title.c_str());
}
GLFWwindow * Window::Get()
{
	return mWindow.get();
}

void Window::SetResizeCallback(const std::function<void(int, int)>& f)
{
	mResf = f;
}

void Window::ResizeCallback(GLFWwindow *, int x, int y)
{
	glfwGetFramebufferSize(mWindow.get(), &fbsize.x, &fbsize.y);
	mSize = { x, y };
	if (mResf)
		mResf(x, y);
}

std::function<void(int, int)> Window::mResf;
glm::uvec2 Window::mSize;
glm::ivec2 Window::fbsize;
std::unique_ptr<GLFWwindow, Window::WindowDeleter> Window::mWindow;


