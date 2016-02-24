// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#ifndef WINDOW_GL_H
#define WINDOW_GL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include "Keyboard.h"
#include "Mouse.h"
#include <functional>


class Window
{
public:
  Window(const glm::uvec2 &size);
  ~Window();

  /// Инициализация оконной системы.
  static void WindowSystemInitialize();

  /// Завершить работу с оконной системой.
  static void WindowSystemFinally();

  /// Установить текущий контекст для данного окна.
  void SetCurrentContext();

  /// Должно ли окно закрыться?
  bool WindowShouldClose();

  void Update();

  const glm::uvec2 &GetSize() const;

  /// Установить имя окна.
  void SetTitle(const std::string &title);

  /// Получить клавиатуру.
  Keyboard &GetKeyboard();

  /// Получить мышь.
  Mouse &GetMouse();

  GLFWwindow *Get();

  void SetResizeCallback(const std::function<void(int, int)> &f);

private:

  struct WindowDeleter
  {
    void operator()(GLFWwindow *window) const
    {
      printf("window delete\n");
      glfwDestroyWindow(window);
    }
  };

  static void ResizeCallback(GLFWwindow*, int, int);
  static std::function<void(int, int)> mResf;

  std::unique_ptr<GLFWwindow, WindowDeleter> mWindow;

  std::unique_ptr<Keyboard> mKeyboard;

  std::unique_ptr<Mouse> mMouse;

  glm::uvec2 mSize;

};

#endif // WINDOW_GL_H
