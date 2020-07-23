#include "window.h"
#include <iostream>
#include <fstream>

void Run() {
	Window window;
	window.Load();
	window.Run();
}

int main(int argc, char* args[]) {
#ifdef _DEBUG
	Run();
#else
	std::ofstream errfile("stderr.log");
	auto cerrbuf = std::cerr.rdbuf(errfile.rdbuf());
	try {
		Run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
	}
	std::cerr.rdbuf(cerrbuf);
#endif
	return 0;
}