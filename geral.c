#include "geral.h"

void inicializar_jogo(void)
{
    if (!al_init())
    {
        printf("Falha ao inicializar o Allegro!\n");
        return;
    }

    if (!al_init_image_addon())
    {
        printf("Falha ao inicializar o addon de imagens!\n");
        return;
    }

    if (!al_init_primitives_addon())
    {
        printf("Falha ao inicializar o addon de primitivas!\n");
        return;
    }

    if (!al_install_keyboard())
    {
        printf("Falha ao instalar o teclado!\n");
        return;
    }

    if (!al_init_font_addon())
    {
        printf("Falha ao inicializar o addon de fontes.\n");
        return;
    }

    if (!al_init_ttf_addon())
    {
        printf("Falha ao inicializar o addon TTF.\n");
        return;
    }
}

