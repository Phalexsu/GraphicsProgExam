#ifndef __GLFWApplication_h
#define __GLFWApplication_h

#include <tclap/CmdLine.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
class GLFWApplication
{
private:


protected:
	GLFWwindow* window;
	int width = 0;
	int height = 0;
public:
	GLFWApplication() = default;
	GLFWApplication(const std::string& name, const std::string& version);
	~GLFWApplication();
	
	//Argument parsing
	virtual unsigned int ParseArguments(int argc, char** argv); // Virtual function with default behavior.
	// Initialization 
	virtual unsigned int Init(); // Virtual function with defaut behavior
	// Run function
	virtual unsigned int Run() const = 0; // Pure virtual function, it must be redefined

  //....
  //....
};
#endif
