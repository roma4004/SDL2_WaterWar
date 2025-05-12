#include "Application.h"

#include <iostream>
#include <SDL.h>

using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
    Application app;

    if (!app.Init()) {
        return 1;
    }

    app.GameLoop();

    return 0;
}
