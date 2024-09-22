#include <stdio.h>

#include "VACA/VACA.h"

#define WIDTH 400
#define HEIGHT 300

enum scenes
{
    TitleScreen
};

struct
{
    VACA *V;
    Sprite *sprites[1];

    int frame;
    int running;

    enum scenes scene;
} Game;

void Initialize()
{
    Game.V = VACA_Initialize("BOMBERMAN", 256, 240, 1, 60);

    Game.sprites[0] = VACA_CreateSprite(Game.V, "assets/title.png", 256, 240, 0, 0);

    Game.frame = 0;
    Game.running = 1;

    Game.scene = TitleScreen;

    VACA_DrawSprite(Game.V, Game.sprites[0]);
    VACA_RenderPresent(Game.V);
}

void GameLoop()
{
    while(VACA_PollEvent(Game.V))
    {
        if(Game.V -> event.type == SDL_QUIT) 
            Game.running = 0;
    }

    // VACA_ClearScreen(Game.V, 0, 0, 0);
    printf("%d\n", Game.frame++);

    switch(Game.scene)
    {
        case TitleScreen:
            VACA_MaintainFrameRate(Game.V);
            break;
    }
}

void CleanUp()
{
    VACA_DestroySprite(Game.sprites[0]);
    VACA_Destroy(Game.V);
}

int main(void)
{
    Initialize();

    while(Game.running)
    {
        GameLoop();
    }

    CleanUp();

    return 0;
}