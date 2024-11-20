//!   gcc -o game basico.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lpthread -ldl -lstdc++
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#define X_SCREEN 840
#define Y_SCREEN 680
#define BULLET_SPEED 10
#define AVIAO_VELOCIDADE 4.0
#define BULLET_SPEED_inimigos 10

typedef struct {
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Boss1;

typedef struct {
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Boss2;

typedef struct {
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} Bomba;

typedef struct {
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} AviaoInimigoVertical;

typedef struct {
    float x, y;
    int hits;
    bool active;
    ALLEGRO_BITMAP *bitmap;
} AviaoInimigoHorizontal;

typedef struct {
    float x, y;
    bool active;
} BulletInimigo;

typedef struct {
    float x, y;
    bool active;
    int hits;
    ALLEGRO_BITMAP *bitmap;
} jogador;

typedef struct{
    float x, y;
    bool active;
    ALLEGRO_BITMAP *bitmap;
}Coracao;


typedef struct {
    float x, y;
    bool active;
} Bullet;

void atira(int sprite_width, int sprite_height, Bullet bullets[], int x, int y) {
    for (int i = 0; i < 5; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x + sprite_width;
            bullets[i].y = y + sprite_height / 2 - 2;
            bullets[i].active = true;
            break;
        }
    }
}

void atiraInimigo(int sprite_width, int sprite_height, Bullet bullets[], int x, int y) {
    for (int i = 0; i < 5; i++) {
        if (!bullets[i].active) {
            bullets[i].x = x - sprite_width;
            bullets[i].y = y - sprite_height / 2 - 2;
            bullets[i].active = true;
            break;
        }
    }
}

void inicializar_jogo(void) {
    if (!al_init()) {
        printf("Falha ao inicializar o Allegro!\n");
        return;
    }

    if (!al_init_image_addon()) {
        printf("Falha ao inicializar o addon de imagens!\n");
        return;
    }

    if (!al_init_primitives_addon()) {
        printf("Falha ao inicializar o addon de primitivas!\n");
        return;
    }

    if (!al_install_keyboard()) {
        printf("Falha ao instalar o teclado!\n");
        return;
    }

    if (!al_init_font_addon()) {
        printf("Falha ao inicializar o addon de fontes.\n");
        return;
    }

    if (!al_init_ttf_addon()) {
        printf("Falha ao inicializar o addon TTF.\n");
        return;
    }
}

void reset_game(jogador *sprite, Bomba *bomba, AviaoInimigoVertical *Aviao1) {
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


//!  --------------------------------------------------------------  COMEÇO DO JOGO ----------------------------------------------------------------

int main() {
    inicializar_jogo();

    ALLEGRO_DISPLAY *display = al_create_display(X_SCREEN, Y_SCREEN);
    if (!display) {
        printf("Falha ao criar a janela!\n");
        return -1;
    }
    ALLEGRO_FONT *font = al_load_font("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/DM_Mono,Edu_AU_VIC_WA_NT_Pre/DM_Mono/DMMono-Light.ttf", 36, 0);
    if (!font) {
        printf("Falha ao carregar a fonte.\n");
        al_destroy_display(display);
        return -1;
    }
    
    ALLEGRO_EVENT_QUEUE *queue = al_create_event_queue();
    if (!queue) {
        printf("Falha ao criar a fila de eventos!\n");
        al_destroy_display(display);
        return -1;
    }

    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0); // 60 FPS
    if (!timer) {
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

    float speed = 8.0;

    Bullet bullets[5] = {{0}};
    
    Bullet bullets_inimigos[5] = {{0}};



            //! ----------------------------------------------INIMIGOS --------------------------------------------------
    Boss1 vilao1;
    vilao1.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Flash_10_80x110.png");
    if (!vilao1.bitmap) {
        printf("Falha ao carregar a sprite da bomba!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    vilao1.x = X_SCREEN - 100;
    vilao1.y = (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap)) / 2;
    vilao1.hits = 0;
    vilao1.active = true;

    Boss2 vilao2;
    vilao2.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Magicfly_47x65.png");
    if (!vilao2.bitmap) {
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
    if (!bomba.bitmap) {
        printf("Falha ao carregar a sprite da bomba!\n");
        al_destroy_bitmap(sprite.bitmap);
        al_destroy_display(display);
        return -1;
    }

    bomba.x = X_SCREEN - 100;
    bomba.y = (Y_SCREEN - al_get_bitmap_height(bomba.bitmap)) / 2;
    bomba.hits = 0;
    bomba.active = true;

    AviaoInimigoVertical Aviao1;
    Aviao1.bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/Scorpion_Evo_112x102.png");
    if (!Aviao1.bitmap) {
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

    Coracao cor1[3]; 
    cor1[0].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[1].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[2].bitmap = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/cor.png");
    cor1[0].x = 100; cor1[0].y = Y_SCREEN-50; cor1[0].active = true;
    cor1[1].x = 150; cor1[1].y = Y_SCREEN-50; cor1[1].active = true;
    cor1[2].x = 200; cor1[2].y = Y_SCREEN-50; cor1[2].active = true;


    //! ----------------------------------------------CONFIGURAÇÕES DOS ELEMENTOS GERAL --------------------------------------------------


    ALLEGRO_BITMAP *fundo = al_load_bitmap("/home/gabriel/Documentos/PROG 2/Fase 3 - Implementando movimentação e colisão/SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    ALLEGRO_BITMAP *fundo2 = al_load_bitmap("SpaceShipSamples/f.png");

    ALLEGRO_BITMAP *carrossel1[3];
    carrossel1[0] =  al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[1] =  al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[2] =  al_load_bitmap("SpaceShipSamples/f.png");



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
    int mov =1;
    int fase = 1;
    int pontuacao =0;
    float coracao_altura=30.0;

    float coracao_largura=30.0;

    ALLEGRO_EVENT event;
    bool morreu = false;

    while (running) {
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            if (!inicio && morreu == false && fase!= 3 && fase!=4) {
                al_clear_to_color(al_map_rgb(0, 0, 0)); 


                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 300, ALLEGRO_ALIGN_CENTRE, "Pressione Space para começar!");
                al_flip_display();
            }

            if (morreu) {
                inicio = false;
                
                al_clear_to_color(al_map_rgb(0, 0, 0)); 
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Morreu");

                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
                reset_game(&sprite, &bomba, &Aviao1);

                if (key_space) {
                    inicio = true;
                    morreu = false;
                                    pontuacao =0;

                }

                al_flip_display();
            }
            
            if (fase==4 ){
                inicio = false;
                
                al_clear_to_color(al_map_rgb(0, 0, 0)); 
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "1 fase completada!");

                sprintf(pont, "Pontuação atual: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space"); 
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTRE,"para ir para a fase 2!");
                reset_game(&sprite, &bomba, &Aviao1);

                if (key_space) {
                    inicio = true;
                    fase=2;
                    morreu = false;

                }

                al_flip_display();
            }

            
            if (fase==3){
                inicio = false;
                
                al_clear_to_color(al_map_rgb(0, 0, 0)); 
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Você venceu!");

                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
                reset_game(&sprite, &bomba, &Aviao1);

                if (key_space) {
                    inicio = true;
                    fase=1;
                    morreu = false;
                    pontuacao =0;

                }

                al_flip_display();
            }

            if (key_space && !inicio) {
                inicio = true;
                key_space = false;

            }

            if (inicio) {

                    milisegundos++;
                    if (milisegundos == 60) {
                        segundos++;
                        milisegundos = 0;
                    }
                    if (segundos == 60) {
                        minutos++;
                        segundos = 0;
                    }
                    
                    //!  ----------------------- configurações do plano de fundo ---------------------------------------------------

                    al_clear_to_color(al_map_rgb(0, 0, 0)); 
                    if(fase==1){
                        for (int i = 0; i < 3; i++) {
                            al_draw_bitmap(carrossel1[i], pos_x[i], 0, 0);
                        }

                        for (int i = 0; i < 3; i++) {
                            pos_x[i] -= velocidade;

                            if (pos_x[i] <= -al_get_bitmap_width(carrossel1[i])) {
                                pos_x[i] = pos_x[(i + 1) % 3] + al_get_bitmap_width(carrossel1[i]);
                            }
                        }

                    }
                    if(fase==2){
                        for (int i = 0; i < 3; i++) {
                            float escala_largura = (float)Y_SCREEN / al_get_bitmap_height(carrossel[i]) * al_get_bitmap_width(carrossel[i]);

                                // Desenhar a imagem escalada
                                al_draw_scaled_bitmap(
                                    carrossel[i], 
                                    0, 0,  // Origem no bitmap
                                    al_get_bitmap_width(carrossel[i]), 
                                    al_get_bitmap_height(carrossel[i]), 
                                    pos_x[i], 0,  // Posição na tela
                                    escala_largura, Y_SCREEN,  // Escala para a altura da tela
                                    0  // Sem inversão
                                );                        }

                        for (int i = 0; i < 3; i++) {
                            pos_x[i] -= velocidade;

                            if (pos_x[i] <= -al_get_bitmap_width(carrossel[i])) {
                                pos_x[i] = pos_x[(i + 1) % 3] + al_get_bitmap_width(carrossel[i]);
                            }
                        }

                    }



                    //! Atualiza a posição da nave do jogador
                    if (key_left) x -= speed;
                    if (key_right) x += speed;
                    if (key_up) y -= speed;
                    if (key_down) y += speed;

                    //! Impede a nave de sair da tela
                    if (x < 0) x = 0;
                    if (x + sprite_width > X_SCREEN) x = X_SCREEN - sprite_width;
                    if (y < 60) y = 60;
                    if (y + sprite_height > Y_SCREEN - 60) y = Y_SCREEN - sprite_height - 60;
                    //! Tiros do jogador contra inimigos


                    for (int i = 0; i < 5; i++) {
                        if (bullets[i].active) {
                            bullets[i].x += BULLET_SPEED;
                            if (bullets[i].x > X_SCREEN) bullets[i].active = false; // Desativa o tiro após sair da tela
                        }
                    }


                    //! Tiros dos inimigos contra o jogador
                    for (int i = 0; i < 5; i++) {
                        if (bullets_inimigos[i].active) {
                            bullets_inimigos[i].x -= BULLET_SPEED_inimigos;
                            if (bullets_inimigos[i].x > X_SCREEN) bullets_inimigos[i].active = false; 
                        }
                    }
                    for (int i = 0; i < 5; i++) {
                        if (bullets_inimigos[i].active) {
                            if (bullets_inimigos[i].x + 10 > x &&
                                bullets_inimigos[i].x < x + al_get_bitmap_width(sprite.bitmap) &&
                                bullets_inimigos[i].y + 5 > y &&
                                bullets_inimigos[i].y < y + al_get_bitmap_height(sprite.bitmap)) {
                                sprite.hits++;
                                bullets_inimigos[i].active = false; 
                            }
                        }
                    }
                    if (sprite.hits >= 3) {
                        morreu = true;
                    }


                    al_draw_bitmap(sprite.bitmap, x, y, 0);


                //! ------------------------------------------------------------ FASE 1 ------------------------------------------------------------
                
                if(fase ==1){
                    al_draw_text(font, al_map_rgb(255, 255, 255), 300, 10, 0, "FASE 1");
                    
                    //! Atualiza a posição do alvo (bomba)
                    bomba.x -= 5;
                    if (bomba.x < -al_get_bitmap_width(bomba.bitmap)) {
                        bomba.x = X_SCREEN - 100;
                        bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                    }

                    //! Atualiza a posição do avião inimigo vertical
                    if (mov == 1) {
                        Aviao1.y -= ps;
                        if (Aviao1.y <= 60) {  
                            mov = 2;
                            }
                    } else {
                        Aviao1.y += ps;
                        if (Aviao1.y + al_get_bitmap_height(Aviao1.bitmap) >= Y_SCREEN-60) {  
                            mov = 1; 
                        }
                    }

                    if (bomba.active) {
                        if (bomba.x + al_get_bitmap_width(bomba.bitmap) > x && 
                            bomba.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            bomba.y + al_get_bitmap_height(bomba.bitmap) > y &&
                            bomba.y < y + al_get_bitmap_height(sprite.bitmap)) {
                            pontuacao -= 5;
                            sprite.hits++;

                            bomba.x = X_SCREEN + 100;
                            bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                        }
                    }

                    for (int i = 0; i < 5; i++) {
                        if (bullets[i].active) {
                            // Colisão com a bomba
                            if (bullets[i].x >= bomba.x && bullets[i].x <= bomba.x + al_get_bitmap_width(bomba.bitmap) &&
                                bullets[i].y >= bomba.y && bullets[i].y <= bomba.y + al_get_bitmap_height(bomba.bitmap)) {
                                bomba.hits++; 
                                pontuacao += 3;
                                bullets[i].active = false;  
                            }

                            if (bullets[i].x >= Aviao1.x && bullets[i].x <= Aviao1.x + al_get_bitmap_width(Aviao1.bitmap) &&
                                bullets[i].y >= Aviao1.y && bullets[i].y <= Aviao1.y + al_get_bitmap_height(Aviao1.bitmap)) {
                                Aviao1.hits++; 
                                pontuacao +=5;
                                bullets[i].active = false; 
                            }
                        }
                    }

                    if (bomba.active) {
                        if (bomba.x + al_get_bitmap_width(bomba.bitmap) > x && 
                            bomba.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            bomba.y + al_get_bitmap_height(bomba.bitmap) > y &&
                            bomba.y < y + al_get_bitmap_height(sprite.bitmap)) {

                            sprite.hits++;
                            pontuacao -= 2;

                            bomba.x = X_SCREEN + 100;
                            bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));
                        }
                    }
                    if (Aviao1.active) {
                        if (Aviao1.x + al_get_bitmap_width(Aviao1.bitmap) > x && 
                            Aviao1.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            Aviao1.y + al_get_bitmap_height(Aviao1.bitmap) > y &&
                            Aviao1.y < y + al_get_bitmap_height(sprite.bitmap)) {

                            sprite.hits++;
                            pontuacao -= 2;

                            Aviao1.x = X_SCREEN + 100;
                            Aviao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao1.bitmap));
                        }
                    }

                    if (bomba.hits >= 3) {
                        bomba.x = X_SCREEN + 100;  
                        bomba.y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba.bitmap));  
                        bomba.hits = 0; 
                        bomba.active = true;  
                    }

                    if (Aviao1.hits >= 3) {
                        Aviao1.active = false;
                    }


                    if (bomba.active) {
                        al_draw_bitmap(bomba.bitmap, bomba.x, bomba.y, 0);
                    }

                    if (Aviao1.active) {
                        al_draw_bitmap(Aviao1.bitmap, Aviao1.x, Aviao1.y, 0);
                    }

                    if(pontuacao >= 100){
                        sprite.hits=0;
                        fase = 4;
                    }

                }

                //! ------------------------------------------------------------ FASE 2 ------------------------------------------------------------
                
                
                if(fase ==2){
                    al_draw_text(font, al_map_rgb(255, 255, 255), 300, 10, 0, "FASE 2");

                    //! Atualiza a posição do alvo (bomba)
                    vilao1.x -= 5;
                    if (vilao1.x < -al_get_bitmap_width(vilao1.bitmap)) {
                        vilao1.x = X_SCREEN - 100;
                        vilao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap));
                    }

                    //! Atualiza a posição do avião inimigo vertical
                    if (mov == 1) {
                        vilao2.y -= ps;
                        if (vilao2.y <= 60) {  
                            mov = 2;  
                        }
                    } else {
                        vilao2.y += ps;
                        if (vilao2.y + al_get_bitmap_height(vilao2.bitmap) >= Y_SCREEN-60) { 
                            mov = 1;  
                        }
                    }

                    if (vilao1.active) {
                        if (vilao1.x + al_get_bitmap_width(vilao1.bitmap) > x && 
                            vilao1.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            vilao1.y + al_get_bitmap_height(vilao1.bitmap) > y &&
                            vilao1.y < y + al_get_bitmap_height(sprite.bitmap)) {
                            pontuacao -=5;
                            sprite.hits++;

                            vilao1.x = X_SCREEN + 100;
                            vilao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap));
                        }
                    }

                    for (int i = 0; i < 5; i++) {
                        if (bullets[i].active) {
                            // Colisão com a bomba
                            if (bullets[i].x >= vilao1.x && bullets[i].x <= vilao1.x + al_get_bitmap_width(vilao1.bitmap) &&
                                bullets[i].y >= vilao1.y && bullets[i].y <= vilao1.y + al_get_bitmap_height(vilao1.bitmap)) {
                                vilao1.hits++;  
                                pontuacao += 8;
                                bullets[i].active = false; 
                            }

                            if (bullets[i].x >= vilao2.x && bullets[i].x <= vilao2.x + al_get_bitmap_width(vilao2.bitmap) &&
                                bullets[i].y >= vilao2.y && bullets[i].y <= vilao2.y + al_get_bitmap_height(vilao2.bitmap)) {
                                vilao2.hits++;  
                                pontuacao += 10;
                                bullets[i].active = false;  
                            }
                        }
                    }

                    if (vilao1.active) {
                        if (vilao1.x + al_get_bitmap_width(vilao1.bitmap) > x && 
                            vilao1.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            vilao1.y + al_get_bitmap_height(vilao1.bitmap) > y &&
                            vilao1.y < y + al_get_bitmap_height(sprite.bitmap)) {
                            
                            pontuacao -= 5;
                            sprite.hits++;

                            vilao1.x = X_SCREEN + 100;
                            vilao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap));
                        }
                    }
                    if (vilao2.active) {
                        if (vilao2.x + al_get_bitmap_width(vilao2.bitmap) > x && 
                            vilao2.x < x + al_get_bitmap_width(sprite.bitmap) &&
                            vilao2.y + al_get_bitmap_height(vilao2.bitmap) > y &&
                            vilao2.y < y + al_get_bitmap_height(sprite.bitmap)) {
                            pontuacao -= 5;
                            sprite.hits++;

                            vilao2.x = X_SCREEN + 100;
                            vilao2.y = rand() % (Y_SCREEN - al_get_bitmap_height(vilao2.bitmap));
                        }
                    }

                    if (vilao1.hits >= 3) {
                        vilao1.x = X_SCREEN + 100;  
                        vilao1.y = rand() % (Y_SCREEN - al_get_bitmap_height(vilao1.bitmap)); 
                        vilao1.hits = 0; 
                        vilao1.active = true; 
                    }

                    if (vilao2.hits >= 3) {
                        vilao2.active = false;
                    }


                    if (vilao1.active) {
                        al_draw_bitmap(vilao1.bitmap, vilao1.x, vilao1.y, 0);
                    }

                    if (vilao2.active) {
                        al_draw_bitmap(vilao2.bitmap, vilao2.x, vilao2.y, 0);
                    }
                
                    if(pontuacao >= 200){
                        fase = 3;
                    }
                }

                //! ----------------------------------- FIM SEGUNDA FASE ------------------------------

                
                for (int i = 0; i < 5; i++) {
                    if (bullets[i].active) {
                        al_draw_filled_rectangle(bullets[i].x, bullets[i].y, bullets[i].x + 10, bullets[i].y + 5, al_map_rgb(255, 255, 0));
                    }
                }

                sprintf(tempo, "%02d:%02d", minutos, segundos);
                al_draw_text(font, al_map_rgb(255, 255, 255), 20, 10, 0, tempo);

                sprintf(pont, "Pontuação: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 600, 0, pont);

                //! CONTROLA OS CORAÇÕES 
                int coracoes_num = sprite.hits-1;
                    for (int i = 2; i > coracoes_num; i--) {
                        if (cor1[i].active) {
                            al_draw_scaled_bitmap(
                                cor1[i].bitmap, 
                                0, 0,                                      
                                al_get_bitmap_width(cor1[i].bitmap),      
                                al_get_bitmap_height(cor1[i].bitmap),     
                                cor1[i].x, cor1[i].y,                  
                                coracao_largura, coracao_altura,0);

                        }
                    }

                al_flip_display();

        al_rest(0.01);
            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) key_left = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key_right = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) key_up = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) key_down = true;
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) key_space = true;
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            if (event.keyboard.keycode == ALLEGRO_KEY_LEFT) key_left = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_RIGHT) key_right = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_UP) key_up = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_DOWN) key_down = false;
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) key_space = false;
        }

        if (key_space) {
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
