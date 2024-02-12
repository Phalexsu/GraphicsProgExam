#include "BlockOutApp.h"
/**
* You are able to move a cube in a tube that is 5x5x10. the cube can move
* up, down, left and right with the arrow keys and inwards with the X key for
* one step and SPACE to go to the end of the tube. the cube will become solid
* when you hit the backwall or another placed cube. there is lighting and
* textures that you can enable/disable.	the active cube is always transparent.
*
* @file BlockOut.cpp
* @author Philip A. Sundt
	with parts from Rafael Palomar and Yan Chernikov (The Cherno)
*/

int main(int argc, char* argv[])
{
	BlockOutApp application("BlockOutApp", "1.0");

	application.ParseArguments(argc, argv);
	application.Init();

	return application.Run();
}