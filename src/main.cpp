#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "scene.hh"

// 2d representation of map
int map[SCREEN_WDITH*SCREEN_HEIGHT] =
{
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int main()
{
	Scene scene;

	while (!glfwWindowShouldClose(scene.window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0, 1.0, 0, 0);

		glfwSwapBuffers(scene.window);

		glfwPollEvents();
	}
}