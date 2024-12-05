#ifndef VILOES_H  // Guarda contra múltiplas inclusões
#define VILOES_H

#include "geral.h"



typedef struct{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Boss;

typedef struct
{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Bomba;

typedef struct
{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} AviaoInimigoVertical;

typedef struct
{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} AviaoInimigoHorizontal;

typedef struct
{
    float x, y;
    bool active;
} BulletInimigo;


void atiraInimigo(int sprite_width, int sprite_height, Bullet bullets[], int x, int y);
void atiraInimigo_vertical(int sprite_width, int sprite_height, Bullet bullets[], int x, int y);
void atiraInimigo_verticalv2(int sprite_width, int sprite_height, Bullet bullets[], int x, int y);

#endif  
