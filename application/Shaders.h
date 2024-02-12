#ifndef SHADERS_H_
#define SHADERS_H_

#include "string"

namespace Shaders {

	const std::string vertexShader =
		R"(
		#version 460 core

		layout(location = 0) in vec2 position;
		out vec2 positions;
		out vec4 vs_pos;
		out vec4 vs_normal;

		uniform mat4 u_viewProjMat;
		uniform mat4 u_model;
		uniform vec3 u_normals;
		void main()
		{

			vs_normal = normalize(u_model*vec4(u_normals,1.0));
			vs_pos = u_model*vec4(position,0.0f,1.0);
			positions = position;
			gl_Position = u_viewProjMat*u_model*vec4(position,0.0, 1.0);
		}
		)";



	const std::string fragmentShader =
		R"(
		#version 460 core

		#define M_PI 3.14159265358979323846 

		in vec2 positions;
		in vec4 vs_pos;
		in vec4 vs_normal;
		out vec4 finalColor;
		out vec4 fragPos;
		uniform vec2 u_divisions=vec2(0.0);
		uniform vec3 u_color1=vec3(0.0);
		uniform vec3 u_color2=vec3(1.0);
		uniform int u_pattern=0;
		uniform int u_texture;
		uniform vec4 u_lightColor = vec4(0.1f,1.0f,0.1f,1.0f);
		uniform float u_ambientStrength=1.0;
		uniform vec3 u_lightSourcePosition;
		uniform float u_diffuseStrength = 0.5;
		uniform vec3 u_cameraPosition;
		uniform float u_specularStrenght = 0.5;
		uniform int u_lighting;
		uniform int u_backWall;

		
		layout(binding=0) uniform sampler2D u_floorTextureSampler;

		void main()
		{

			bool back = false;
			if(u_backWall==1){
				back = true;
			}

			if(u_pattern==0){
				if(sin(M_PI*u_divisions.y*(positions.y-0.5))>0)
				{
					if(sin(M_PI*u_divisions.x*(positions.x-0.5))>0)
						finalColor = vec4(u_color1,1.0);
					else
						finalColor = vec4(u_color2,1.0);
				}
				else
				{
					if(sin(M_PI*u_divisions.y*(positions.x-0.5))>0)
						finalColor = vec4(u_color2,1.0);
					else
						finalColor = vec4(u_color1,1.0);
				}
			}
			else{
				if(sin(M_PI*u_divisions.y*(positions.y-0.5))>0)
				{
					if(sin(M_PI*u_divisions.x*(positions.x-0.5))>0)
						finalColor = vec4(u_color2,1.0);
					else
						finalColor = vec4(u_color1,1.0);
				}
				else
				{
					if(sin(M_PI*u_divisions.y*(positions.x-0.5))>0)
						finalColor = vec4(u_color1,1.0);
					else
						finalColor = vec4(u_color2,1.0);
				}
			}

			//diffuse illumination
			vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_pos.xyz));
			if(back){
				lightDirection = -lightDirection;
			}
			float diffuseStrength = max(dot(lightDirection, vs_normal.xyz), 0.0f)*u_diffuseStrength;
			
			//Specualar illumination
			vec3 reflectedLight = normalize(reflect(-lightDirection, vs_normal.xyz));
			vec3 observerDirection = normalize(u_cameraPosition - vs_pos.xyz);
			float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), 15);
			float specular = specFactor * u_specularStrenght;
			
			if(u_texture == 1)
				finalColor = mix(finalColor,texture(u_floorTextureSampler, positions),0.5);

			fragPos = vs_pos;

			if(u_lighting==1){
				finalColor *= u_ambientStrength + diffuseStrength + specular;
			}
		}
		)";
	const std::string cubeVertexShader =
		R"(
		#version 460 core

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec3 normals;
		
		out vec3 vs_texPos;
		out vec4 vs_normal;
		out vec4 vs_pos;

		uniform mat4 u_cubeViewProjMat;
		uniform mat4 u_cubeModMat;

		void main(){
			vs_texPos = position;
			vs_normal = normalize(u_cubeModMat*vec4(normals,1.0));
			vs_pos = u_cubeModMat*vec4(position,1.0);
			gl_Position = u_cubeViewProjMat*u_cubeModMat*vec4(position, 1.0);

		}
		)";

	const std::string cubeFragmentShader =
		R"(
		#version 460 core
		
		in vec4 vs_pos;
		in vec4 vs_normal;
		in vec3 vs_texPos;
		
		out vec4 finalColor;
		out vec4 fragPos;

		uniform vec4 u_cubeColor = vec4(0.0f,0.0f,1.0f,1.0f);
		uniform int u_texture=0;
		uniform vec4 u_lightColor = vec4(0.1f,1.0f,0.1f,1.0f);
		uniform float u_ambientStrength=1.0;
		uniform vec3 u_lightSourcePosition;
		uniform float u_diffuseStrength = 0.5;
		uniform vec3 u_cameraPosition;
		uniform float u_specularStrenght = 0.5;
		uniform int u_lighting;

		layout(binding=1) uniform samplerCube u_cubeTextureSampler;

		void main(){
			finalColor = u_cubeColor;
			if(u_texture==1)
				finalColor = mix(u_cubeColor,texture(u_cubeTextureSampler, vs_texPos),0.5);

			
			//diffuse illumination
			vec3 lightDirection = normalize(vec3(u_lightSourcePosition - vs_pos.xyz));
			float diffuseStrength = max(dot(lightDirection, vs_normal.xyz), 0.0f)*u_diffuseStrength;
			
			//Specualar illumination
			vec3 reflectedLight = normalize(reflect(-lightDirection, vs_normal.xyz));
			vec3 observerDirection = normalize(u_cameraPosition - vs_pos.xyz);
			float specFactor = pow(max(dot(observerDirection, reflectedLight), 0.0), 15);
			float specular = specFactor * u_specularStrenght;

			fragPos = vs_pos;

			if(u_lighting==1)
				finalColor *= u_ambientStrength + diffuseStrength + specular;
		}
		
		)";
}
#endif
