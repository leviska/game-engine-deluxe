#include "window.h"

int main(int argc, char* args[])
{
	Window window;
	if (window.IsGood())
		window.Process();
    return 0;
}