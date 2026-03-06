#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

void main_loop(void) {
    // Frame update will go here
}

int main(void) {
    printf("Time - A Visual Study\n");

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#endif

    return 0;
}
