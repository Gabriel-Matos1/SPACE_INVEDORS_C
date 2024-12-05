#ifndef JOGADOR_H  // Guarda contra múltiplas inclusões
#define JOGADOR_H

#include "geral.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
typedef struct
{
    float x, y;
    bool active;
    int hits;
    int altura;
    int largura;
    ALLEGRO_BITMAP *bitmap;
} jogador;


void atira(int sprite_width, int sprite_height, Bullet bullets[], int x, int y);

#endif  
