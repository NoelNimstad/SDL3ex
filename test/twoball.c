/*
    TODO:
    - More balls
    - Ball collisions
    - Winning
*/

#define FRICTION 0.2
#define MASS 0.16
#define GRAVITY 9.82

#define SPEED 3

#define TARGET_FPS 120
#define MINIMUM_VELOCITY (FRICTION * MASS * GRAVITY * (500 / TARGET_FPS))

#include "VACA/VACA.h"

typedef struct Ball
{
    Vector2_f     velocity;
    Vector2_f     position;
    unsigned char image;
} Ball;

struct
{
    VACA         *V;
    Sprite       *sprites[4];

    int           frame;
    int           running;

    unsigned char state;
    unsigned char mouse;

    Ball         *balls;

    Vector2_f     yellowBallVelocity;
    Vector2_f     yellowBallPosition;

    Vector2_f     friction;
} Game;

void init()
{
    Game.V = VACA_Initialize("Two Ball", 400, 500, 2, TARGET_FPS);

    Game.sprites[0] = VACA_CreateSprite(Game.V, "assets/twoball/board.png", 400, 500, 0, 0);
    Game.sprites[1] = VACA_CreateSprite(Game.V, "assets/twoball/yellow.png", 15, 15, 0, 0);
    Game.sprites[2] = VACA_CreateSprite(Game.V, "assets/twoball/yellow_hand.png", 18, 18, 0, 0);
    Game.sprites[3] = VACA_CreateSprite(Game.V, "assets/twoball/cursor.png", 15, 15, 0, 0);

    Game.state = 0;
}

int main(int argc, char const *argv[])
{
    init();

    VACA_SetRenderDrawColor(Game.V, 255, 0, 0);
    SDL_ShowCursor(SDL_DISABLE);

    unsigned char running = 1;
    while(running)
    {   
        VACA_StartFrame(Game.V);

        while(VACA_PollEvent(Game.V))
        {
            if(Game.V -> event.type == SDL_QUIT)
                running = 0;

            if(Game.V -> event.type == SDL_MOUSEBUTTONDOWN && Game.V -> event.button.button == SDL_BUTTON_LEFT)
            {
                Game.mouse = 1;
            }

            if(Game.V -> event.type == SDL_MOUSEBUTTONUP && Game.V -> event.button.button == SDL_BUTTON_LEFT)
            {
                Game.mouse = 0;
            }
        }

        VACA_UpdateMouse(Game.V);

        VACA_DrawSprite(Game.V, Game.sprites[0]);
        switch(Game.state)
        {
            case 0:
                Game.V -> mousePosition.x = SDL_clamp(Game.V -> mousePosition.x, 63, 347);
                Game.V -> mousePosition.y = SDL_clamp(Game.V -> mousePosition.y, 63, 447);

                VACA_MoveSprite(Game.sprites[2], Game.V -> mousePosition.x - 12, Game.V -> mousePosition.y - 12);
                VACA_DrawSprite(Game.V, Game.sprites[2]);

                if(Game.mouse == 1)
                {
                    Game.state = 1;
                    VACA_MoveSprite(Game.sprites[1], Game.V -> mousePosition.x - 12, 
                                                     Game.V -> mousePosition.y - 12);
                    Game.mouse = 0;
                }
                break;
            case 1:
                VACA_DrawSprite(Game.V, Game.sprites[1]);

                VACA_DrawLine(Game.V, Game.sprites[1] -> rect.x + 7, 
                                      Game.sprites[1] -> rect.y + 7,
                                      Game.V -> mousePosition.x,
                                      Game.V -> mousePosition.y);

                VACA_MoveSprite(Game.sprites[3], Game.V -> mousePosition.x,
                                                 Game.V -> mousePosition.y);
                VACA_DrawSprite(Game.V, Game.sprites[3]);

                if(Game.mouse == 1)
                {
                    float theta = VACA_AngleBetween(Game.V -> mousePosition.x, 
                                                    Game.V -> mousePosition.y, 
                                                    Game.sprites[1] -> rect.x + 7,
                                                    Game.sprites[1] -> rect.y + 7);

                    Vector2_f direction = { cosf(theta), sinf(theta) };
                    float magnitude = SDL_max(VACA_DistanceBetween(Game.V -> mousePosition.x, 
                                                                   Game.V -> mousePosition.y, 
                                                                   Game.sprites[1] -> rect.x + 7,
                                                                   Game.sprites[1] -> rect.y + 7), 
                                              1.0f);

                    Game.yellowBallVelocity.x = direction.x * magnitude * SPEED;
                    Game.yellowBallVelocity.y = direction.y * magnitude * SPEED;

                    Game.friction.x = -direction.x * FRICTION * GRAVITY * MASS;
                    Game.friction.y = -direction.y * FRICTION * GRAVITY * MASS;

                    Game.yellowBallPosition.x = Game.sprites[1] -> rect.x;
                    Game.yellowBallPosition.y = Game.sprites[1] -> rect.y;

                    printf("%f %f\n", Game.yellowBallVelocity.x, sinf(theta));
                    Game.state = 2;
                    Game.mouse = 0;
                }
                break;
            case 2:
                Game.yellowBallVelocity.x += Game.friction.x * Game.V -> deltaTime * 1000;
                Game.yellowBallVelocity.y += Game.friction.y * Game.V -> deltaTime * 1000;

                if(-MINIMUM_VELOCITY < Game.yellowBallVelocity.x && MINIMUM_VELOCITY > Game.yellowBallVelocity.x
                && -MINIMUM_VELOCITY < Game.yellowBallVelocity.y && MINIMUM_VELOCITY > Game.yellowBallVelocity.y)
                {
                    Game.yellowBallVelocity.x = 0;
                    Game.yellowBallVelocity.y = 0;

                    Game.state = 0;
                    break;
                }

                Game.yellowBallPosition.x += Game.yellowBallVelocity.x * (Game.V -> deltaTime);
                Game.yellowBallPosition.y += Game.yellowBallVelocity.y * (Game.V -> deltaTime);

                if(Game.yellowBallPosition.x < 50 || Game.yellowBallPosition.x > 335)
                {
                    Game.yellowBallVelocity.x *= -1;
                    Game.friction.x *= -1;
                    Game.yellowBallPosition.x += Game.yellowBallVelocity.x * (Game.V -> deltaTime);
                }
                if(Game.yellowBallPosition.y < 50 || Game.yellowBallPosition.y > 435)
                {
                    Game.yellowBallVelocity.y *= -1;
                    Game.friction.y *= -1;
                    Game.yellowBallPosition.y += Game.yellowBallVelocity.y * (Game.V -> deltaTime);
                }

                // printf("dt%f\n", Game.V -> deltaTime);

                VACA_MoveSprite(Game.sprites[1], Game.yellowBallPosition.x, Game.yellowBallPosition.y);
                VACA_DrawSprite(Game.V, Game.sprites[1]);

                VACA_MoveSprite(Game.sprites[3], Game.V -> mousePosition.x,
                                                 Game.V -> mousePosition.y);
                VACA_DrawSprite(Game.V, Game.sprites[3]);
        }

        VACA_RenderPresent(Game.V);
        VACA_EndFrame(Game.V);
    }

    VACA_DestroySprite(Game.sprites[0]);
    VACA_DestroySprite(Game.sprites[1]);
    VACA_DestroySprite(Game.sprites[2]);
    VACA_DestroySprite(Game.sprites[3]);
    VACA_Destroy(Game.V);

    return 0;
}