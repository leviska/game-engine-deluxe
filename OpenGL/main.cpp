#include "window.h"
#include <iostream>

void Run() {
	Window window;
	window.Load();
	window.Run();
}

int main(int argc, char* args[]) {
#ifdef _DEBUG
	Run();
#else
	try {
		Run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
#endif
	return 0;
}