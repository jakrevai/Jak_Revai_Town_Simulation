#include "Jak_Revai_Town_SimulationApp.h"

int main() {
	
	auto app = new Jak_Revai_Town_SimulationApp();
	app->run("Town Simulation - Jak Revai - AIE", 1280, 720, false);
	delete app;

	return 0;
}