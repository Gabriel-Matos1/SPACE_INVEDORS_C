//!   gcc -o game basico.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lpthread -ldl -lstdc++
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#define X_SCREEN 840
#define Y_SCREEN 680
#define BULLET_SPEED 20
#define AVIAO_VELOCIDADE 4.0
#define BULLET_SPEED_inimigos 10

typedef struct
{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Boss1;

typedef struct
{
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Boss2;

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

typedef struct
{
    float x, y;
    bool active;
    int hits;
    int altura;
    int largura;
    ALLEGRO_BITMAP *bitmap;
} jogador;

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
} item;

typedef struct
{
    float x, y;
    int altura;
    int largura;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Bullet;

void atira(int sprite_width, int sprite_height, Bullet bullets[], int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        if (!bullets[i].active)
        {
            bullets[i].x = x + sprite_width;
            bullets[i].y = y + sprite_height / 2 - 2;
            bullets[i].active = true;
            break;
        }
    }
}

void atiraInimigo(int sprite_width, int sprite_height, Bullet bullets[], int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        bullets[i].x = x + sprite_width;
        bullets[i].y = y + sprite_height / 2 - 2;
        bullets[i].active = true;
        break;
    }
}


void atiraInimigo_vertical(int sprite_width, int sprite_height, Bullet bullets[], int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        if (!bullets[i].active) {  // Encontra um projétil inativo
            bullets[i].x = x + sprite_width / 2 - 2; // Centraliza o tiro no meio do sprite
            bullets[i].y = y -300;        // Começa logo abaixo do inimigo
            bullets[i].active = true;               // Ativa o projétil
            break;                                  // Dispara apenas um por vez
        }
    }
}
void atiraInimigo_verticalv2(int sprite_width, int sprite_height, Bullet bullets[], int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        if (!bullets[i].active) {  // Encontra um projétil inativo
            bullets[i].x = x + sprite_width / 2 - 2; // Centraliza o tiro no meio do sprite
            bullets[i].y = y ;        // Começa logo abaixo do inimigo
            bullets[i].active = true;               // Ativa o projétil
            break;                                  // Dispara apenas um por vez
        }
    }
}


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

void reset_game(jogador *sprite, Bomba *bomba, AviaoInimigoVertical *Aviao1)
{
    sprite->x = 10;
    sprite->y = (Y_SCREEN - al_get_bitmap_height(sprite->bitmap)) / 2;
    sprite->hits = 0;
    bomba->x = X_SCREEN - 100;
    bomba->y = (Y_SCREEN - al_get_bitmap_height(bomba->bitmap)) / 2;
    bomba->hits = 0;
    bomba->active = true;
    Aviao1->x = X_SCREEN - 100;
    Aviao1->y = (Y_SCREEN - al_get_bitmap_height(Aviao1->bitmap)) / 1.5;
    Aviao1->hits = 0;
    Aviao1->active = true;
}


bool verificarColisao(int x1, int y1, int largura1, int altura1, int x2, int y2, int largura2, int altura2) {
    return (x1 < x2 + largura2 && x1 + largura1 > x2 &&
            y1 < y2 + altura2 && y1 + altura1 > y2);
}




void atira_transv(int sprite_width, int sprite_height, Bullet bullets[], int x, int y, ALLEGRO_FONT *font)
{
    al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "Atira Longo");
}


//!  --------------------------------------------------------------  COMEÇO DO JOGO ----------------------------------------------------------------

int main()
{
    inicializar_jogo();

    ALLEGRO_DISPLAY *display = al_create_display(X_SCREEN, Y_SCREEN);
    if (!display)
    {
        printf("Falha ao criar a janela!\n");
        return -1;
    }
    ALLEGRO_FONT *font = al_load_font("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/DM_Mono,Edu_AU_VIC_WA_NT_Pre/DM_Mono/DMMono-Light.ttf", 36, 0);
    if (!font)
    {
        printf("Falha ao carregar a fonte.\n");
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue)
    {
        printf("Falha ao criar a fila de eventos!\n");
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer)
    {
        printf("Falha ao criar o timer!\n");
        al_destroy_event_queue(queue);
        al_destroy_display(display);
        return -1;
    }

    //! -------------------------------------------------------------JOGADOR --------------------------------------------------
    jogador sprite;
    sprite.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Cruiser2_42x61.png");

    int sprite_width = al_get_bitmap_width(sprite.bitmap);
    int sprite_height = al_get_bitmap_height(sprite.bitmap);

    float x = 10;
    float y = (Y_SCREEN - sprite_height) / 2;
    sprite.altura = sprite_height;
    sprite.largura = sprite_width;

    float speed = 8.0;

    Bullet bullets[5] = {{0}};

    
    Bullet bullets_inimigos[5] = {{0}};

    Bullet bullets_inimigos2[5] = {{0}};
    for(int j =0; j<5; j++){

        bullets[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");
        bullets_inimigos[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");
        bullets_inimigos2[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");
        bullets_inimigos[j].altura = 15;
        bullets_inimigos[j].largura = 30;
        bullets_inimigos2[j].largura = 30;
        bullets_inimigos2[j].altura = 15;
        bullets[j].largura = 30;
        bullets[j].altura = 15;
    
    }

    //! ----------------------------------------------INIMIGOS --------------------------------------------------
    Boss1 vilao1;
    vilao1.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Flash_10_80x110.png");
    if (!vilao1.bitmap)
    {
        printf("Falha ao carregar a sprite da bomba!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    vilao1.x = X_SCREEN - 100;
    vilao1.y = (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap)) / 2;
    vilao1.hits = 0;
    vilao1.active = true;

    Bullet tiros_v1[5];
    Bullet tiros_v1_2[5];
    Bullet tiros_v1_vertical[5];
    for(int j =0; j<5; j++){

        tiros_v1[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");
        tiros_v1_2[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");
        tiros_v1_vertical[j].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/salvo/git/SPACE_INVEDORS_C/SpaceShipSamples/cor.png");

    
    }

    Boss2 vilao2;
    vilao2.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Magicfly_47x65.png");
    if (!vilao2.bitmap)
    {
        printf("Falha ao carregar a sprite da bomba!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    vilao2.x = X_SCREEN - 100;
    vilao2.y = (Y_SCREEN - al_get_bitmap_height(vilao2.bitmap)) / 2;
    vilao2.hits = 0;
    vilao2.active = true;

    Bomba bomba;
    bomba.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
    if (!bomba.bitmap)
    {
        printf("Falha ao carregar a sprite da bomba!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    bomba.x = X_SCREEN - 100;
    bomba.y = (Y_SCREEN - al_get_bitmap_height(bomba.bitmap)) / 2;
    bomba.hits = 0;
    bomba.active = true;


    Bomba bomba2;
    bomba2.bitmap = al_load_bitmap("SpaceShipSamples/Buggy_Black_63x69.png");
    if (!bomba2.bitmap)
    {
        printf("Falha ao carregar a sprite da bomba2!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    bomba2.x = X_SCREEN - 50;
    bomba2.y = 50 + ((Y_SCREEN - al_get_bitmap_height(bomba2.bitmap)) / 2);
    bomba2.hits = 0;
    bomba2.active = true;

    AviaoInimigoVertical Aviao1;
    Aviao1.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Scorpion_Evo_112x102.png");
    if (!Aviao1.bitmap)
    {
        printf("Falha ao carregar a sprite do avião inimigo!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_bitmap(bomba.bitmap);
        al_destroy_display(display);
        return -1;
    }
    Aviao1.x = X_SCREEN - 100;
    Aviao1.y = (Y_SCREEN - al_get_bitmap_height(Aviao1.bitmap)) / 1.5;
    Aviao1.hits = 0;
    Aviao1.active = true;


    AviaoInimigoVertical Aviao2;
    Aviao2.bitmap = al_load_bitmap("SpaceShipSamples/Fear_A_Shadow_100x100.png");

    Aviao2.x = X_SCREEN - 100;
    Aviao2.y = (Y_SCREEN/2)-100;
    Aviao2.hits = 0;
    Aviao2.active = true;



    AviaoInimigoVertical Aviao3;
    Aviao3.bitmap = al_load_bitmap("SpaceShipSamples/Flash_A_71x75.png");

    Aviao3.x = X_SCREEN + 100;
    Aviao3.y = (Y_SCREEN/2)-100;
    Aviao3.hits = 0;
    Aviao3.active = true;

    AviaoInimigoVertical Aviao4;
    Aviao4.bitmap = al_load_bitmap("SpaceShipSamples/Flash_A_71x75.png");

    Aviao4.x = X_SCREEN + 100;
    Aviao4.y = (Y_SCREEN/2)+100;
    Aviao4.hits = 0;
    Aviao4.active = true;

   //! _________________________________________________________________ ITEMS _______________________________________________
    item tiro_longo_map;
    tiro_longo_map.bitmap = al_load_bitmap("SpaceShipSamples/busssLight.png");
    
    tiro_longo_map.y = Y_SCREEN -300;
    tiro_longo_map.x = X_SCREEN +100;
    tiro_longo_map.active = true;

    Coracao cor1[3];
    cor1[0].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[1].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[2].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[0].x = X_SCREEN-100;
    cor1[0].y = 20;
    cor1[0].active = true;
    cor1[1].x = X_SCREEN-150;
    cor1[1].y = 20;
    cor1[1].active = true;
    cor1[2].x = X_SCREEN-200;
    cor1[2].y = 20;
    cor1[2].active = true;

    //! ----------------------------------------------CONFIGURAÇÕES DOS ELEMENTOS GERAL --------------------------------------------------

    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    ALLEGRO_BITMAP *fundo2 = al_load_bitmap("SpaceShipSamples/f.png");

    ALLEGRO_BITMAP *carrossel1[3];
    carrossel1[0] = al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[1] = al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[2] = al_load_bitmap("SpaceShipSamples/f.png");

    ALLEGRO_BITMAP *carrossel[3];
    carrossel[0] = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    carrossel[1] = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    carrossel[2] = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");

    float pos_x[3] = {0, X_SCREEN, X_SCREEN * 2};
    float velocidade = 2.0f;

    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    bool running = true;
    bool key_left = false, key_right = false, key_up = false, key_down = false;
    bool key_space = false;

    int milisegundos = 0, segundos = 0, minutos = 0;
    char tempo[100];
    char pont[100];

    bool inicio = false;
    int confirmacao = 0;
    int ps = 2;
    int mov = 1;
    int fase = 1;
    int cont_tiros = 0;
    int pontuacao = 0;
    float coracao_altura = 30.0;
    float coracao_largura = 30.0;
    bool pode_atirar[4];
    int ppower=0;
    int mov_v1 = 1;
    int mov_v2 = 1;
    int bomba_mov =0;
    bool tiro_transv = false;
    int permissao_transv =0;
    int controler_tiros_especiais =0;
    int permissao_longo =0;
    int tiro_longo = false;
    int final =0;
    int ctnl_vilao2 =0;
    int ctnl_tiros_vertical_v2=false;
    ALLEGRO_EVENT event;
    bool morreu = false;
    int p=0;



int primeira_fase_comeco = 15; // A fase começa no segundo 1
int final_horizontal = primeira_fase_comeco + 9; // 1-10: Vilão atira horizontalmente
int final_branco = final_horizontal + 1;        // 11: Tela branca
int final_com_vertical = final_branco + 9;      // 12-20: Vilão atira verticalmente
int final_branco2 = final_com_vertical + 1;     // 21: Tela branca
int final_horizontal2 = final_branco2 + 9;      // 22-30: Vilão atira horizontalmente
int final_branco3 = final_horizontal2 + 1;      // 31: Tela branca
int final_vertical2 = final_branco3 + 9;        // 32-40: Vilão atira verticalmente

    while (running)
    {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {
            if (!inicio && morreu == false && fase== 1)
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "Pressione Space para começar!");
                al_flip_display();
            }
                if (morreu == true && fase == 0)
                {
                    inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Morreu");

                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
                reset_game(&sprite, &bomba, &Aviao1);

                if (key_space)
                {
                    inicio = true;
                    morreu = false;
                    pontuacao = 0;
                    milisegundos = 0;
                    segundos = 0;
                    minutos = 0;
                    x = 10;
                    y = (Y_SCREEN - sprite_height) / 2;
                    fase = 1;
                }

                al_flip_display();
            }

            if (fase == 4)
            {
                inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "1 fase completada!");

                sprintf(pont, "Pontuação atual: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTRE, "para ir para a fase 2!");
                reset_game(&sprite, &bomba, &Aviao1);
                milisegundos = 0;
                segundos = 0;
                minutos = 0;
                if (key_space)
                {
                    inicio = true;
                    fase = 2;
                    morreu = false;
                }

                al_flip_display();
            }

            if (fase == 3)
            {
                inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Você venceu!");

                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
                reset_game(&sprite, &bomba, &Aviao1);
                milisegundos = 0;
                segundos = 0;
                minutos = 0;
                if (key_space)
                {
                    inicio = true;
                    fase = 1;
                    morreu = false;
                    pontuacao = 0;
                }

                al_flip_display();
            }

            if (key_space && !inicio)
            {
                inicio = true;
                key_space = false;
            }
                //!  ----------------------- inicio do ambiente do jogo em si ---------------------------------------------------

            if (inicio)
            {

                milisegundos++;
                if (milisegundos == 60)
                {
                    segundos++;
                    milisegundos = 0;
                }
                if (segundos == 60)
                {
                    minutos++;
                    segundos = 0;
                }

                //!  ----------------------- configurações do plano de fundo ---------------------------------------------------

                al_clear_to_color(al_map_rgb(0, 0, 0));
                if (fase == 1)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        al_draw_bitmap(carrossel1[i], pos_x[i], 0, 0);
                    }

                    for (int i = 0; i < 3; i++)
                    {
                        pos_x[i] -= velocidade;

                        if (pos_x[i] <= -al_get_bitmap_width(carrossel1[i]))
                        {
                            pos_x[i] = pos_x[(i + 1) % 3] + al_get_bitmap_width(carrossel1[i]);
                        }
                    }
                }
                if (fase == 2)
                {
                    for (int i = 0; i < 3; i++)
                    {
                        float escala_largura = (float)Y_SCREEN / al_get_bitmap_height(carrossel[i]) * al_get_bitmap_width(carrossel[i]);

                        // Desenhar a imagem escalada
                        al_draw_scaled_bitmap(
                            carrossel[i],
                            0, 0, // Origem no bitmap
                            al_get_bitmap_width(carrossel[i]),
                            al_get_bitmap_height(carrossel[i]),
                            pos_x[i], 0,              // Posição na tela
                            escala_largura, Y_SCREEN, // Escala para a altura da tela
                            0                         // Sem inversão
                        );
                    }

                    for (int i = 0; i < 3; i++)
                    {
                        pos_x[i] -= velocidade;

                        if (pos_x[i] <= -al_get_bitmap_width(carrossel[i]))
                        {
                            pos_x[i] = pos_x[(i + 1) % 3] + al_get_bitmap_width(carrossel[i]);
                        }
                    }
                }

                //! Atualiza a posição da nave do jogador
                if (key_left){
                    x -= speed;
                    al_draw_rectangle(x+5+sprite_width, y+10, x-10 +sprite_width, y+10,al_map_rgb(0, 0, 220), 2.0);
                    al_draw_rectangle(x+5+sprite_width, y+30, x-10+sprite_width, y+30,al_map_rgb(0, 0, 220), 2.0);

                    }
                if (key_right){
                    x += speed;
                    al_draw_rectangle(x+5, y+10, x-10, y+10,al_map_rgb(0, 0, 220), 2.0);
                    al_draw_rectangle(x+5, y+30, x-10, y+30,al_map_rgb(0, 0, 220), 2.0);
            }
                if (key_up){
                    y -= speed;
                    al_draw_rectangle(x+10, y+sprite_height, x+10, y+sprite_height+30,al_map_rgb(0, 0, 220), 2.0);
                    al_draw_rectangle(x+30, y+sprite_height, x+30, y+sprite_height+30,al_map_rgb(0, 0, 220), 2.0);

                    }
                if (key_down){
                    y += speed;

                    al_draw_rectangle(x+10, y-30, x+10, y,al_map_rgb(0, 0, 220), 2.0);
                    al_draw_rectangle(x+30, y-30, x+30, y,al_map_rgb(0, 0, 220), 2.0);

                }

                //! Impede a nave de sair da tela
                if (x < 0)
                    x = 0;
                if (x + sprite_width > X_SCREEN)
                    x = X_SCREEN - sprite_width;
                if (y < 60)
                    y = 60;
                if (y + sprite_height > Y_SCREEN - 60)
                    y = Y_SCREEN - sprite_height - 60;

                //! Tiros do jogador contra inimigos

                for (int i = 0; i < 5; i++)
                {
                    if (bullets[i].active)
                    {
                        bullets[i].x += BULLET_SPEED;
                        if (bullets[i].x > X_SCREEN)
                            bullets[i].active = false; // Desativa o tiro após sair da tela
                    }
                    //!inimigos
                    if (bullets_inimigos[i].active && ctnl_tiros_vertical_v2 ==false)
                    {
                        bullets_inimigos[i].x -= BULLET_SPEED ;
                        if (bullets_inimigos[i].x < 10)
                            bullets_inimigos[i].active = false; // Desativa o tiro após sair da tela
                    }
                    if (bullets_inimigos2[i].active && ctnl_tiros_vertical_v2 ==false)
                    {
                        bullets_inimigos2[i].x -= BULLET_SPEED;
                        if (bullets_inimigos2[i].x < 10)
                            bullets_inimigos2[i].active = false; // Desativa o tiro após sair da tela
                    }
                    if (tiros_v1[i].active)
                    {
                        tiros_v1[i].x -= BULLET_SPEED;
                        if (tiros_v1[i].x < 0)
                            tiros_v1[i].active = false; // Desativa o tiro após sair da tela
                    }
                    
                    if (tiros_v1_2[i].active)
                    {
                        tiros_v1_2[i].x -= BULLET_SPEED;
                        if (tiros_v1_2[i].x < 10)
                            tiros_v1_2[i].active = false; // Desativa o tiro após sair da tela
                    }

                }

              //! Tiros do inimigo contra o jogador


                if (sprite.hits >= 3)
                {
                    morreu = true;
                    fase = 0;   
                }

                al_draw_bitmap(sprite.bitmap, x, y, 0);

                
                //! ------------------------------------------------------------ FASE 1 ------------------------------------------------------------

                if (fase == 1)
                {
                    al_draw_text(font, al_map_rgb(255, 255, 255), 300, 10, 0, "FASE 1");
                    if (segundos  == 0){ 
                        al_clear_to_color(al_map_rgb(255, 255, 255));
                        final =0;
                    }
 
                    if( segundos > 0 && minutos ==0 &&  final == 0 && segundos <= primeira_fase_comeco){
                                        //! Atualiza a posição do alvo (bomba)

                                        if(bomba_mov < 90 || bomba_mov > 180 ){
                                            bomba.x -= 5;   
                                        }else{
                                            bomba_mov ++;
                                        }
                                        if(bomba.x < 10){
                                            bomba_mov = 0;
                                        }

                                        if (bomba.x < -al_get_bitmap_width(bomba.bitmap))
                                        {
                                            bomba.x = X_SCREEN - 100;
                                            bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                                        }

                                        //! Atualiza a posição do alvo (bomba)
                                        bomba2.x -= 5;
                                        if (bomba2.x < -al_get_bitmap_width(bomba2.bitmap))
                                        {
                                            bomba2.x = X_SCREEN - 100;
                                            bomba2.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2.bitmap));
                                        }
                                        //! Atualiza a posição do avião inimigo vertical
                                        if (mov == 1)
                                        {
                                            Aviao1.y -= ps;
                                            if (Aviao1.y <= 60)
                                            {
                                                mov = 2;
                                            }
                                        }
                                        else
                                        {
                                            Aviao1.y += ps;
                                            if (Aviao1.y + al_get_bitmap_height(Aviao1.bitmap) >= Y_SCREEN - 60)
                                            {
                                                mov = 1;
                                            }
                                        }
                                        if (bomba.active)
                                        {
                                            if (bomba.x + al_get_bitmap_width(bomba.bitmap) > x &&
                                                bomba.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                bomba.y + al_get_bitmap_height(bomba.bitmap) > y &&
                                                bomba.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite.hits++;
                                                printf("Bomba atingiu - linha 742");

                                                bomba.x = X_SCREEN + 100;
                                                bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                                                bomba_mov =0;
                                            }
                                        }

                                        if (bomba2.active)
                                        {
                                            if (bomba2.x + al_get_bitmap_width(bomba2.bitmap) > x &&
                                                bomba2.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                bomba2.y + al_get_bitmap_height(bomba2.bitmap) > y &&
                                                bomba2.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite.hits++;

                                                bomba2.x = X_SCREEN + 100;
                                                bomba2.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2.bitmap));
                                            }
                                        }

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets[i].active)
                                            {
                                                // Colisão com a bomba
                                                if (bullets[i].x >= bomba.x && bullets[i].x <= bomba.x + al_get_bitmap_width(bomba.bitmap) &&
                                                    bullets[i].y >= bomba.y && bullets[i].y <= bomba.y + al_get_bitmap_height(bomba.bitmap))
                                                {
                                                    bomba.hits++;
                                                    pontuacao += 3;
                                                    bullets[i].active = false;
                                                }

                                                if (bullets[i].x >= Aviao1.x && bullets[i].x <= Aviao1.x + al_get_bitmap_width(Aviao1.bitmap) &&
                                                    bullets[i].y >= Aviao1.y && bullets[i].y <= Aviao1.y + al_get_bitmap_height(Aviao1.bitmap))
                                                {
                                                    Aviao1.hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }
                                                if (bullets[i].x >= bomba2.x && bullets[i].x <= bomba2.x + al_get_bitmap_width(bomba2.bitmap) &&
                                                    bullets[i].y >= bomba2.y && bullets[i].y <= bomba2.y + al_get_bitmap_height(bomba2.bitmap))
                                                {
                                                    bomba2.hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }                      
                                        }
                                        }
                                        if (bomba.active)
                                        {
                                            if (bomba.x + al_get_bitmap_width(bomba.bitmap) > x &&
                                                bomba.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                bomba.y + al_get_bitmap_height(bomba.bitmap) > y &&
                                                bomba.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                printf("Bomba atingiu - linha 803");
                                                bomba.x = X_SCREEN + 100;
                                                bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                                                bomba_mov = 0;
                                            }
                                        }
                                        if (bomba2.active)
                                        {
                                            if (bomba2.x + al_get_bitmap_width(bomba2.bitmap) > x &&
                                                bomba2.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                bomba2.y + al_get_bitmap_height(bomba2.bitmap) > y &&
                                                bomba2.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                printf("Bomba2 atingiu - linha 819");
                                                bomba2.x = X_SCREEN + 100;
                                                bomba2.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2.bitmap));
                                            }
                                        }
                                        if (Aviao1.active)
                                        {
                                            if (Aviao1.x + al_get_bitmap_width(Aviao1.bitmap) > x &&
                                                Aviao1.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao1.y + al_get_bitmap_height(Aviao1.bitmap) > y &&
                                                Aviao1.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                printf("Aviao1 atingiu - linha 834");
                                                Aviao1.x = X_SCREEN + 100;
                                                Aviao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao1.bitmap));
                                            }
                                        }
                                        for (int d = 0; d < 5; d++) {
                                        if (bullets_inimigos[d].active) {
                                                float largura_tiro = 40;
                                                float altura_tiro = al_get_bitmap_height(bullets_inimigos[d].bitmap);
                                                float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                                float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                                // Retângulo do tiro
                                                float tiro_x_min = bullets_inimigos[d].x;
                                                float tiro_x_max = bullets_inimigos[d].x + largura_tiro;
                                                float tiro_y_min = bullets_inimigos[d].y;
                                                float tiro_y_max = bullets_inimigos[d].y +40;

                                                // Retângulo do sprite
                                                float sprite_x_min = x;
                                                float sprite_x_max = x + largura_sprite;
                                                float sprite_y_min = y;
                                                float sprite_y_max = y + altura_sprite;

                                                // Desenha o retângulo do tiro (vermelho)
                                                al_draw_rectangle(
                                                    tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,
                                                    al_map_rgb(255, 0, 0), 2.0
                                                );
                                                
                                                if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                                    tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max) {
                                                    
                                                    sprite.hits++;
                                                    pontuacao -= 2;
                                                    bullets_inimigos[d].active = false; // Desativa o tiro
                                                }
                                            }
                                                if (bullets_inimigos2[d].active) {
                                                float largura_tiro = 40;
                                                float altura_tiro = al_get_bitmap_height(bullets_inimigos2[d].bitmap);
                                                float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                                float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                                // Retângulo do tiro
                                                float tiro_x_min = bullets_inimigos2[d].x;
                                                float tiro_x_max = bullets_inimigos2[d].x + largura_tiro;
                                                float tiro_y_min = bullets_inimigos2[d].y;
                                                float tiro_y_max = bullets_inimigos2[d].y +40;

                                                // Retângulo do sprite
                                                float sprite_x_min = x;
                                                float sprite_x_max = x + largura_sprite;
                                                float sprite_y_min = y;
                                                float sprite_y_max = y + altura_sprite;

                                                // Desenha o retângulo do tiro (vermelho)
                                                al_draw_rectangle(
                                                    tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,
                                                    al_map_rgb(255, 0, 0), 2.0
                                                );

                                                // Verifica a colisão
                                                if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                                    tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max) {
                                                    
                                                    sprite.hits++;
                                                    pontuacao -= 2;
                                                    bullets_inimigos2[d].active = false; // Desativa o tiro
                                                }
                                            }
                                        }
                                        if (bomba.hits >= 3)
                                        {
                                            bomba.x = X_SCREEN + 100;
                                            bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                                            bomba.hits = 0;
                                            bomba.active = true;
                                            bomba_mov  =0;
                                        }

                                        if (Aviao1.hits >= 3)
                                        {
                                            Aviao1.active = false;
                                        }


                                        if (bomba2.hits >= 2)
                                        {
                                            bomba2.x = X_SCREEN + 100;
                                            bomba2.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2.bitmap));
                                            bomba2.hits = 0;
                                            bomba2.active = true;
                                        }

                                        //! desenha os inimigos da fase
                                        if (bomba.active)
                                        {
                                            al_draw_bitmap(bomba.bitmap, bomba.x, bomba.y, 0);
                                        }

                                        if (bomba2.active)
                                        {
                                            al_draw_bitmap(bomba2.bitmap, bomba2.x, bomba2.y, 0);
                                        }
                                        if (Aviao1.active)
                                        {
                                            al_draw_bitmap(Aviao1.bitmap, Aviao1.x, Aviao1.y, 0);
                                        }

                                        //! desenha os tiros do inimigo

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets_inimigos[i].active)
                                            {
                                                al_draw_scaled_bitmap(
                                                    bullets_inimigos[i].bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(bullets_inimigos[i].bitmap),
                                                    al_get_bitmap_height(bullets_inimigos[i].bitmap),
                                                    bullets_inimigos[i].x, bullets_inimigos[i].y,
                                                    40, 40, 0);
                                            }
                                            if (bullets_inimigos2[i].active)
                                            {
                                                al_draw_scaled_bitmap(
                                                    bullets_inimigos2[i].bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(bullets_inimigos2[i].bitmap),
                                                    al_get_bitmap_height(bullets_inimigos2[i].bitmap),
                                                    bullets_inimigos2[i].x, bullets_inimigos2[i].y,
                                                    40, 40, 0);
                                        }

                                        if(segundos >= 7){
                                             al_draw_scaled_bitmap(
                                                    tiro_longo_map.bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(tiro_longo_map.bitmap),
                                                    al_get_bitmap_height(tiro_longo_map.bitmap),
                                                    tiro_longo_map.x, tiro_longo_map.y,
                                                    40, 40, 0);
                                                
                                                tiro_longo_map.x +=3;

                                        }

                            }

                        }else{ 
                   //* ativa o vilão ou desativa
                            if (vilao1.hits >= 15){
                                vilao1.active = false;
                                sprite.hits = 0;
                                final = 0;
                                fase = 4;
                            }

                            //! BOSS FINAL DA FASE
                            if(segundos > primeira_fase_comeco ){
                            final = 1;


                            if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)) {
                                    vilao1.x = X_SCREEN -100;
                            
                                al_draw_bitmap(vilao1.bitmap, vilao1.x, vilao1.y, 0);
                            
                                //* desenha os tiros horizontais
                                for (int i = 0; i < 5; i++)
                                {
                                    if (tiros_v1[i].active)
                                    {
                                        al_draw_scaled_bitmap(tiros_v1[i].bitmap, 0, 0, al_get_bitmap_width(tiros_v1[i].bitmap), al_get_bitmap_height(tiros_v1[i].bitmap) + 50, tiros_v1[i].x, tiros_v1[i].y, 40, 40, 0);
                                    }
                                    if (tiros_v1_2[i].active)
                                    {
                                        al_draw_scaled_bitmap(tiros_v1_2[i].bitmap,0, 0,al_get_bitmap_width(tiros_v1_2[i].bitmap), al_get_bitmap_height(tiros_v1_2[i].bitmap),tiros_v1_2[i].x, tiros_v1_2[i].y,40, 40, 0);
                                    }
                                }

                                //* move o vilão verticalmente
                                ps = 5;
                                if (mov_v1 == 1)
                                {
                                    vilao1.y -= ps;
                                    if (vilao1.y <= 60)
                                    {
                                        mov_v1 = 2;
                                    }
                                }
                                else
                                {
                                    vilao1.y += ps;
                                    if (vilao1.y + al_get_bitmap_height(vilao1.bitmap) >= Y_SCREEN)
                                    {
                                        mov_v1 = 1;
                                    }
                                }
                            } //? acaba aqui os primeiros 10 segundos do vilão

                            if (segundos == final_branco ){ 
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                            }

                            if ((segundos > final_branco && segundos <= final_com_vertical)  || (segundos > final_branco3 && segundos <= final_vertical2) ){
                                al_draw_bitmap(vilao1.bitmap, vilao1.x, 20, 0);
                                ps = 3;
                                if (mov_v1 == 1){
                                    vilao1.x -= ps;
                                    if (vilao1.x <= 0)
                                    {
                                        mov_v1 = 2;
                                    }
                                }else{
                                    vilao1.x += ps;
                                    if (vilao1.x + al_get_bitmap_height(vilao1.bitmap) >= X_SCREEN)
                                    {
                                        mov_v1 = 1;
                                    }
                                }
                                
                                for (int i = 0; i < 5; i++)
                                {
                                    if (tiros_v1_vertical[i].active)
                                    {
                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (tiros_v1_vertical[i].active)
                                            {
                                                    al_draw_scaled_bitmap(
                                                        tiros_v1_vertical[i].bitmap,
                                                        0, 0,
                                                        al_get_bitmap_width(tiros_v1_vertical[i].bitmap),
                                                        al_get_bitmap_height(tiros_v1_vertical[i].bitmap),
                                                        tiros_v1_vertical[i].x, tiros_v1_vertical[i].y,
                                                        20, 20, 0);

                                                // Move o projétil para baixo
                                                tiros_v1_vertical[i].y += BULLET_SPEED;

                                                // Verifica se o projétil saiu da tela
                                                if (tiros_v1_vertical[i].y > Y_SCREEN)
                                                {
                                                    tiros_v1_vertical[i].active = false; // Desativa projétil fora da tela
                                                }
                                            }
                                        }
                                    }
                                }

                            }


                            if (segundos == final_branco2 || segundos == final_vertical2){
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                                vilao1.y = Y_SCREEN / 2;
                            }

                            if (milisegundos % 30 == 0){
                                if (vilao1.active){
                                    if (vilao1.x + al_get_bitmap_width(vilao1.bitmap) > x &&
                                        vilao1.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                        vilao1.y + al_get_bitmap_height(vilao1.bitmap) > y &&
                                        vilao1.y < y + al_get_bitmap_height(sprite.bitmap)){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                    }
                                }
                            }

                            for (int i = 0; i < 5; i++){
                                if (bullets[i].active)
                                {
                                    if (bullets[i].x >= vilao1.x && bullets[i].x <= vilao1.x + al_get_bitmap_width(vilao1.bitmap) &&
                                        bullets[i].y >= vilao1.y && bullets[i].y <= vilao1.y + al_get_bitmap_height(vilao1.bitmap))
                                    {
                                        bullets[i].active = false; // Desativa o projétil
                                        vilao1.hits++;             // Incrementa os "hits" no vilão
                                        pontuacao += 3;            // Incrementa a pontuação
                                    }
                                    if (bullets[i].x < 0 || bullets[i].x > X_SCREEN || bullets[i].y < 0 || bullets[i].y > Y_SCREEN)
                                    {
                                        bullets[i].active = false;
                                    }
                                }
                            }


                            for (int d = 0; d < 5; d++){
                                if (tiros_v1[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(tiros_v1[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1[d].x;
                                    float tiro_x_max = tiros_v1[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1[d].y;
                                    float tiro_y_max = tiros_v1[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                            tiros_v1[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (tiros_v1_2[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(tiros_v1_2[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1_2[d].x;
                                    float tiro_x_max = tiros_v1_2[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_2[d].y;
                                    float tiro_y_max = tiros_v1_2[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        sprite.hits++;
                                        pontuacao -= 2;
                                        tiros_v1_2[d].active = false; // Desativa o tiro
                                    }
                                }
                                //! <!-=====================================================               consertar essa parte comentada +++++++++++++++++++++++++++++++++++++++++++- >
                                if (tiros_v1_vertical[d].active){
                                    // Obtém as dimensões do tiro e do sprite
                                    float largura_tiro = 40;
                                    float altura_tiro = -40;
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro (área de detecção vertical)
                                    float tiro_x_min = tiros_v1_vertical[d].x;
                                    float tiro_x_max = tiros_v1_vertical[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_vertical[d].y;
                                    float tiro_y_max = tiros_v1_vertical[d].y + altura_tiro;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho) para visualização
                                    al_draw_rectangle(
                                        tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,
                                        al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão (detecção vertical com o sprite)
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        // Incrementa hits e decrementa pontuação
                                        sprite.hits++;
                                        pontuacao -= 2;
                                        tiros_v1_vertical[d].active = false; // Desativa o tiro após a colisão
                                    }

                                }
                            }
                                    
                        }
                    
                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map.active){    al_draw_scaled_bitmap(
                                tiro_longo_map.bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map.bitmap),
                                al_get_bitmap_height(tiro_longo_map.bitmap),
                                tiro_longo_map.x, tiro_longo_map.y,
                                60, 60, 0);
                            
                            tiro_longo_map.x -=3;
                        }
                    }
                    if (tiro_longo_map.active)
                    {
                        if (tiro_longo_map.x + al_get_bitmap_width(tiro_longo_map.bitmap) > x &&
                            tiro_longo_map.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            tiro_longo_map.y + al_get_bitmap_height(tiro_longo_map.bitmap) > y &&
                            tiro_longo_map.y < y + al_get_bitmap_height(sprite.bitmap))
                        {
                            tiro_longo_map.active=false;
                            pontuacao +=5;
                            tiro_longo = true;
                        }
                    }
                    if(segundos == 10){
                        tiro_longo_map.x = X_SCREEN +80;
                        tiro_longo_map.active = true;

                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map.active){    al_draw_scaled_bitmap(
                                tiro_longo_map.bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map.bitmap),
                                al_get_bitmap_height(tiro_longo_map.bitmap),
                                tiro_longo_map.x, tiro_longo_map.y,
                                60, 60, 0);
                            
                            tiro_longo_map.x -=3;
                        }
                        if(tiro_longo_map.x < 0){
                            tiro_longo_map.active = false;
                        }
                    }

                    if (tiro_longo_map.active)
                    {
                        if (tiro_longo_map.x + al_get_bitmap_width(tiro_longo_map.bitmap) > x &&
                            tiro_longo_map.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            tiro_longo_map.y + al_get_bitmap_height(tiro_longo_map.bitmap) > y &&
                            tiro_longo_map.y < y + al_get_bitmap_height(sprite.bitmap))
                        {
                            tiro_longo_map.active=false;
                            pontuacao +=5;
                            
                        
                            if(segundos < 10){tiro_longo = true;}
                            if(segundos < 10){tiro_transv = true;}
                        }
                    }
                    if(tiro_longo){
                        al_draw_text(font, al_map_rgb(0,0,0), 500, 500, 0, "pegou");       
                    }
                    if(tiro_longo){
                        al_draw_text(font, al_map_rgb(0,0,0), 500, 300, 0, "pegou tmb");       
                    }

                }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                //! ------------------------------------------------------------ FASE 2 ------------------------------------------------------------

                if (fase == 2){

                    al_draw_text(font, al_map_rgb(255, 255, 255), 300, 10, 0, "FASE 2");
                    if (segundos  == 0){ 
                        al_clear_to_color(al_map_rgb(255, 255, 255));
                        final =0;
                    }

                    if( segundos > 0 && minutos ==0 &&  final == 0 && segundos <= primeira_fase_comeco){
                                        //! Atualiza a posição do alvo (bomba)
                                        final =0;


                                        ps = 5;
                                            if (mov_v1 == 1)
                                            {
                                                Aviao3.y -= ps;
                                                Aviao3.x -=ps;
                                                if (Aviao3.y <= 60)
                                                {
                                                    mov_v1 = 2;
                                                }
                                            }
                                            else
                                            {
                                                Aviao3.y += ps;
                                                Aviao3.x -=ps;
                                                if (Aviao3.y + al_get_bitmap_height(Aviao3.bitmap) >= (Y_SCREEN/2))
                                                {
                                                    mov_v1 = 1;
                                                }
                                            }
                                        

                                        if (mov_v2 == 1)
                                        {
                                            Aviao4.y -= ps;
                                                Aviao4.x -=ps;
                                                if (Aviao4.y <= (Y_SCREEN/2))
                                                {
                                                    mov_v2 = 2;
                                                }
                                            }
                                            else
                                            {
                                                Aviao4.y += ps;
                                                Aviao4.x -=ps;
                                                if (Aviao4.y + al_get_bitmap_height(Aviao4.bitmap) >= Y_SCREEN)
                                                {
                                                    mov_v2 = 1;
                                                }
                                        }


                                        //! Atualiza a posição do avião inimigo vertical
                                        if (segundos % 4 == 0) {
                                            Aviao2.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao2.bitmap));
                                        }


                                        if (Aviao3.active)
                                        {
                                            if (Aviao3.x + al_get_bitmap_width(Aviao3.bitmap) > x &&
                                                Aviao3.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao3.y + al_get_bitmap_height(Aviao3.bitmap) > y &&
                                                Aviao3.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite.hits++;
                                                printf("Bomba atingiu - linha 742");

                                                Aviao3.x = X_SCREEN + 100;
                                                Aviao3.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3.bitmap));
                                            }
                                        }


                                        /////////////////////////!
                                        if (Aviao4.active)
                                        {
                                            if (Aviao4.x + al_get_bitmap_width(Aviao4.bitmap) > x &&
                                                Aviao4.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao4.y + al_get_bitmap_height(Aviao4.bitmap) > y &&
                                                Aviao4.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite.hits++;
                                                printf("Bomba atingiu - linha 742");

                                                Aviao4.x = X_SCREEN + 100;
                                                Aviao4.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4.bitmap));
                                            }
                                        }

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets[i].active)
                                            {
                                                // Colisão com a bomba
                                                if (bullets[i].x >= Aviao3.x && bullets[i].x <= Aviao3.x + al_get_bitmap_width(Aviao3.bitmap) &&
                                                    bullets[i].y >= Aviao3.y && bullets[i].y <= Aviao3.y + al_get_bitmap_height(Aviao3.bitmap))
                                                {
                                                    Aviao3.hits++;
                                                    pontuacao += 3;
                                                    bullets[i].active = false;
                                                }

                                                if (bullets[i].x >= Aviao2.x && bullets[i].x <= Aviao2.x + al_get_bitmap_width(Aviao2.bitmap) &&
                                                    bullets[i].y >= Aviao2.y && bullets[i].y <= Aviao2.y + al_get_bitmap_height(Aviao2.bitmap))
                                                {
                                                    Aviao2.hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }
                                        }
                                        }
                                        if(Aviao3.x<0){
                                            Aviao3.x = X_SCREEN + 100;
                                            Aviao3.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3.bitmap));
                                        }
                                        if (Aviao3.active)
                                        {
                                            if (Aviao3.x + al_get_bitmap_width(Aviao3.bitmap) > x &&
                                                Aviao3.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao3.y + al_get_bitmap_height(Aviao3.bitmap) > y &&
                                                Aviao3.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                Aviao3.x = X_SCREEN + 100;
                                                Aviao3.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3.bitmap));
                                            }
                                        }
                                        if(Aviao4.x<0){
                                            Aviao4.x = X_SCREEN + 100;
                                            Aviao4.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4.bitmap));
                                        }
                                        if (Aviao4.active)
                                        {
                                            if (Aviao4.x + al_get_bitmap_width(Aviao4.bitmap) > x &&
                                                Aviao4.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao4.y + al_get_bitmap_height(Aviao4.bitmap) > y &&
                                                Aviao4.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                Aviao4.x = X_SCREEN + 100;
                                                Aviao4.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4.bitmap));
                                            }
                                        }

                                        if (Aviao2.active)
                                        {
                                            if (Aviao2.x + al_get_bitmap_width(Aviao2.bitmap) > x &&
                                                Aviao2.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                                Aviao2.y + al_get_bitmap_height(Aviao2.bitmap) > y &&
                                                Aviao2.y < y + al_get_bitmap_height(sprite.bitmap))
                                            {

                                                sprite.hits++;
                                                pontuacao -= 2;
                                                printf("Aviao2 atingiu - linha 834");
                                                Aviao2.x = X_SCREEN + 100;
                                                Aviao2.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao2.bitmap));
                                            }
                                        }
                                        if (Aviao3.hits >= 4)
                                        {
                                            Aviao3.x = X_SCREEN + 100;
                                            Aviao3.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3.bitmap));
                                            Aviao3.hits = 0;
                                            Aviao3.active = true;
                                        }

                                        if (Aviao4.hits >= 4)
                                        {
                                            Aviao4.x = X_SCREEN + 100;
                                            Aviao4.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4.bitmap));
                                            Aviao4.hits = 0;
                                            Aviao4.active = true;
                                        }

                                        if (Aviao2.hits >= 4)
                                        {
                                            Aviao2.active = false;
                                        }



                                        //! desenha os tiros do inimigo

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets_inimigos2[i].active)
                                            {
                                                al_draw_scaled_bitmap(
                                                    bullets_inimigos2[i].bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(bullets_inimigos2[i].bitmap),
                                                    al_get_bitmap_height(bullets_inimigos2[i].bitmap),
                                                    bullets_inimigos2[i].x, bullets_inimigos2[i].y,
                                                    40, 40, 0);
                                            }
                                            if (tiros_v1[i].active)
                                            {
                                                al_draw_scaled_bitmap(
                                                    tiros_v1[i].bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(tiros_v1[i].bitmap),
                                                    al_get_bitmap_height(tiros_v1[i].bitmap),
                                                    tiros_v1[i].x, tiros_v1[i].y,
                                                    40, 40, 0);
                                            }
                                            if (tiros_v1_2[i].active)
                                            {
                                                al_draw_scaled_bitmap(
                                                    tiros_v1_2[i].bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(tiros_v1_2[i].bitmap),
                                                    al_get_bitmap_height(tiros_v1_2[i].bitmap),
                                                    tiros_v1_2[i].x, tiros_v1_2[i].y,
                                                    40, 40, 0);
                                        }
                                        }
                                        //! desenha os inimigos da fase
                                        if (Aviao3.active)
                                        {
                                            al_draw_bitmap(Aviao3.bitmap, Aviao3.x, Aviao3.y, 0);
                                        }
                                        if (Aviao4.active)
                                        {
                                            al_draw_bitmap(Aviao4.bitmap, Aviao4.x, Aviao4.y, 0);
                                        }
                                        if (Aviao2.active)
                                        {
                                            al_draw_bitmap(Aviao2.bitmap, Aviao2.x, Aviao2.y, 0);
                                        }

                                        if(segundos >= 7){
                                             al_draw_scaled_bitmap(
                                                    tiro_longo_map.bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(tiro_longo_map.bitmap),
                                                    al_get_bitmap_height(tiro_longo_map.bitmap),
                                                    tiro_longo_map.x, tiro_longo_map.y,
                                                    40, 40, 0);
                                                
                                                tiro_longo_map.x +=3;
                                        }

                        }else{ 
                   //* ativa o vilão ou desativa
                            if (vilao2.hits >= 15){
                                vilao2.active = false;
                                sprite.hits = 0;
                                final = 0;
                                fase = 3;
                            }

                            //! BOSS FINAL DA FASE
                            if(segundos > primeira_fase_comeco ){
                            final = 1;

                            //consertar os tiros horizontais
                            if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)) {
                                    vilao2.x = X_SCREEN -100;
                            
                            
                                //* desenha os tiros horizontais
                                for (int i = 0; i < 5; i++){
                                    if(bullets_inimigos2[i].active){
                                        bullets_inimigos2[i].x-=BULLET_SPEED;                                        
                                        al_draw_scaled_bitmap(
                                                bullets_inimigos2->bitmap,
                                                0, 0, 
                                                al_get_bitmap_width(bullets_inimigos2->bitmap),
                                                al_get_bitmap_height(bullets_inimigos2->bitmap),
                                                bullets_inimigos2[i].x, bullets_inimigos2[i].y,              
                                                40,40, 
                                                0      
                                            );       
                                    }
                                    if(tiros_v1_2[i].active){
                                        tiros_v1_2[i].x-=BULLET_SPEED;                                        
                                        al_draw_scaled_bitmap(
                                                tiros_v1_2->bitmap,
                                                0, 0, 
                                                al_get_bitmap_width(tiros_v1_2->bitmap),
                                                al_get_bitmap_height(tiros_v1_2->bitmap),
                                                tiros_v1_2[i].x, tiros_v1_2[i].y,              
                                                40,40, 
                                                0      
                                            );       
                                    }

                                }

                                //* move o vilão verticalmente
                                ps = 5;
                                if (mov_v1 == 1)
                                {
                                    vilao2.y -= ps;
                                    if (vilao2.y <= 60)
                                    {
                                        mov_v1 = 2;
                                    }
                                }
                                else
                                {
                                    vilao2.y += ps;
                                    if (vilao2.y + al_get_bitmap_height(vilao2.bitmap) >= Y_SCREEN)
                                    {
                                        mov_v1 = 1;
                                    }
                                }
                                al_draw_bitmap(vilao2.bitmap, vilao2.x, vilao2.y, 0);

                            } //? acaba aqui os primeiros 10 segundos do vilão

                            if (segundos == final_branco ){ 
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                                vilao2.y = Y_SCREEN / 2;
                                vilao2.x = X_SCREEN / 2;                                
                                ctnl_tiros_vertical_v2 =true;
                            }


                            if(segundos > final_branco3 && segundos <= final_vertical2){

                    if( segundos > 0 && minutos ==0 &&  final == 1 && segundos <= primeira_fase_comeco)
                                al_draw_bitmap(vilao2.bitmap, vilao2.x, vilao2.y, 0);
                                ps = 30;
                                if((vilao2.y <=600 && mov_v1!=2 &&  mov_v1!=3)){
                                    vilao2.x -= ps+5; // Movimento na direção direita
                                    vilao2.y += ps; // Movimento na direção para baixo
                                    if((vilao2.y > 600)||(vilao2.x <= 0)){
                                        mov_v1 =2;
                                        ctnl_vilao2 =1;
                                    }
                                } 
                                if(mov_v1 == 2){
                                    vilao2.y-=ps;
                                    if(vilao2.y <= 50 && ctnl_vilao2 ==1){
                                        mov_v1=3;
                                    }
                                    if(vilao2.y <= 50 && ctnl_vilao2 ==0){
                                        mov_v1=1;
                                    }
                                }
                                if(mov_v1 ==3){
                                    vilao2.x += ps+5; 
                                    vilao2.y += ps; 
                                    if((vilao2.y > 600)||(vilao2.x >= X_SCREEN)){
                                        mov_v1 =2;
                                        ctnl_vilao2 =0;
                                    }
                                    
                                }

                            }


                            if (segundos == final_branco2 || segundos == final_vertical2){
                                al_clear_to_color(al_map_rgb(255, 255, 255));

                                vilao2.y = 50;
                                mov_v1=1;

                            }
                            if(segundos == final_branco3 ){  
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                                vilao2.y = Y_SCREEN / 2;
                                vilao2.x = X_SCREEN / 2;;
                                mov_v1=1;}
                            if(final_com_vertical == segundos){
                                ctnl_tiros_vertical_v2 =false;
                            }

                            if ((segundos > final_branco && segundos <= final_com_vertical)){
                                
                     
                                //! funciona devagar
                                for(int r =0; r<5; r++){
                                    if(bullets_inimigos2[r].active){
                                        bullets_inimigos2[r].x+=BULLET_SPEED;   
                                        al_draw_scaled_bitmap(
                                            bullets_inimigos2->bitmap,
                                            0, 0, // Origem no bitmap
                                            al_get_bitmap_width(bullets_inimigos2->bitmap),
                                            al_get_bitmap_height(bullets_inimigos2->bitmap),
                                            bullets_inimigos2[r].x, bullets_inimigos2[r].y,              // Posição na tela
                                            40,40, // Escala para a altura da tela
                                            0                         // Sem inversão
                                        );                                     
                                    }
                                    //!funciona uma vez?
                                    if(bullets_inimigos[r].active){
                                        bullets_inimigos[r];     
                                        bullets_inimigos[r].y-=BULLET_SPEED;                                   
                                        al_draw_scaled_bitmap(
                                            bullets_inimigos->bitmap,
                                            0, 0, // Origem no bitmap
                                            al_get_bitmap_width(bullets_inimigos->bitmap),
                                            al_get_bitmap_height(bullets_inimigos->bitmap),
                                            bullets_inimigos[r].x, bullets_inimigos[r].y,              
                                            40,40, 
                                            0      
                                        );       

                                        if (tiros_v1_vertical[r].y > X_SCREEN) {
                                            tiros_v1_vertical[r].active = false; // Desativa o projétil
                                        }
                                    }
                                    //! funciona ok
                                if (tiros_v1_vertical[r].active) {
                                            tiros_v1_vertical[r].y += BULLET_SPEED;
                                            al_draw_scaled_bitmap(
                                                tiros_v1_vertical[r].bitmap,
                                                0, 0,
                                                al_get_bitmap_width(tiros_v1_vertical[r].bitmap),
                                                al_get_bitmap_height(tiros_v1_vertical[r].bitmap),
                                                tiros_v1_vertical[r].x, tiros_v1_vertical[r].y,
                                                20, 20, 0
                                            );

                                            if (tiros_v1_vertical[r].y > X_SCREEN) { 
                                                tiros_v1_vertical[r].active = false; 
                                            }
                                        }
                                        //! esse tá ok
                            if(tiros_v1[r].active){
                                        tiros_v1[r].x-=BULLET_SPEED;                                        
                                        al_draw_scaled_bitmap(
                                                tiros_v1->bitmap,
                                                0, 0, 
                                                al_get_bitmap_width(tiros_v1->bitmap),
                                                al_get_bitmap_height(tiros_v1->bitmap),
                                                tiros_v1[r].x, tiros_v1[r].y,              
                                                40,40, 
                                                0      
                                            );       
                                    }

                                }                            
                                al_draw_bitmap(vilao2.bitmap, vilao2.x, vilao2.y, 0);

                            }


                            if (milisegundos % 30 == 0){
                                if (vilao2.active){
                                    if (vilao2.x + al_get_bitmap_width(vilao2.bitmap) > x &&
                                        vilao2.x < x + al_get_bitmap_width(sprite.bitmap) &&
                                        vilao2.y + al_get_bitmap_height(vilao2.bitmap) > y &&
                                        vilao2.y < y + al_get_bitmap_height(sprite.bitmap)){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                    }
                                }
                            }

                            for (int i = 0; i < 5; i++){
                                if (bullets[i].active)
                                {
                                    if (bullets[i].x >= vilao2.x && bullets[i].x <= vilao2.x + al_get_bitmap_width(vilao2.bitmap) &&
                                        bullets[i].y >= vilao2.y && bullets[i].y <= vilao2.y + al_get_bitmap_height(vilao2.bitmap))
                                    {
                                        bullets[i].active = false; // Desativa o projétil
                                        vilao2.hits++;             // Incrementa os "hits" no vilão
                                        pontuacao += 3;            // Incrementa a pontuação
                                    }
                                    if (bullets[i].x < 0 || bullets[i].x > X_SCREEN || bullets[i].y < 0 || bullets[i].y > Y_SCREEN)
                                    {
                                        bullets[i].active = false;
                                    }
                                }
                            }


                                    
                        }
                    
                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map.active){    al_draw_scaled_bitmap(
                                tiro_longo_map.bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map.bitmap),
                                al_get_bitmap_height(tiro_longo_map.bitmap),
                                tiro_longo_map.x, tiro_longo_map.y,
                                60, 60, 0);
                            
                            tiro_longo_map.x -=3;
                        }
                    }
                    if (tiro_longo_map.active)
                    {
                        if (tiro_longo_map.x + al_get_bitmap_width(tiro_longo_map.bitmap) > x &&
                            tiro_longo_map.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            tiro_longo_map.y + al_get_bitmap_height(tiro_longo_map.bitmap) > y &&
                            tiro_longo_map.y < y + al_get_bitmap_height(sprite.bitmap))
                        {
                            tiro_longo_map.active=false;
                            pontuacao +=5;
                            tiro_longo = true;
                        }
                    }
                    if(segundos == 10){
                        tiro_longo_map.x = X_SCREEN +80;
                        tiro_longo_map.active = true;

                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map.active){    al_draw_scaled_bitmap(
                                tiro_longo_map.bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map.bitmap),
                                al_get_bitmap_height(tiro_longo_map.bitmap),
                                tiro_longo_map.x, tiro_longo_map.y,
                                60, 60, 0);
                            
                            tiro_longo_map.x -=3;
                        }
                        if(tiro_longo_map.x < 0){
                            tiro_longo_map.active = false;
                        }
                    }

                    if (tiro_longo_map.active)
                    {
                        if (tiro_longo_map.x + al_get_bitmap_width(tiro_longo_map.bitmap) > x &&
                            tiro_longo_map.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            tiro_longo_map.y + al_get_bitmap_height(tiro_longo_map.bitmap) > y &&
                            tiro_longo_map.y < y + al_get_bitmap_height(sprite.bitmap))
                        {
                            tiro_longo_map.active=false;
                            pontuacao +=5;
                            
                        
                            if(segundos < 10){tiro_longo = true;}
                            if(segundos < 10){tiro_transv = true;}
                        }
                    }
                    if(tiro_longo){
                        al_draw_text(font, al_map_rgb(0,0,0), 500, 500, 0, "pegou");       
                    }
                    if(tiro_longo){
                        al_draw_text(font, al_map_rgb(0,0,0), 500, 300, 0, "pegou tmb");       
                    }

                }

                //! ----------------------------------- FIM SEGUNDA FASE ------------------------------


                for (int d = 0; d < 5; d++){
                
                                if (bullets_inimigos[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(bullets_inimigos[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = bullets_inimigos[d].x;
                                    float tiro_x_max = bullets_inimigos[d].x + largura_tiro;
                                    float tiro_y_min = bullets_inimigos[d].y;
                                    float tiro_y_max = bullets_inimigos[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                            bullets_inimigos[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (bullets_inimigos2[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(bullets_inimigos2[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = bullets_inimigos2[d].x;
                                    float tiro_x_max = bullets_inimigos2[d].x + largura_tiro;
                                    float tiro_y_min = bullets_inimigos2[d].y;
                                    float tiro_y_max = bullets_inimigos2[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                            bullets_inimigos2[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (tiros_v1[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(tiros_v1[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1[d].x;
                                    float tiro_x_max = tiros_v1[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1[d].y;
                                    float tiro_y_max = tiros_v1[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite.hits++;
                                            pontuacao -= 2;
                                            tiros_v1[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (tiros_v1_2[d].active){
                                    float largura_tiro = 40;
                                    float altura_tiro = al_get_bitmap_height(tiros_v1_2[d].bitmap);
                                    float largura_sprite = al_get_bitmap_width(sprite.bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite.bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1_2[d].x;
                                    float tiro_x_max = tiros_v1_2[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_2[d].y;
                                    float tiro_y_max = tiros_v1_2[d].y + 40;

                                    // Retângulo do sprite
                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    // Desenha o retângulo do tiro (vermelho)
                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    // Verifica a colisão
                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        sprite.hits++;
                                        pontuacao -= 2;
                                        tiros_v1_2[d].active = false; // Desativa o tiro
                                    }
                                }
                                //! <!-=====================================================               consertar essa parte comentada +++++++++++++++++++++++++++++++++++++++++++- >
                }
                //! desenha os tiros do jogador

                for (int i = 0; i < 5; i++)
                {
                    if (bullets[i].active)
                    {
                        al_draw_filled_rectangle(bullets[i].x, bullets[i].y, bullets[i].x + 30, bullets[i].y + 15, al_map_rgb(255, 255, 0));
                    }
            
                }

                //! desenha as infos de tempo e pontuação na tela
                sprintf(tempo, "%02d:%02d", minutos, segundos);
                al_draw_text(font, al_map_rgb(255, 255, 255), 20, 10, 0, tempo);

                sprintf(pont, "Pontuação: %d", sprite.hits);
                al_draw_text(font, al_map_rgb(255, 255, 255), 500, 610, 0, pont);

                
                //!  power scalling
                ALLEGRO_COLOR cor = al_map_rgb(0, 255, 0);

                if(ppower >=3){ cor = al_map_rgb(255, 0, 0);}
                ppower = 5;
                for (int t = 0; t < ppower; t++) {
                    al_draw_filled_rectangle(50 + 60*t, Y_SCREEN-50, 100 + 60*t, Y_SCREEN-30, cor);
                }

                //! CONTROLA OS CORAÇÕES
                int coracoes_num = sprite.hits - 1;
                for (int i = 2; i > coracoes_num; i--)
                {
                    if (cor1[i].active)
                    {
                        al_draw_scaled_bitmap(
                            cor1[i].bitmap,
                            0, 0,
                            al_get_bitmap_width(cor1[i].bitmap),
                            al_get_bitmap_height(cor1[i].bitmap),
                            cor1[i].x, cor1[i].y,
                            coracao_largura, coracao_altura, 0);
                    }
                }

                //! controle de tiros da fase 1 
                if (segundos % 2 == 0 && fase == 1){
                    if(final ==0){
                        if(Aviao1.active){
                            atiraInimigo(al_get_bitmap_width(Aviao1.bitmap), al_get_bitmap_height(Aviao1.bitmap), bullets_inimigos, Aviao1.x, Aviao1.y);
                        }
                        if(bomba.active){
                            atiraInimigo(al_get_bitmap_width(bomba.bitmap), al_get_bitmap_height(bomba.bitmap), bullets_inimigos2, bomba.x, bomba.y);
                        if(bomba.x == X_SCREEN-200 && bomba_mov < 90){
                            bomba_mov = 90;

                        }}
                    }

                    if(final ==1){
                                                    
                        if ((segundos < final_horizontal) || 
                            (segundos > final_branco2 && segundos <= final_horizontal2) || 
                            (segundos > final_vertical2)){
                            atiraInimigo(al_get_bitmap_width(vilao1.bitmap), al_get_bitmap_height(vilao1.bitmap), tiros_v1_2, vilao1.x, vilao1.y);
                            atiraInimigo(al_get_bitmap_width(vilao1.bitmap), al_get_bitmap_height(vilao1.bitmap), tiros_v1, vilao1.x, vilao1.y+25);
                        }                        
                    }
                }

                if(milisegundos %60 == 0 && fase == 1 && final ==1){
                    if (segundos > final_branco && segundos < final_com_vertical || segundos > final_branco3 && segundos < final_vertical2){
                            atiraInimigo_vertical(al_get_bitmap_width(vilao1.bitmap), al_get_bitmap_height(vilao1.bitmap), tiros_v1_vertical, vilao1.x, vilao1.y);
                    }
                }
                

                if (milisegundos %60 == 0 && fase == 2)
                {
                    if(final ==0){
                        if(Aviao2.active){
                            atiraInimigo(al_get_bitmap_width(Aviao2.bitmap), al_get_bitmap_height(Aviao2.bitmap), bullets_inimigos2, Aviao2.x, Aviao2.y+25);

                        }
                        if(Aviao3.active){
                            atiraInimigo(al_get_bitmap_width(Aviao3.bitmap), al_get_bitmap_height(Aviao3.bitmap), tiros_v1, Aviao3.x, Aviao3.y+25);

                        }
                        if(Aviao4.active){
                            atiraInimigo(al_get_bitmap_width(Aviao4.bitmap), al_get_bitmap_height(Aviao4.bitmap), tiros_v1_2, Aviao4.x, Aviao4.y+25);
                    }
                }
                    if(final ==1){
                        if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)){
                        
                            atiraInimigo(al_get_bitmap_width(vilao2.bitmap), al_get_bitmap_height(vilao2.bitmap), bullets_inimigos2, vilao2.x, vilao2.y+25);
                        }
                        if ((segundos > final_branco && segundos <= final_com_vertical)){
                            atiraInimigo(al_get_bitmap_width(vilao2.bitmap), al_get_bitmap_height(vilao2.bitmap), tiros_v1, vilao2.x, vilao2.y+25);

                            atiraInimigo_verticalv2(al_get_bitmap_width(vilao2.bitmap), al_get_bitmap_height(vilao2.bitmap), tiros_v1_vertical, vilao2.x, vilao2.y);
                            atiraInimigo_verticalv2(al_get_bitmap_width(vilao2.bitmap), al_get_bitmap_height(vilao2.bitmap), bullets_inimigos, vilao2.x, vilao2.y);
                        }}

                }

                if(tiro_transv && permissao_transv == 1){
                    atira_transv(sprite_width, sprite_height, bullets, x, y, font);
                    controler_tiros_especiais +=1;
                    if(controler_tiros_especiais >=250){
                        permissao_transv = 0;
                        tiro_transv = false;
                        controler_tiros_especiais = 0;

                    }
                }
                if(tiro_longo && permissao_longo == 1){
                    atira_transv(sprite_width, sprite_height, bullets, x, y, font);

                    //atira_longo(sprite_width, sprite_height, bullets, x, y, font);
                    controler_tiros_especiais +=1;
                    if(controler_tiros_especiais >=250){
                        permissao_longo = 0;
                        tiro_longo = false;
                        controler_tiros_especiais = 0;

                    }
                }
        

                al_flip_display();

                al_rest(0.01);

            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sprite.hits =0;
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_K) {
                if(tiro_transv){
                    permissao_transv =1;
                }
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_M){
                if(tiro_longo){
                    permissao_longo =1;
                }
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                key_left = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                key_right = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                key_up = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                key_down = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                key_space = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT)
                key_left = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                key_right = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_UP)
                key_up = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN)
                key_down = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE)
                key_space = false;
        }

        if (key_space)
        {
            atira(sprite_width, sprite_height, bullets, x, y);
        }
    }

    al_destroy_bitmap(cor1[0].bitmap);
    al_destroy_bitmap(cor1[1].bitmap);
    al_destroy_bitmap(cor1[2].bitmap);
    al_destroy_bitmap(vilao1.bitmap);
    al_destroy_bitmap(vilao2.bitmap);
    al_destroy_bitmap(sprite.bitmap);
    al_destroy_bitmap(bomba.bitmap);
    al_destroy_bitmap(Aviao1.bitmap);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
