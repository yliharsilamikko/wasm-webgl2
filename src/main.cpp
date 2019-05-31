#include <functional>
#include <emscripten.h>
#include <emscripten/html5.h>

#include "graphics.h"

//std
#include <stdio.h>

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{

	EM_ASM(console.log("Start main\n"););

	printf("Create window\n");
	auto window = graphics::window("canvas");

	//FILE *file = fopen("tests/hello_world_file.txt", "rb");

	loop = [&] {
		window.draw();
	};

	printf("Start main loop\n");
	emscripten_set_main_loop(main_loop, 0, true);

	return EXIT_SUCCESS;
}
