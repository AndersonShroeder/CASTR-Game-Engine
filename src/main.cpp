#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include "renderer.hh"
#include "player.hh"
#include "newRenderer.hh"

// 2d representation of map
int map[SCREEN_WDITH * SCREEN_HEIGHT] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 1, 0, 1, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 1, 0, 2, 0, 0, 0, 1,
		1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 3, 1, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int main()
{
	// Renderer renderer;
	GLuint num = 0;
	
	CASTRWindow window(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "1");
	CASTRWindow window2(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "2");
	CASTRRayCastMapRenderer renderer(MAP_WIDTH, MAP_HEIGHT, map, &window);
	CASTRRayCastMapRenderer renderer2(MAP_WIDTH, MAP_HEIGHT, map, &window2);
	Point p1({0, 0}, {1.0f, 0.0f, 0.0f}, 20);

	while (!glfwWindowShouldClose(renderer.window->window) && !glfwWindowShouldClose(renderer2.window->window))
	{
		glfwMakeContextCurrent(renderer.window->window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 1);
		renderer.drawGrid();
		glfwSwapBuffers(renderer.window->window);

		glfwMakeContextCurrent(renderer2.window->window);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2, 0.2, 0.2, 1);
		renderer2.drawGrid();
		glfwSwapBuffers(renderer2.window->window);

		glfwPollEvents();
	}

	return 0;
}