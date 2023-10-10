#include <string>

unsigned int CreateShader(int type);
void CompileShader(unsigned int id, const char* src);
unsigned int CreateProgram(unsigned int vs, unsigned int fs);
std::string ReadFiles(const std::string& file);