#include "Application.h"

int main() {
    Application application;

	while (application.isRunning()) {
		application.update();

		application.render();
	}
    return 0;
}
