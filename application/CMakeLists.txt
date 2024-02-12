project(BlockOut)

add_executable(
	BlockOut
	BlockOut.cpp
	BlockOutApp.cpp
)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.png
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.png)

target_link_libraries(BlockOut PRIVATE GLFWApplication GeometricTools Rendering)
target_compile_definitions(${PROJECT_NAME} PRIVATE
  TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)