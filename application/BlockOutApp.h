#ifndef __BlockOutApp_h
#define __BlockOutApp_h

#include "GLFWApplication.h"
#include "iostream"
#include "string"

class BlockOutApp : public GLFWApplication
{
public:
	BlockOutApp(const std::string& name, const std::string& version);
	~BlockOutApp();

	//Argument parsing
	virtual unsigned int ParseArguments(int argc, char** argv); // Virtual function with default behavior.
	// Initialization 
	virtual unsigned int Init(); // Virtual function with defaut behavior
	// Run function
	virtual unsigned int Run() const override; // Pure virtual function, it must be redefined
};
#endif
