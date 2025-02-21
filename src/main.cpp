#include <iostream>
#include <SDL.h>
#include "Application.h"

using namespace std;

int main(int argc, char** args) {
    Application app;
    if (!app.init()) return 1;
    app.run();
    return 0;
}