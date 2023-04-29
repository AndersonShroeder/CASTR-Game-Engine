#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "renderer.hh"
#include "player.hh"

// 2d representation of map
int map[SCREEN_WDITH * SCREEN_HEIGHT] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 1, 0, 0, 1,
		1, 1, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 2, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 3, 1, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int main()
{
	Renderer renderer;

	Player player(-.5, 0, renderer.window, renderer.VAO, renderer.shaderProgram, map);

	while (!glfwWindowShouldClose(renderer.window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 1);

		#ifdef GRID_DEMO
		renderer.drawMap(map);
		renderer.drawGrid();
		#endif
		player.drawPlayer(renderer);
		

		glfwSwapBuffers(renderer.window);

		glfwPollEvents();
	}

	return 0;
}