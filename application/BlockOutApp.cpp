#include "BlockOutApp.h"
#include "GeometricTools.h"
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Shaders.h"
#include "RenderCommands.h"
#include "PerspectiveCamera.h"
#include "TextureManager.h"

//constructor
BlockOutApp::BlockOutApp(const std::string& name, const std::string& version) {
	std::cout << "Program: " << name << "\nVersion: " << version << "\n\n";
}

//destructor
BlockOutApp::~BlockOutApp() {
}

//Argument parsing
unsigned int BlockOutApp::ParseArguments(int argc, char** argv) {
	GLFWApplication::ParseArguments(argc, argv);
	return 0;
}

// Initialization 
unsigned int BlockOutApp::Init() {
	GLFWApplication::Init();
	return 0;
}
/**
* @brief checks if two floats are equal accounting for a 0.0001 difference
*			since floats are not super accurate
*
* @param float a - first float
* @param float b - second float 
* 
* @return true if equal
*/
bool floatEqual(float a, float b) {
	if (a <= b + 0.0001f && a >= b - 0.0001f) {
		return true;
	}
	return false;
}

/**
* @brief checks if there is collision in the y direction
* 
* @param glm::vec3 cubePos - the active cube position
* @param std::vector<glm::vec3> cubeTranslationVectors - vector with all the
			translation vectors for the solid cubes and the starting position
* @param float dir - position offset from the active cube
		(for example -0.2f to check if there is a cube under the active cube)
*
* @see floatEqual(...)
* 
* @return true if there is a collision
*/
bool collisonY(glm::vec3 cubePos, std::vector<glm::vec3> cubeTranslationVectors
				,float dir) {
	
	for (int i = 1; i < cubeTranslationVectors.size(); i++)
	{
		if (floatEqual(cubeTranslationVectors[i].z, cubePos.z) &&
			floatEqual(cubeTranslationVectors[i].y, cubePos.y+dir) &&
			floatEqual(cubeTranslationVectors[i].x, cubePos.x)){
			return true;
		}
	}
	return false;
}

/**
* @brief checks if there is collision in the x direction
*
* @param glm::vec3 cubePos - the active cube position
* @param std::vector<glm::vec3> cubeTranslationVectors - vector with all the
			translation vectors for the solid cubes and the starting position
* @param float dir - position offset from the active cube
(for example -0.2f to check if there is a cube to the right of the active cube)
*
* @see floatEqual(...)
* 
* @return true if there is a collision
*/
bool collisonX(glm::vec3 cubePos, std::vector<glm::vec3> cubeTranslationVectors
				, float dir) {

	for (int i = 1; i < cubeTranslationVectors.size(); i++)
	{
		if (floatEqual(cubeTranslationVectors[i].z, cubePos.z) &&
			floatEqual(cubeTranslationVectors[i].y, cubePos.y) &&
			floatEqual(cubeTranslationVectors[i].x, cubePos.x + dir)) {
			return true;
		}
	}
	return false;
}

/**
* @brief checks if there is a collision in z direction
*
* @param glm::vec3 cubePos - the active cube position
* @param std::vector<glm::vec3> cubeTranslationVectors - vector with all the
			translation vectors for the solid cubes and the starting position
*
* @see floatEqual(...)
* 
* @return true if there is a collision
*/
bool collisionZ(glm::vec3 cubePos, std::vector<glm::vec3> cubeTranslationVectors) {

	for (int i = 0; i < cubeTranslationVectors.size(); i++) {
		if (floatEqual(cubePos.x, cubeTranslationVectors[i].x) &&
			floatEqual(cubePos.y, cubeTranslationVectors[i].y) &&
			floatEqual(cubePos.z - 0.2f, cubeTranslationVectors[i].z))
			return true;
	}

	return false;
}

/**
* @brief checks if all keys that you are not supposed to be able to hold down are released
*
* @param GLFWwindow* window - the window that we are working with
* @param bool &pressed - if a key is pressed right now
*/
void checkReleased(GLFWwindow* window, bool& pressed) {
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_T) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_X) == GLFW_RELEASE && glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE){
		pressed = false;
	}
}

/**
* @brief checks what the bottom is on the spot that the active cube is
*
* @param glm::vec3 cubePos - the active-cube position
* @param std::vector<glm::vec3> cubeTranslationVectors - vector with all the
			translation vectors for the solid cubes and the starting position
* 
* @see floatEqual(...)
* 
* @return returns the z coordinate that is the bottom
*/
float bottom(glm::vec3 cubePos, std::vector<glm::vec3> cubeTranslationVectors) {
	float bot = 0.1f;
	for (int i = 1; i < cubeTranslationVectors.size(); i++)
	{
		if (floatEqual(cubePos.x, cubeTranslationVectors[i].x) &&
			floatEqual(cubePos.y, cubeTranslationVectors[i].y)) {
			bot = cubeTranslationVectors[i].z + 0.2f;
		}
	}
	return bot;
}

/**
* @brief movement keys
*
* @param GLFWwindow* window - the window that we are working with
* @param glm::vec3 cubePos - the active-cube position
* @param bool &pressed - if a key is pressed right now
* @param std::vector<glm::vec3> cubeTranslationVectors - vector with all the
			translation vectors for the solid cubes and the starting position
* @param bool &texture - if texture is supposed to be on or not
* @param bool &lighting - if lighting is supposed to be on or not
*
* @see checkReleased(...)
* @see collisionX(...)
* @see collisionY(...)
* @see collisionZ(...)
* @see bottom(...)
*/
void keyInput(GLFWwindow* window, glm::vec3& cubePos, bool& pressed,
	std::vector<glm::vec3> cubeTranslationVectors,bool& texture, int& lighting) {
	//move the cubeector up
	if ((glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) && !pressed && cubePos.y<0.4f) {

		if(!collisonY(cubePos,cubeTranslationVectors,0.2f))
			cubePos.y += 0.2f;
		pressed = true;
	}

	//move the cubeector down
	if ((glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) && !pressed && cubePos.y > -0.4f) {
		if(!collisonY(cubePos, cubeTranslationVectors, -0.2f))
			cubePos.y -= 0.2f;
		pressed = true;
	}

	//move the cubeector left
	if ((glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) && !pressed && cubePos.x > -0.4f) {
		if (!collisonX(cubePos, cubeTranslationVectors, -0.2f))
			cubePos.x -= 0.2f;
		pressed = true;
	}

	//move the cubeector right
	if ((glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) && !pressed && cubePos.x < 0.4f) {
		if (!collisonX(cubePos, cubeTranslationVectors, 0.2f))
			cubePos.x += 0.2f;
		pressed = true;
	}

	//move inwards
	if ((glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) && !pressed && cubePos.z > 0.0f ) {
		if(!collisionZ(cubePos, cubeTranslationVectors))
			cubePos.z -= 0.2f;
		pressed = true;
	}

	//move to the end
	if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && !pressed && cubePos.z > 0.0f ) {
		if(!collisionZ(cubePos, cubeTranslationVectors))
			cubePos.z = bottom(cubePos, cubeTranslationVectors);
		pressed = true;
	}

	//enbable/disable textures
	if ((glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) && !pressed) {
		texture = !texture;
		pressed = true;
	}

	//enbable/disable lighting
	if ((glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) && !pressed) {
		(lighting == 0) ? lighting = 1 : lighting = 0;
		pressed = true;
	}
	checkReleased(window, pressed);

}
/**
* @brief finds the color for the solid cubes according to the section they are
	a part of
*
* @param glm::vec3 cubeTranslation - translation for the cube calling
*/
glm::vec4 findColor(glm::vec3 cubeTranslation) {
	glm::vec4 color(0.0f, 0.0f, 1.0f,1.0f);
	
	if (cubeTranslation.z < 0.21f) {
		color = glm::vec4(1.0f, 0.5f, 0.5f, 1.0f);
	}
	else if (cubeTranslation.z < 0.41f) {
		color = glm::vec4(0.4f, 0.1f, 0.5f, 1.0f);
	}
	else if (cubeTranslation.z < 0.61f) {
		color = glm::vec4(0.7f, 0.0f, 0.3f, 1.0f);
	}
	else if (cubeTranslation.z < 0.81f) {
		color = glm::vec4(0.5f, 0.3f, 0.5f, 1.0f);
	}
	else if (cubeTranslation.z < 1.01f) {
		color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (cubeTranslation.z < 1.21f) {
		color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (cubeTranslation.z < 1.41f) {
		color = glm::vec4(0.3f, 0.5f, 0.1f, 1.0f);
	}
	else if (cubeTranslation.z < 1.61f) {
		color = glm::vec4(0.6f, 0.2f, 1.0f, 1.0f);
	}

	return color;
}
 //Run function
unsigned int BlockOutApp::Run() const { // Pure virtual function, it must be redefined

	//struct with the matricies except scale because it the same for all of them
	struct grid {	
		glm::mat4 model;
		glm::mat4 translation;
		glm::mat4 rotation;
	};

	// Create buffers and arrays for the grid
	auto gridGeometry = GeometricTools::UnitGridGeometry2D(5, 5);
	auto gridTopology = GeometricTools::unitGridTopology(5, 5);
	auto gridVertexArray = std::make_shared<VertexArray>();
	auto gridIndexBuffer = std::make_shared<IndexBuffer>(gridTopology.data(), gridTopology.size());
	auto gridBufferLayout = BufferLayout({ {ShaderDataType::Float2, "position"} });
	auto gridVertexBuffer = std::make_shared<VertexBuffer>(gridGeometry.data(), gridGeometry.size() * sizeof(gridGeometry[0]));
	gridVertexBuffer->SetLayout(gridBufferLayout);
	gridVertexArray->AddVertexBuffer(gridVertexBuffer);
	gridVertexArray->SetIndexBuffer(gridIndexBuffer);

	// camera
	PerspectiveCamera* cam = new PerspectiveCamera(GLFWApplication::width, GLFWApplication::height);

	//grid-matricies
	std::vector<grid> grids; // vector with all the grids
	grids.resize(9);		
	//the scale is the same for all of them
	auto gridScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
	/*
	adding the values to the matricies
	 the order of the grids are: left back, right back, top back, bottom back
	 left front, right front, top front, bottom front then the backboard
	*/
	for (int i = 0; i <grids.size(); i++)
	{
		float rotx = 0.0f;
		float roty = 0.0f;
		float rotz = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		if (i % 2 == 1) {
			z = 0.5f;
			if (i > 3) {
				if (i < 6)
					x = -1.5f;
				else if (i < 8)
					y = 1.5f;
			}
			else {
				if (i == 1) {
					x = -0.5f;
				}
				else {
					y = 0.5f;
				}
			}
		}
		else {
			z = -0.5f;
			if (i > 3) {
				if (i < 6)
					x = -1.5f;
				else if (i < 8)
					y = 1.5f;
				else {
					z = 0.0f;
				}
			}
			else {
				if (i == 0) {
					x = -0.5f;
				}
				else {
					y = 0.5f;
				}
			}
		}
		
		if (i < 2 ||(i>3&&i<6)) {
			rotx = 0.0f;
			roty = 1.0f;
			rotz = 0.0f;	
		}
		else if (i < 4 ||(i>5&&i<8)) {
			rotx = 1.0f;
			roty = 0.0f;
			rotz = 0.0f;
		}
		else {
			rotx = 0.0f;
			roty = 0.0f;
			rotz = 1.0f;
		}
		grids[i].rotation = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(rotx, roty, rotz));		
		grids[i].translation = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		grids[i].model = gridScale * grids[i].rotation * grids[i].translation;
	}

	auto gridViewProjectionMatrix = cam->GetViewProjectionMatrix();
	
	// shaders for grid
	auto gridShader = std::make_shared<Shader>(Shaders::vertexShader, Shaders::fragmentShader);
	gridShader->Bind();
	glm::vec2 gridPos = { 5,5 };
	gridShader->UploadUniformFloat2("u_divisions", gridPos);
	gridShader->UploadUniformMat4x4("u_viewProjMat", gridViewProjectionMatrix);
	gridShader->UploadUniformFloat("u_diffuseStrength", 0.7f);

	// Create buffers and arrays for cubes
	auto cube = GeometricTools::Cube3DWNormals(5);
	auto cubeTopology = GeometricTools::cubeTopologyWNormals;
	auto cubeVertexArray = std::make_shared<VertexArray>();
	auto cubeIndexBuffer = std::make_shared<IndexBuffer>(cubeTopology.data(), cubeTopology.size());
	auto cubeBufferLayout = BufferLayout({ {ShaderDataType::Float3, "position"},{ShaderDataType::Float3, "normals"} });
	auto cubeVertexBuffer = std::make_shared<VertexBuffer>(cube.data(), cube.size() * sizeof(cube[0]));
	cubeVertexBuffer->SetLayout(cubeBufferLayout);
	cubeVertexArray->AddVertexBuffer(cubeVertexBuffer);
	cubeVertexArray->SetIndexBuffer(cubeIndexBuffer);

	//applying the camera to the cube
	auto cubeRotation = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	std::vector<glm::mat4> cubeTranslations;
	std::vector<glm::vec3> cubeTranslationVectors;
	glm::vec3 startPos(-0.4f, -0.4f, 1.9f);
	auto cubeTranslation = glm::translate(glm::mat4(1.0f), startPos);
	cubeTranslations.push_back(cubeTranslation);
	cubeTranslationVectors.push_back(startPos);
	auto cubeScale = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 3.0f));
	std::vector <glm::mat4> cubeModelMatricies;
	auto cubeModelMatrix = cubeScale * cubeRotation * cubeTranslation;
	cubeModelMatricies.push_back(cubeModelMatrix);
	
	auto cubeViewProjectionMatrix = cam->GetViewProjectionMatrix();

	// Shaders for cube
	auto cubeShader = std::make_shared<Shader>(Shaders::cubeVertexShader, Shaders::cubeFragmentShader);
	cubeShader->Bind();
	cubeShader->UploadUniformMat4x4("u_cubeModMat", cubeModelMatrix);
	cubeShader->UploadUniformMat4x4("u_cubeViewProjMat", cubeViewProjectionMatrix);
	cubeShader->UploadUniformFloat("u_diffuseStrength", 0.7);

	//texture manager
	TextureManager* texMan = TextureManager::GetInstance();
	texMan->LoadTexture2DRGBA("floor", std::string(TEXTURES_DIR) + std::string("floor_texture.png"), 0);
	texMan->LoadCubeMapRGBA("cube", std::string(TEXTURES_DIR) + std::string("cube_texture.png"), 1);

	// Enables
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glPolygonOffset(1, 1);
	glEnable(GL_POLYGON_OFFSET_FILL);

	//adding the normals for the grids to work with lighting
	std::vector <glm::vec3> normals;
	normals.push_back(glm::vec3(0.1f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(0.1f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.1f, 1.0f));
	normals.push_back(glm::vec3(0.0f, 0.1f, -1.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
	normals.push_back(glm::vec3(1.0f, 0.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, 1.0f));
	normals.push_back(glm::vec3(0.0f, -1.0f, -1.0f));
	normals.push_back(glm::vec3(0.0f, 0.0f, -1.0f));

	std::vector <bool> solids;	//keeping track of which cubes are solid
	solids.push_back(false);//first one is not solid
	bool pressed = false;	//is a key pressed
	bool filled = false;		//if a stack is completely filled
	bool texture = false;	//if textures should be active
	int lighting = 0;		//telling the shader if lighting should be active 
	int textureInt = 0;		//telling the shader if textures should be active
	//if the day-night cycle should go brighter or darker
	bool isMorning = false;	
	float ambient = 0.5f;	//the ambient value sent to the shaders
	int backwall = 0;		//if the grid sent to the shader is the backwall 
	glm::vec3 cubePos(-0.4f, -0.4f , 1.9f);	//the active cubes current position
	//the background color around the tube
	glm::vec4 backgroundColor(0.5f, 0.5f, 0.5f, 1.0f);
	//current position of the light
	glm::vec3 lightPos= cam->GetPosition();
	lightPos[2] = cubePos.z * 3; //z-axis follows the cube

	//game-loop
	while (!glfwWindowShouldClose(GLFWApplication::window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
		float timer = glfwGetTime(); //timer
		
		if (!texture) {
			textureInt = 0;
		}
		else {
			textureInt = 1;
		}

		//day night cycle
		if (isMorning) {
			ambient += 0.001f;
			if (ambient > 0.9f) {
				isMorning = false;
			}
		} 
		else{
			ambient -= 0.001f;
			if (ambient < 0.1f) {
				isMorning = true;
			}
		}
		
		//binds the grid VA, upload the grid uniforms and draws them	
		gridVertexArray->Bind();
		gridShader->Bind();
		backwall = 0;
		for (int i = 0; i < grids.size(); i++)
		{		
		//changes the pattern of the grid to stop the same color coming twice
			if(i%8==0|| i % 8 == 1 || i % 8 == 6 || i % 8 == 7) 
				gridShader->UploadUniformInt("u_pattern", 0);
			else
				gridShader->UploadUniformInt("u_pattern", 1);
			if (i == 8) {
				backwall = 1;
			}
			gridShader->UploadUniformMat4x4("u_model", grids[i].model);
			gridShader->UploadUniformInt("u_backWall", backwall);
			gridShader->UploadUniformFloat3("u_normals", normals[i]);
			gridShader->UploadUniformFloat3("u_lightSourcePosition", lightPos);
			gridShader->UploadUniformFloat3("u_cameraPosition", cam->GetPosition());
			gridShader->UploadUniformFloat("u_specularStrenght", 0.7);
			gridShader->UploadUniformInt("u_lighting", lighting);
			gridShader->UploadUniformInt("u_texture", textureInt);
			gridShader->UploadUniformFloat("u_ambientStrength", ambient);
			RenderCommands::DrawIndex(gridVertexArray, GL_TRIANGLES);
		}

		//binds the cube VA, upload the cube uniforms and draws them	
		cubeShader->Bind();
		cubeVertexArray->Bind();
		//process the keyboard input
		keyInput(window, cubePos, pressed, cubeTranslationVectors, texture, lighting);
		for (int i = 0; i < cubeModelMatricies.size(); i++)
		{	
			//checks if a stack is filled
			filled =false;
			for (int j = 1; j < cubeTranslationVectors.size(); j++)
			{
				if (floatEqual(cubePos.y, cubeTranslationVectors[j].y) &&
					floatEqual(cubePos.x, cubeTranslationVectors[j].x) &&
					cubeTranslationVectors[j].z > 1.71f) {
					filled = true;
				}
			}
			float bot = bottom(cubePos, cubeTranslationVectors) + 0.001f;
			//checks if the active cube is at the bottom of the stack and
			// has not reached the top
			if (cubePos.z <= bot && cubePos.z<1.81f) {
				if (!filled) {
					int last = solids.size() - 1;
					solids[last] = true; //sets the active cube to solid
					//saves the active cube translation
					cubeTranslations[last] = glm::translate(glm::mat4(1.0f),
																cubePos);
					//recalculates the modelmatrix
					cubeModelMatricies[last] = cubeScale * cubeRotation 
												* cubeTranslations[last];
					//make a new cube
					solids.push_back(false);
					cubeTranslations.push_back(cubeTranslation);
					cubeModelMatricies.push_back(cubeModelMatrix);
					cubeTranslationVectors.push_back(cubePos);
				}
				//resets the position of the active cube
				cubePos = startPos;			
			}
			//the already made solid cubes
			if (solids[i]) {
				glm::vec4 color = findColor(cubeTranslationVectors[i+1]);
				cubeModelMatricies[i] = cubeScale * cubeRotation
					* cubeTranslations[i];
				cubeShader->UploadUniformFloat4("u_cubeColor", color);
				cubeShader->UploadUniformInt("u_texture", textureInt);
				cubeShader->UploadUniformMat4x4("u_cubeModMat",
												cubeModelMatricies[i]);
				//disable blending with lighting to stop the alpha going wild
				if (lighting == 1)	
					glDisable(GL_BLEND);
				else
					glEnable(GL_BLEND);
				cubeShader->UploadUniformFloat3("u_lightSourcePosition",
													lightPos);
				cubeShader->UploadUniformFloat3("u_cameraPosition",
													cam->GetPosition());
				cubeShader->UploadUniformFloat("u_specularStrenght", 0.5);
				cubeShader->UploadUniformFloat("u_ambientStrength", ambient);
				cubeShader->UploadUniformInt("u_lighting", lighting);
				RenderCommands::DrawIndex(cubeVertexArray, GL_TRIANGLES);
				//draws the border around the cube
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				cubeShader->UploadUniformFloat4("u_cubeColor",
											glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
				RenderCommands::DrawIndex(cubeVertexArray, GL_TRIANGLES);
			}
			else {			
				//always enables blending for the active cube since
				// the lighting does not affect it
				glEnable(GL_BLEND); 
				cubeTranslations[i] = glm::translate(glm::mat4(1.0f), cubePos);
				cubeModelMatricies[i] = cubeScale * cubeRotation
					* cubeTranslations[i];
				cubeShader->UploadUniformMat4x4("u_cubeModMat",
					cubeModelMatricies[i]);
				cubeShader->UploadUniformInt("u_texture", textureInt);
				cubeShader->UploadUniformMat4x4("u_cubeViewProjMat",
					cam->GetViewProjectionMatrix());
				cubeShader->UploadUniformInt("u_lighting", 0);	
				cubeShader->UploadUniformFloat4("u_cubeColor",
					glm::vec4(0.0f, 1.0f, 0.0f, 0.3f));
				RenderCommands::DrawIndex(cubeVertexArray, GL_TRIANGLES);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				cubeShader->UploadUniformFloat4("u_cubeColor",
					glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
				RenderCommands::DrawIndex(cubeVertexArray, GL_TRIANGLES);
				
				//update the light position to follow the cube
				lightPos[2] = cubePos.z * 3;
				//moves the active cube one section in every 2 seconds
				if (timer > 2.0f) {
					cubePos.z -= 0.2f;
					glfwSetTime(0.0f);
				}
			}
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			//day-night cycle for the background
			if (lighting) {
				glClearColor(backgroundColor[0] * ambient, backgroundColor[1] * ambient,
					backgroundColor[2] * ambient, backgroundColor[3] * ambient);
			}
		}
		glfwSwapBuffers(GLFWApplication::window);
		// Exit the loop if escape is pressed
		if (glfwGetKey(GLFWApplication::window, GLFW_KEY_Q) == GLFW_PRESS) break;
	}
	glfwTerminate();
	return 0;
}