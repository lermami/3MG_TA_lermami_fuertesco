/**
 * @file ShaderLoader.hpp
 * @brief Header file for shader loading functionality.
 *
 * @defgroup ShaderLoader Shader Loader
 * @brief This file defines functions for loading and compiling shader programs.
 */
#pragma once
#include <string>
#include "vector_3.hpp"

class Window;

/**
 * @brief Creates a shader object of the specified type.
 *
 * This function creates a shader object of the specified type (vertex shader or fragment shader).
 *
 * @param type The type of shader to create. 0 to vertex and 1 to fragment shader.
 * @return The ID of the created shader object, or 0 if creation failed.
 */
unsigned int CreateShader(int type);

/**
 * @brief Compiles the shader source code for the specified shader object.
 *
 * This function compiles the provided shader source code for the shader object identified by ID.
 *
 * @param id The ID of the shader object to compile.
 * @param src The shader source code as a string.
 */
void CompileShader(unsigned int id, const char* src);

/**
 * @brief Creates a shader program by linking a vertex shader and a fragment shader.
 *
 * This function links a vertex shader and a fragment shader to create a complete shader program.
 *  The program is stored in a internal list of the specified window.
 *
 * @param w Reference to the Window object.
 * @param vs The vertex shader source code as a C-style string.
 * @param fs The fragment shader source code as a C-style string.
 * @return The ID of the created shader program, or 0 if linking failed.
 */
unsigned int CreateProgram(Window& w, const char* v, const char* f, const char* g = "0");

/**
 * @brief Reads the contents of a file into a string.
 *
 * This function reads the contents of a file specified by path and returns the contents as a string.
 *
 * @param file The path to the file to read.
 * @return The contents of the file as a string, or an empty string if reading failed.
 */
std::string ReadFiles(const std::string& file);

/**
 * @brief Sets a vec3 uniform variable in a shader program.
 *
 * This function sets a uniform variable of type vec3 in a shader program.
 *  The name of the uniform variable and the vector value are provided as arguments.
 *
 * @param program The ID of the shader program to set the uniform in.
 * @param name The name of the uniform variable to set.
 * @param vector The vec3 value to set the uniform variable to.
 */
void SetVector3(unsigned int program, char* name, Vec3 vector);
