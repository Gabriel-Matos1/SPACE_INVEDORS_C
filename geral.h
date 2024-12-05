#ifndef GERAL_H  // Guarda contra múltiplas inclusões
#define GERAL_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#define X_SCREEN 840
#define Y_SCREEN 680
#define BULLET_SPEED 20
#define AVIAO_VELOCIDADE 4.0
#define BULLET_SPEED_inimigos 10

typedef struct
{
    float x, y;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Coracao;
typedef struct
{
    float x, y;
    int altura;
    int largura;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Bullet;


typedef struct
{
    float x, y;
    int altura;
    int largura;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} item;

void inicializar_jogo(void);
#endif  
