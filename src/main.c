#include "game.h"

int main() {
    game_init();
    while (game_is_running()) {
        game_update();
    }
    game_deinit();

    return 0;
}
