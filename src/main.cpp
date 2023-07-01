#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game.hh"

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

// Entities get created and then to be rendered we pass the entities to the appropriate renderer. Every Renderer must implement a virtual function to render entities.

int main()
{
	float stepSizeX = 1.0/5.0;
	float stepSizeY = 1.0/5.0;
	
	CASTRWindow window(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "1");
	CASTRWindow window2(SCREEN_HEIGHT, SCREEN_WDITH, SCREEN_HEIGHT, SCREEN_WDITH, "2");
	CASTRRayCastMapRenderer renderer(&window);
	CASTRRayCastMapRenderer renderer2(&window2);
	renderer.generateMap(10, 10, map, stepSizeX, stepSizeY);
	renderer2.generateMap(10, 10, map2, stepSizeX, stepSizeY);
	Point p1({0, 0}, {1.0f, 0.0f, 0.0f}, 20);
	std::vector<Entity> objs;
	// objs.push_back(Entity{{0.5f, 0.5f}});
	objs.insert(
		objs.end(),
		{Entity{{0.5f, 0.5f}}, Entity{{0.0f, 0.0f}}, Entity{{-0.5f, -0.5f}}, Entity{{-0.5f, 0.5f}}, Entity{{0.5f, -0.5f}}}
	);

	while (!glfwWindowShouldClose(renderer.window->window) && !glfwWindowShouldClose(renderer2.window->window))
	{
		renderer.render();
		// for (Entity& i: objs)
		// {
		// 	float r1 = -0.01 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.01f+0.01f)));
		// 	float r2 = -0.01 + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.01f+0.01f)));
		// 	renderer.renderEntity(i);
		// 	i.updatePos(r1,r2);
		// }
		glfwSwapBuffers(window.window);

		renderer2.render();
		glfwSwapBuffers(window2.window);

		glfwPollEvents();
	}

	return 0;
}