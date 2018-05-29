#include <stdlib.h>
#include <stdio.h>
#include <emscripten.h>

void mainLoop() {
    static int step = 0;
    if (step > 10) {
        exit(0);
    }
    else {
        printf("Hello %d \n", step);
        step++;
    }
}

int main(int argc, char* argv[]) {
    printf("Initializing!\n");
    emscripten_set_main_loop(mainLoop, 0, 1);
}