#include <string>
#include "vector_3.hpp"

unsigned int CreateShader(int type);
void CompileShader(unsigned int id, const char* src);
unsigned int CreateProgram(const char* vs, const char* fs);
std::string ReadFiles(const std::string& file);

void SetVector3(unsigned int program, char* name, Vec3 vector);