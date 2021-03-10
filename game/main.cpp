#include "game.h"
#include <iostream>
#include <fstream>

#ifdef DEBUGCONFIG
#include "test_main.h"
#endif

void Run() {
	Game().Run();
}

int main(int argc, char* argv[]) {
#ifdef DEBUGCONFIG
	return RunTests(argc, argv);
#elif defined _DEBUG
	Run();
#else
	std::ofstream errfile("stderr.log");
	auto cerrbuf = std::cerr.rdbuf(errfile.rdbuf());
	try {
		Run();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "A fatal error has occurred", e.what(), NULL);
	}
	std::cerr.rdbuf(cerrbuf);
#endif
	return 0;
}