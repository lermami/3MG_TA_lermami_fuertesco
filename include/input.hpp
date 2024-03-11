/**
 * @file input.hpp
 * @brief Header file for the Key and Input class.
 *
 * @defgroup Input Input class
 * @brief This file defines the Input class and Key, managing the input events.
 */
#pragma once

#include "GLFW/glfw3.h"
#include <vector>

class Window;
class Input;

/**
 * @brief Represents a key and its input state.
 */
class Key {
  friend class Input;
public:

  /**
     * @brief Default destructor for Key.
     */
  ~Key() = default;

  /**
     * @brief Sets the state of the key.
     *
     * @param state The new state of the key.
     */
  void setState(InputState state);

  /**
     * @brief Gets the current state of the key.
     *
     * @return The current state of the key.
     */
  InputState getState() const;

  /**
     * @brief Sets the key code for this Key object.
     *
     * @param key The key code to set.
     */
  void setKey(int key);

  /**
     * @brief Gets the key code associated with this Key object.
     *
     * @return The key code.
     */
  int getKey() const;
private:

  /**
     * @brief Private constructor for Key.
     * Used exclusively by the Input class.
     */
  Key();
  InputState state_; /**< The current state of the key. */
  int key_; /**< The key code. */
};

/**
 * @brief Manages input for the application, including keyboard, mouse, and scroll events.
 */
class Input {
public:

  /**
     * @brief Constructor for Input class.
     * Takes a reference to the GLFWwindow object to handle input events.
     *
     * @param w Reference to the GLFWwindow object.
     */
  Input(Window& w);


  /**
   * @brief Destructor for Input class.
   * Performs any necessary cleanup for the input system.
   */
  ~Input();


  /**
   * @brief Checks if a character key is currently pressed down.
   *
   * @param key The character key to check.
   * @return True if the key is down, false otherwise.
   */
  bool IsKeyDown(char key);

  /**
     * @brief Checks if a character key was just pressed in the current frame.
     *
     * @param key The character key to check.
     * @return True if the key was pressed this frame, false otherwise.
     */
  bool IsKeyPressed(char key);

  /**
     * @brief Checks if a character key is currently not pressed down.
     *
     * @param key The character key to check.
     * @return True if the key is up this frame, false otherwise.
     */
  bool IsKeyUp(char key);

  /**
     * @brief Checks if an integer key code is currently pressed down.
     *
     * @param key The integer key code to check.
     * @return True if the key is down, false otherwise.
     */
  bool IsKeyDown(int key);

  /**
     * @brief Checks if an integer key code was just pressed in the current frame.
     *
     * @param key The integer key code to check.
     * @return True if the key was pressed this frame, false otherwise.
     */
  bool IsKeyPressed(int key);

  /**
     * @brief Checks if an integer key code is currently not pressed down.
     *
     * @param key The integer key code to check.
     * @return True if the key is up this frame, false otherwise.
     */
  bool IsKeyUp(int key);

  /**
     * @brief Updates the internal state of the input class.
     * This function should be called every frame to ensure accurate input information.
     */
  void updateInputs();

  /**
     * @brief Sets the scroll offset for the window.
     *
     * @param scroll_x The new scroll offset on the X-axis.
     * @param scroll_y The new scroll offset on the Y-axis.
     */
  void setScroll(double scroll_x, double scroll_y);

  /**
     * @brief Gets the current scroll offset for the window.
     *
     * @param scroll_x Reference to a double variable to store the X-axis scroll offset.
     * @param scroll_y Reference to a double variable to store the Y-axis scroll offset.
     */
  void getScroll(double& scroll_x, double& scroll_y);

  /**
     * @brief Gets the current mouse position within the window.
     *
     * @param x Reference to a double variable to store the X-coordinate of the mouse.
     * @param y Reference to a double variable to store the Y-coordinate of the mouse.
     */
  void getMousePos(double &x, double &y);

private:
  GLFWwindow* windowHandle_; /**< Handle to the GLFW window object. */
  Key input_[88]; /**< Array of Key objects to store information for each key. */

  /**
     * @brief GLFW scroll callback function.
     * This function is called internally by GLFW whenever the window is scrolled.
     *
     * @param window The GLFWwindow object that generated the scroll event.
     * @param xoffset The scroll offset on the X-axis.
     * @param yoffset The scroll offset on the Y-axis.
     */
  static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

  inline static double scroll_x_; /**< Internal variable to store the current scroll offset on the X-axis. */
  inline static double scroll_y_; /**< Internal variable to store the current scroll offset on the Y-axis. */

  double mouse_x_; /**< Internal variable to store the current X-coordinate of the mouse. */
  double mouse_y_; /**< Internal variable to store the current Y-coordinate of the mouse. */
};

