#include <functional>
#include <emscripten.h>
//#include <emscripten/html5.h>

#include "graphics.h"
#include <stdio.h>

std::function<void()> loop;
void main_loop() { loop(); }

int main()
{

	EM_ASM(console.log("Start main\n"););

	printf("Create window\n");
	graphics::window window{"canvas"};

	int counter = 0;
	loop = [&] {
		window.draw();
	};

	printf("Start main loop\n");
	emscripten_set_main_loop(main_loop, 0, true);

	return 0;
}
