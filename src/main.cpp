#include <glad.h>
#include <GLFW/glfw3.h>
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

int map2[SCREEN_WDITH * SCREEN_HEIGHT] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 2, 0, 0, 0, 1,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 3, 1, 0, 1,
		1, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

int main()
{
	float stepSizeX = 1.0/5.0;
	float stepSizeY = 1.0/5.0;
	
	CASTRWindow window(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "1");
	CASTRWindow window2(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "2");
	CASTRRayCastMapRenderer renderer(MAP_WIDTH, MAP_HEIGHT, map, stepSizeX, stepSizeY, &window);
	CASTRRayCastMapRenderer renderer2(MAP_WIDTH, MAP_HEIGHT, map2, stepSizeX, stepSizeY, &window2);
	Point p1({0, 0}, {1.0f, 0.0f, 0.0f}, 20);

	while (!glfwWindowShouldClose(renderer.window->window) && !glfwWindowShouldClose(renderer2.window->window))
	{
		renderer.render();
		renderer2.render();

		glfwPollEvents();
	}

	return 0;
}