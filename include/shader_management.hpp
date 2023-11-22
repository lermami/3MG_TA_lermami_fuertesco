#include <string>

unsigned int CreateShader(int type);
void CompileShader(unsigned int id, const char* src);
unsigned int CreateProgram(const char* vs, const char* fs);
std::string ReadFiles(const std::string& file);