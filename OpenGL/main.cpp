#include "window.h"
#include <iostream>

int main(int argc, char* args[]) {
	try {
		Window window;
		window.Load();
		window.Run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
    return 0;
}