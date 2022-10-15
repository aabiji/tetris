#include "src/game.hpp"

int main()
{
    Game g = Game();
    g.init();
    g.gameloop();
    g.exit();
}
