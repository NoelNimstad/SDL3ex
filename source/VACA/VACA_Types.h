/**
 * @file VACA_Types.h
 * @date 2024/09/22
 *
 * This file defines the types and type conversion functions used in regards to the VACA SDL wrapper
 */

#ifndef VACA_types_h
#define VACA_types_h

/**
 * @brief Convert an integer 2d vector into a floating point 2d vector
 * @param _v2f Input integer 2d vector
 * 
 * @return Output floating point 2d vector
 */
#define VACA_ConvertVector2_i2f(_v2i)((Vector2_f){ (_v2i).x, (_v2i).y })
/**
 * @brief Convert a floating point 2d vector into an integer 2d vector
 * @param _v2f Input floating point 2d vector
 * 
 * @return Output integer 2d vector
 */
#define VACA_ConvertVector2_f2i(_v2f)((Vector2_i){ (_v2f).x, (_v2f).y })
/**
 * @brief Check if an integer 2d vector is has magnitude 0
 * @param _v2f Input integer 2d vector
 * 
 * @return Output integer boolean 
 */
#define VACA_Vector2_i_isZero(_v2i)(((_v2i).x = 0) && ((_v2i).y = 0))
/**
 * @brief Check if a floating point 2d vector is has magnitude 0
 * @param _v2f Input floating point 2d vector
 * 
 * @return Output integer boolean 
 */
#define VACA_Vector2_f_isZero(_v2f)(((_v2f).x == 0) && ((_v2f).y == 0))
/**
 * @brief An integer 2 dimensional vector 
 */
typedef struct Vector2_i
{
    int           x, y;
} Vector2_i;
/**
 * @brief A floating point 2 dimensional vector 
 */
typedef struct Vector2_f
{
    float         x, y;
} Vector2_f;

/**
 * @brief The VACA struct encapsulates all variables needed for the operation of the VACA SDL wrapper
 */
typedef struct VACA
{
    SDL_Event     event;
    double        deltaTime;

    unsigned char mouseState;
    Vector2_i     mousePosition;

    int           _width;
    int           _height;
    int           _scale;

    float         _frameDelay;
    Uint64        _currentCounter;
    Uint64        _lastCounter;
    double        _performanceFrequency;

    SDL_Window   *_SDL_Window;
    SDL_Renderer *_SDL_Renderer;
} VACA;

/**
 * @brief A single sprite 
 */
typedef struct Sprite
{
    SDL_Rect      rect;
    
    SDL_Texture  *_SDL_Texture;
} Sprite;
/**
 * @brief Multiple sprites
 */
typedef struct Spritesheet
{
    SDL_Rect      rect;

    SDL_Rect      _sourceRect; 
    SDL_Texture  *_SDL_Texture;
} Spritesheet;

#endif // VACA_types_h