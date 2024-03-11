/**
 * @file Window.hpp
 * @brief Header file for the Window class.
 *
 * @defgroup Window Window class
 * @brief This file defines the Window class, which represents a window for the application.
 */
#pragma once
#include <GL/glew.h>
#include "GLFW/glfw3.h"
#include <optional>
#include <vector>
#include <ctime>
#include <string>
#include "enum.hpp"
#include "glm/glm.hpp"

class Engine;

/**
 * @class Window
 * @brief This class represents a window for the application.
 *
 * The Window class provides functionality to create, manage, and render through a window
 *  used by the application. It exposes methods to handle window creation, destruction,
 *  swapping buffers, setting clear color, managing ImGui integration, handling sound context,
 *  managing time delta, camera management, culling and depth test configuration, retrieving
 *  window size, setting window size, program management, retrieving ImGui integration status,
 *  and resetting the viewport.
 *
 * @ingroup Window
 */
class Window {
public:
  friend class Input;
  friend class Camera;

  /**
   * Destructor for the Window class.
   */
  ~Window();

  /**
   * Copy constructor.
   */
  Window(Window& w);

  /**
  * Move constructor.
  */
  Window(Window&& w) noexcept;

  /**
   * Copy constructor (disabled).
   */
  Window(const Window&) = delete;

  /**
   * Static method to create a Window instance.
   *
   * @param engine Reference to the Engine object.
   * @param w Width of the window.
   * @param h Height of the window.
   * @param title Title of the window (default: "Window").
   * @param imgui Flag to enable ImGui integration (default: false).
   * @return An optional Window object, empty if creation failed.
   */
  static std::optional<Window> create(Engine& engine, int w, int h, const char* title = "Window", bool imgui = false);

  /**
   * Checks if the window should be closed.
   *
   * @return True if the window should be closed, false otherwise.
   */
  bool is_done() const;

  /**
   * Swaps the front and back buffers of the window.
   */
  void swap();

  /**
   * Sets the clear color for the window.
   *
   * @param r Red value (0.0f - 1.0f).
   * @param g Green value (0.0f - 1.0f).
   * @param b Blue value (0.0f - 1.0f).
   * @param a Alpha value (0.0f - 1.0f).
   */
  void clearColor(float r, float g, float b, float a) const;

  /**
   * Updates ImGui frame.
   */
  void updateImGui();

  /**
   * Opens sound device and creates OpenAL context.
   */
  void initSoundContext();

  /**
   * Calculates the time delta since the last frame.
   *
   * @return Time delta in seconds since the last frame.
   */
  float getDeltaTime();

  /**
   * Updates the current time for calculating time delta.
   */
  void calculateCurrentTime();

  /**
   * Updates the last time for calculating time delta.
   */
  void calculateLastTime();

  /**
   * Enables or disables backface culling.
   *
   * Backface culling is an optimization technique that discards geometric primitives
   *  that are facing away from the viewer.
   *
   * @param enable True to enable backface culling, false to disable.
   */
  void enableCulling(bool enable);

  /**
   * Sets the culling mode (front face and backface).
   *
   * Specifies which faces to cull based on the winding order of their vertices.
   *
   * @param culling The culling mode to use 
   * @param frontface The winding order that defines the front face 
   */
  void setCullingMode(CullingMode culling, FrontFace frontface);

  /**
   * Enables or disables depth testing.
   *
   * Depth testing is a technique used to determine which fragment is closer to the camera
   *  when rendering overlapping geometry.
   *
   * @param enable True to enable depth testing, false to disable.
   */
  void enableDepthTest(bool enable);

  /**
   * Sets the depth test mode.
   *
   * Specifies how to compare the fragment's depth with the depth buffer value.
   *
   * @param mode The depth test mode to use .
   */
  void setDepthTestMode(DepthTestMode mode);

  /**
   * Retrieves the current window width and height.
   *
   * @param w Reference to store the window width.
   * @param h Reference to store the window height.
   */
  void getWindowSize(unsigned int& w, unsigned int& h);

  /**
   * Sets the window size.
   *
   * Resizes the window to the specified width and height.
   *
   * @param w New width of the window.
   * @param h New height of the window.
   */
  void setwindowsize(unsigned int w, unsigned int h);

  /**
   * Adds a new shader program to the window's internal list.
   *
   * @param new_program ID of the new program to add.
   */
  void addProgram(unsigned new_program);

  /**
   * Retrieves a shader program from the window's internal list.
   *
   * @param n Index of the program to retrieve (0-based).
   * @return ID of the program at the specified index.
   */
  unsigned getProgram(int n);

  /**
   * Gets the size of the window's shader program list.
   *
   * @return The number of programs stored in the window's internal list.
   */
  int getProgramListSize();

  /**
   * Checks if ImGui integration is enabled for this window.
   *
   * @return True if ImGui is enabled, false otherwise.
   */
  bool getImguiStatus();

  void resetViewport();


private:
  /**
   * Constructor for the Window class (private - use create() method).
   *
   * @param engine Reference to the Engine object.
   * @param w Width of the window.
   * @param h Height of the window.
   * @param title Title of the window.
   * @param imgui Flag to enable ImGui integration.
   */
  Window(Engine& e, int w, int h, const char* title, bool imgui);

  /**
   * Initializes ImGui for the window (if enabled during creation).
   */
  void initImGui();

  GLFWwindow* handle_;
  clock_t  currentTime_;
  clock_t  lastTime_;
  unsigned int width_;
  unsigned int height_;
  float deltaTime_;
  bool imguiInit_;

  size_t current_cam_;

  std::vector<unsigned> program_list_;
  Engine& engine_;
};

