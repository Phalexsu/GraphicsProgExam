#ifndef GEOMETRICTOOLS_H_
#define GEOMETRICTOOLS_H_

#include "array"

namespace GeometricTools {
	constexpr std::array<float, 3 * 2> UnitTriangle2D = 
	{ -0.5f, -0.5f, 0.5f, -0.5f, 0.0f, 0.5f }; // [2,3]

	constexpr std::array<float, 3 * 2 * 2> UnitSquare2D =
	{ -0.5f, -0.5f,
	  -0.5f, 0.5f,
	   0.5f, -0.5f,
	  -0.5f, 0.5f,
	   0.5f, 0.5f,
	   0.5f, -0.5f }; // [2,3]
	
	constexpr std::array<float, 3 * 2 * 2 * 2> UnitCube3D = {
		-0.5f, -0.5f,  0.5f, // Left - Bottom - Front 
		-0.5f,  0.5f,  0.5f, // Left - Top - Front
		 0.5f, -0.5f,  0.5f, // Right - Bottom - Front
		 0.5f,  0.5f,  0.5f, // Right - Top - Front
		 0.5f, -0.5f, -0.5f, // Right - Bottom - Back
		 0.5f,  0.5f, -0.5f, // Right - Top - Back
		-0.5f, -0.5f, -0.5f, // Left - Bottom - Back
		-0.5f,  0.5f, -0.5f // Left - Top - Back
	};
	constexpr std::array<float, 3 * 24 * 2> UnitCube3D24WNormals = {
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,// Left - Bottom - Front
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,// Left - Top - Front
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,// Right - Bottom - Front
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,// Right - Top - Front
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,// Right - Bottom - Back
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,// Right - Top - Back
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,// Left - Bottom - Back
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f,  -1.0f,

	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,// Left - Top - Back
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  -1.0f
	};
	std::vector<GLuint> cubeTopologyWNormals = {
		2,5,8,		5,8,11,		//Front
		6,9,12,		9,12,15,	//Right
		14,17,20,	17,20,23,	//Back
		0,3,18,		3,18,21,	//Left
		4,10,16,	4,16,22,	//Top
		1,7,13,		1,13,19		//Bottom

	};
	template<typename T>
	std::vector<float> Cube3DWNormals(T X) {
		std::vector<float>cube;
		cube.resize(3 * 24 * 2);
		for (int i = 0; i < cube.size(); i++)
		{
			cube[i] = UnitCube3D24WNormals[i] / X;
		}
		return cube;
	};
	template<typename T>
	std::vector<float> Cube3D(T X) {
		std::vector<float>cube;
		cube.resize(3 * 4 * 2);
		for (int i = 0; i < cube.size(); i++)
		{
			cube[i] = UnitCube3D[i] / X;
		}
		return cube;
	};


	std::vector<GLuint> cubeTopology = {
		0,1,2,	//front
		1,2,3,	
		
		2,3,4,	//right
		3,4,5,

		4,5,6,	//back
		5,6,7,

		0,1,6,	//left
		1,6,7,

		1,3,7,	//top
		3,5,7,

		0,2,6,	//bottom
		2,4,6
	};

	// ============================================
	// Unit grid
	// ============================================

	template<typename T, typename U>
	std::vector<float> UnitGridGeometry2D(T X, U Y)
	{
		std::vector<float> vbo;
		vbo.resize((X + 1) * (Y + 1) * 2);
		for (unsigned int j = 0; j < Y+1; ++j)
		{
			float y = j / static_cast<float>(Y) - 0.5f;

			for (unsigned int i = 0; i < X+1; ++i)
			{
				float x = i / static_cast<float>(X) - 0.5f;
				vbo[(j * (X + 1) + i) * 2 + 0] = x;
				vbo[(j * (X + 1) + i) * 2 + 1] = y;
			}
		}
		return vbo;
	}
	
	template<typename T, typename U>
	std::vector<GLuint> unitGridTopology(T X, U Y) {
		std::vector<GLuint> topology;
		topology.resize(6 * X * Y);
		int h = 0;
		GLuint k = 0;
		bool botLeft = true;
		for (int i = 0; i < (2*X*Y); i++)
		{
			if (botLeft) {
				topology[h] = k;
				topology[h + 1] = k + 1;
				topology[h + 2] = k + X + 1;
				k++;
			}
			else {
				topology[h] = k;
				topology[h + 1] = k + X;
				topology[h + 2] = k + X + 1;
				if ((k + 1) % (X+1) == 0) {
					k++;
				}
			}
			h += 3;

			botLeft = !botLeft;
		}
		return topology;
	}

	template<typename T, typename U>
	std::vector<float> UnitGridGeometry2DWTCoords(T X, U Y)
	{
		std::vector<float> vbo;
		vbo.resize((X + 1) * (Y + 1) * (2+2));
		for (unsigned int j = 0; j < Y + 1; ++j)
		{
			float y = j / static_cast<float>(Y) - 0.5f;

			for (unsigned int i = 0; i < X + 1; ++i)
			{
				float x = i / static_cast<float>(X) - 0.5f;
				vbo[(j * (X + 1) + i) * 4 + 0] = x;
				vbo[(j * (X + 1) + i) * 4 + 1] = y;
				vbo[(j * (X + 1) + i) * 4 + 2] = x + 0.5;
				vbo[(j * (X + 1) + i) * 4 + 3] = y + 0.5;
			}
		}
		return vbo;
	}

}


#endif

