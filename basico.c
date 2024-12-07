//!   gcc -o game basico.c -lallegro -lallegro_image -lallegro_primitives -lallegro_font -lallegro_ttf -lm -lpthread -ldl -lstdc++

#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

#include "geral.h"
#include "viloes.h"
#include "jogador.h"

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
    ALLEGRO_FONT *font = al_load_font("DM_Mono,Edu_AU_VIC_WA_NT_Pre/DM_Mono/DMMono-Light.ttf", 36, 0);

    ALLEGRO_FONT *font2 = al_load_font("DM_Mono,Edu_AU_VIC_WA_NT_Pre/DM_Mono/DMMono-Light.ttf", 22, 0);

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
    jogador *sprite = (jogador*) malloc(sizeof(jogador));
    sprite->bitmap = al_load_bitmap("SpaceShipSamples/Cruiser2_42x61.png");

    int sprite_width = al_get_bitmap_width(sprite->bitmap);
    int sprite_height = al_get_bitmap_height(sprite->bitmap);

    float x = 10;
    float y = (Y_SCREEN - sprite_height) / 2;
    sprite->altura = sprite_height;
    sprite->largura = sprite_width;

    float speed = 8.0;

    jogador *auxiliar1= (jogador*) malloc(sizeof(jogador));

    auxiliar1->bitmap = al_load_bitmap("SpaceShipSamples/Cruiser2_42x61.png");

    int auxiliar1_width = al_get_bitmap_width(auxiliar1->bitmap);
    int auxiliar1_heig = al_get_bitmap_height(auxiliar1->bitmap);

    float x_auxiliar1 = -70;
    float y_auxiliar1 = ((Y_SCREEN - sprite_height) / 2)-100;
    auxiliar1->altura = auxiliar1_width;
    auxiliar1->largura = auxiliar1_width;


    //!tiros amigos
    Bullet bullets[5] = {{0}};
    Bullet especial[5] = {{0}};
    Bullet especial2[5] = {{0}};

    //!tiros inimigos
    Bullet bullets_inimigos[5] = {{0}};
    Bullet bullets_inimigos2[5] = {{0}};
    Bullet tiros_v1[5];
    Bullet tiros_v1_2[5];
    Bullet tiros_v1_vertical[5];
    
    for(int j =0; j<5; j++){
        bullets[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        bullets_inimigos[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        bullets_inimigos2[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        especial[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        especial2[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        bullets_inimigos[j].altura = 15;
        bullets_inimigos[j].largura = 30;
        bullets_inimigos2[j].largura = 30;
        bullets_inimigos2[j].altura = 15;
        bullets[j].largura = 30;
        bullets[j].altura = 15;
        especial2[j].largura = 30;
        especial[j].altura = 15;
        tiros_v1[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        tiros_v1_2[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
        tiros_v1_vertical[j].bitmap = al_load_bitmap("SpaceShipSamples/UFO_PlasmaShield_Pink_69x69.png");
    }

    //! ----------------------------------------------INIMIGOS --------------------------------------------------
    Boss *vilao1 = (Boss*) malloc(sizeof(Boss));
    vilao1->bitmap = al_load_bitmap("SpaceShipSamples/Flash_10_80x110.png");

    vilao1->x = X_SCREEN - 100;
    vilao1->y = (Y_SCREEN - al_get_bitmap_height(vilao1->bitmap)) / 2;
    vilao1->hits = 0;
    vilao1->active = true;

    Boss *vilao2  = (Boss*) malloc(sizeof(Boss));
    vilao2->bitmap = al_load_bitmap("SpaceShipSamples/Fear_A_Shadow_100x100.png");

    vilao2->x = X_SCREEN - 100;
    vilao2->y = (Y_SCREEN - al_get_bitmap_height(vilao2->bitmap)) / 2;
    vilao2->hits = 0;
    vilao2->active = true;

    Bomba *bomba = (Bomba*) malloc(sizeof(Bomba));
    bomba->bitmap = al_load_bitmap("SpaceShipSamples/Magicfly_47x65.png");

    bomba->x = X_SCREEN - 100;
    bomba->y = (Y_SCREEN - al_get_bitmap_height(bomba->bitmap)) / 2;
    bomba->hits = 0;
    bomba->active = true;


    Bomba *bomba2 = (Bomba*) malloc(sizeof(Bomba));
    bomba2->bitmap = al_load_bitmap("SpaceShipSamples/Buggy_Black_63x69.png");

    bomba2->x = X_SCREEN - 50;
    bomba2->y = 50 + ((Y_SCREEN - al_get_bitmap_height(bomba2->bitmap)) / 2);
    bomba2->hits = 0;
    bomba2->active = true;

    AviaoInimigoVertical *Aviao1 = (AviaoInimigoVertical*) malloc(sizeof(AviaoInimigoVertical));
    Aviao1->bitmap = al_load_bitmap("SpaceShipSamples/Scorpion_Evo_112x102.png");
    Aviao1->x = X_SCREEN - 100;
    Aviao1->y = (Y_SCREEN - al_get_bitmap_height(Aviao1->bitmap)) / 1.5;
    Aviao1->hits = 0;
    Aviao1->active = true;


    AviaoInimigoVertical *Aviao2 = (AviaoInimigoVertical*) malloc(sizeof(AviaoInimigoVertical));
    Aviao2->bitmap = al_load_bitmap("SpaceShipSamples/TinyCruiser_Ice_64x64.png");
    Aviao2->x = X_SCREEN - 100;
    Aviao2->y = (Y_SCREEN/2)-100;
    Aviao2->hits = 0;
    Aviao2->active = true;


    AviaoInimigoVertical *Aviao3 = (AviaoInimigoVertical*) malloc(sizeof(AviaoInimigoVertical));;
    Aviao3->bitmap = al_load_bitmap("SpaceShipSamples/Flash_A_71x75.png");
    Aviao3->x = X_SCREEN + 100;
    Aviao3->y = (Y_SCREEN/2)-100;
    Aviao3->hits = 0;
    Aviao3->active = true;

    AviaoInimigoVertical *Aviao4 = (AviaoInimigoVertical*) malloc(sizeof(AviaoInimigoVertical));;
    Aviao4->bitmap = al_load_bitmap("SpaceShipSamples/Flash_A_71x75.png");
    Aviao4->x = X_SCREEN + 100;
    Aviao4->y = (Y_SCREEN/2)+100;
    Aviao4->hits = 0;
    Aviao4->active = true;


   //! _________________________________________________________________ ITEMS _______________________________________________

    item *tiro_longo_map = (item*) malloc(sizeof(item));
    tiro_longo_map->bitmap = al_load_bitmap("SpaceShipSamples/busssLight.png");
    
    tiro_longo_map->y = Y_SCREEN -300;
    tiro_longo_map->x = X_SCREEN +100;
    tiro_longo_map->active = true;

    
    Coracao cor1[3];
    cor1[0].bitmap = al_load_bitmap("SpaceShipSamples/cor.png");
    cor1[1].bitmap = al_load_bitmap("SpaceShipSamples/cor.png");
    cor1[2].bitmap = al_load_bitmap("SpaceShipSamples/cor.png");
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

    ALLEGRO_BITMAP *fundo = al_load_bitmap("SpaceShipSamples/toystoynuvens.jpg");
    ALLEGRO_BITMAP *zurc = al_load_bitmap("SpaceShipSamples/zurg.jpeg");
    ALLEGRO_BITMAP *zurc_icon = al_load_bitmap("SpaceShipSamples/zzz.png");
    ALLEGRO_BITMAP *buzz = al_load_bitmap("SpaceShipSamples/busssLight.png");
    ALLEGRO_BITMAP *alien = al_load_bitmap("SpaceShipSamples/alien.png");
    
    ALLEGRO_BITMAP *carrossel1[3];
    carrossel1[0] = al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[1] = al_load_bitmap("SpaceShipSamples/f.png");
    carrossel1[2] = al_load_bitmap("SpaceShipSamples/f.png");

    ALLEGRO_BITMAP *carrossel[3];
    carrossel[0] = al_load_bitmap("SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    carrossel[1] = al_load_bitmap("SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");
    carrossel[2] = al_load_bitmap("SpaceShipSamples/pngtree-digital-game-background-picture-image_1598113.png");

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
    int ps = 2;
    int mov = 1;
    int fase = 1;
    int pontuacao = 0;
    float coracao_altura = 30.0;
    float coracao_largura = 30.0;
    int ppower=0;
    int ppower2=0;
    int mov_v1 = 1;
    int mov_v2 = 1;
    int bomba_mov =0;
    int tiro_transv = 0;
    int ctln_transversal=0;
    int ctln_longo=2;
    int tiro_longo = 0;
    int final =0;
    int ctnl_vilao2 =0;
    int ctnl_tiros_vertical_v2=false;
    ALLEGRO_EVENT event;
    bool morreu = false;
    int primeira_fase_comeco = 10; //! A fase começa no segundo 1
    int final_horizontal = primeira_fase_comeco + 9; //! 1-10: Vilão atira horizontalmente
    int final_branco = final_horizontal + 1;        //! 11: Tela branca
    int final_com_vertical = final_branco + 9;      //! 12-20: Vilão atira verticalmente
    int final_branco2 = final_com_vertical + 1;     //! 21: Tela branca
    int final_horizontal2 = final_branco2 + 9;      //! 22-30: Vilão atira horizontalmente
    int final_branco3 = final_horizontal2 + 1;      //! 31: Tela branca
    int final_vertical2 = final_branco3 + 9;        //! 32-40: Vilão atira verticalmente
    int personagem =0;
    int comeco=0;
    int segundos_total=0;
    int anima_seg=0;

    while (running){
        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER)
        {

            if (!inicio && morreu == false && fase== 1 && fase != 3  && comeco ==0){
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, X_SCREEN, Y_SCREEN, 0);


                al_draw_scaled_bitmap(buzz,0, 0,al_get_bitmap_width(buzz),al_get_bitmap_height(buzz),X_SCREEN-200, Y_SCREEN-200,200, 250, 0);
                al_draw_filled_rounded_rectangle((X_SCREEN/2)-300,(Y_SCREEN/2)+65, (X_SCREEN)-200, (Y_SCREEN)+100, 10,10, al_map_rgb(1,1,1));

                al_draw_filled_rounded_rectangle((X_SCREEN/2)-300,300, (X_SCREEN)-200, 200, 10,10, al_map_rgb(1,1,1));

                al_draw_text(font2, al_map_rgb(255, 255, 255), 400, 450, ALLEGRO_ALIGN_CENTRE, "Zurc sequestrou uma de nossas ");
                al_draw_text(font2, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTRE, "equipes de reconhecimento");

                al_draw_text(font2, al_map_rgb(255, 255, 255), 400, 550, ALLEGRO_ALIGN_CENTRE, "Vamos ter que salva-los!!!");

                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Pressione Space");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "para começar!");
                ctln_transversal =0;
                ctln_longo =0;
                tiro_longo =0;
                tiro_transv =0;
                if (key_space)
                {
                    comeco = 1;
                }
                }
                al_flip_display();

            }


            if (morreu == true && fase == 0){
                    inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));

                al_draw_scaled_bitmap(zurc, 0, 0, al_get_bitmap_width(zurc), al_get_bitmap_height(zurc), 0, 0, X_SCREEN, Y_SCREEN, 0);
                
                al_draw_filled_rounded_rectangle(50,(Y_SCREEN/2)+30,X_SCREEN-50, (Y_SCREEN/2)+250, 10,10, al_map_rgb(1,1,1));
                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Zurc vence novamente!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 450, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 420, 500, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
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
                ctln_transversal =0;
                ctln_longo =0;
                tiro_longo =0;
                tiro_transv =0;
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

                    vilao1->hits=0;
                    vilao2->hits=0;

                }

                al_flip_display();
            }

            if (fase == 4){
                inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, X_SCREEN, Y_SCREEN, 0);

                al_draw_filled_rounded_rectangle((X_SCREEN/2)-100,Y_SCREEN-65, (X_SCREEN/2)+100, Y_SCREEN, 10,10, al_map_rgb(1,1,1));
                al_draw_filled_rounded_rectangle(X_SCREEN-500,Y_SCREEN-60, X_SCREEN, Y_SCREEN, 10,10, al_map_rgb(225,225,210));
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "1 fase completada!");
                personagem=1;
                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Só mais um passo e derrotaremos Zurc!");

                sprintf(pont, "Pontuação atual: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 400, ALLEGRO_ALIGN_CENTRE, "Pressione Space");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 500, ALLEGRO_ALIGN_CENTRE, "para ir para a fase 2!");
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
                milisegundos = 0;
                segundos = 0;
                minutos = 0;
                ctln_transversal =0;
                ctln_longo =0;
                tiro_longo =0;
                tiro_transv =0;
                if (key_space)
                {
                    inicio = true;
                    fase = 2;
                    morreu = false;

                    vilao1->hits=0;
                    vilao2->hits=0;

                }

                al_flip_display();
            }

            if (fase == 3)
            {
                inicio = false;

                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_draw_scaled_bitmap(fundo, 0, 0, al_get_bitmap_width(fundo), al_get_bitmap_height(fundo), 0, 0, X_SCREEN, Y_SCREEN, 0);
                al_draw_scaled_bitmap(alien, 0, 0, al_get_bitmap_width(alien), al_get_bitmap_height(alien), 0, Y_SCREEN-200,300, 200, 0);

                al_draw_filled_rounded_rectangle(50,200, X_SCREEN-50, 400, 10,10, al_map_rgb(1,1,1));
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 200, ALLEGRO_ALIGN_CENTRE, "Você me salvou!");
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 250, ALLEGRO_ALIGN_CENTRE, "Sou eternamente grato!");

                al_draw_filled_rounded_rectangle(X_SCREEN-500,Y_SCREEN-60, X_SCREEN, Y_SCREEN, 10,10, al_map_rgb(225,225,210));

                sprintf(pont, "Pontuação final: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 200, 300, 0, pont);
                al_draw_text(font, al_map_rgb(255, 255, 255), 400, 350, ALLEGRO_ALIGN_CENTRE, "Pressione Space para recomeçar!");
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
                milisegundos = 0;
                segundos = 0;
                minutos = 0;
                ctln_transversal =0;
                ctln_longo =0;
                tiro_longo =0;
                tiro_transv =0;
                if (key_space)
                {
                    inicio = true;
                    fase = 1;
                    morreu = false;
                    pontuacao = 0;

                    vilao1->hits=0;
                    vilao2->hits=0;

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

                        al_draw_scaled_bitmap(
                            carrossel[i],
                            0, 0,
                            al_get_bitmap_width(carrossel[i]),
                            al_get_bitmap_height(carrossel[i]),
                            pos_x[i], 0,           
                            escala_largura, Y_SCREEN, 
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
                            bullets[i].active = false; 
                    }
                    if (especial2[i].active)
                    {
                        especial2[i].x += BULLET_SPEED;
                        especial2[i].y += BULLET_SPEED;
                        if (especial2[i].x > X_SCREEN || especial2[i].y > Y_SCREEN)
                            especial2[i].active = false; 
                    }
                    if (especial[i].active)
                    {
                        especial[i].x += BULLET_SPEED;
                        especial[i].y -= BULLET_SPEED;
                        if (especial[i].x > X_SCREEN || especial2[i].y <0)
                            especial[i].active = false; 
                    }
                    //!inimigos
                    if (bullets_inimigos[i].active && ctnl_tiros_vertical_v2 ==false)
                    {
                        bullets_inimigos[i].x -= BULLET_SPEED ;
                        if (bullets_inimigos[i].x < 10)
                            bullets_inimigos[i].active = false; 
                    }
                    if (bullets_inimigos2[i].active && ctnl_tiros_vertical_v2 ==false)
                    {
                        bullets_inimigos2[i].x -= BULLET_SPEED;
                        if (bullets_inimigos2[i].x < 10)
                            bullets_inimigos2[i].active = false; 
                    }
                    if (tiros_v1[i].active)
                    {
                        tiros_v1[i].x -= BULLET_SPEED;
                        if (tiros_v1[i].x < 0)
                            tiros_v1[i].active = false; 
                    }
                    
                    if (tiros_v1_2[i].active)
                    {
                        tiros_v1_2[i].x -= BULLET_SPEED;
                        if (tiros_v1_2[i].x < 10)
                            tiros_v1_2[i].active = false; 
                    }

                }

              //! Tiros do inimigo contra o jogador


                if (sprite->hits >= 3)
                {
                    morreu = true;
                    fase = 0;   
                }

                al_draw_bitmap(sprite->bitmap, x, y, 0);



                al_draw_filled_rounded_rectangle(X_SCREEN-505,Y_SCREEN-65, X_SCREEN, Y_SCREEN, 10,10, al_map_rgb(1,1,1));
                al_draw_filled_rounded_rectangle(X_SCREEN-500,Y_SCREEN-60, X_SCREEN, Y_SCREEN, 10,10, al_map_rgb(225,225,210));
                if(personagem==1){
                al_draw_scaled_bitmap(buzz,
                        0, 0,
                        al_get_bitmap_width(buzz),
                        al_get_bitmap_height(buzz),
                        X_SCREEN-100, Y_SCREEN-100,
                        100, 150, 0);}
                if(personagem == 2){
                al_draw_scaled_bitmap(zurc_icon,
                        0, 0,
                        al_get_bitmap_width(zurc_icon),
                        al_get_bitmap_height(zurc_icon),
                        X_SCREEN-100, Y_SCREEN-100,
                        100, 150, 0);

                }


                //! ------------------------------------------------------------ FASE 1 ------------------------------------------------------------

                if (fase == 1)
                {
                    al_draw_text(font, al_map_rgb(255, 255, 255), 300, 10, 0, "FASE 1");
                    if (segundos  == 0){ 
                        al_clear_to_color(al_map_rgb(255, 255, 255));
                        final =0;
                    }

                    if( segundos > 0 && minutos ==0 &&  final == 0 && segundos <= primeira_fase_comeco){

                                        if(segundos <=2){   
                                        personagem=1;
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Tenho que resgatar meus ");
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "amigos!");
                                        }
                                        if(segundos >= 3 && segundos <= 5){   
                                        personagem=2;
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Hoje será sua derrota");
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "Buzz Lightear!");
                                        }
                                        if(segundos == 6){   
                                        personagem=2;
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Tropas!");
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "AVANTE!");
                                        }if(segundos > 6){
                                            personagem =0;
                                        }

                                        //! Atualiza a posição do alvo (bomba)

                                        if(bomba_mov < 90 || bomba_mov > 180 ){
                                            bomba->x -= 5;   
                                        }else{
                                            bomba_mov ++;
                                        }
                                        if(bomba->x < 10){
                                            bomba_mov = 0;
                                        }

                                        if (bomba->x < -al_get_bitmap_width(bomba->bitmap))
                                        {
                                            bomba->x = X_SCREEN - 100;
                                            bomba->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba->bitmap));
                                        }

                                        //! Atualiza a posição do alvo (bomba)
                                        bomba2->x -= 5;
                                        if (bomba2->x < -al_get_bitmap_width(bomba2->bitmap))
                                        {
                                            bomba2->x = X_SCREEN - 100;
                                            bomba2->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2->bitmap));
                                        }
                                        //! Atualiza a posição do avião inimigo vertical
                                        if (mov == 1)
                                        {
                                            Aviao1->y -= ps;
                                            if (Aviao1->y <= 60)
                                            {
                                                mov = 2;
                                            }
                                        }
                                        else
                                        {
                                            Aviao1->y += ps;
                                            if (Aviao1->y + al_get_bitmap_height(Aviao1->bitmap) >= Y_SCREEN - 60)
                                            {
                                                mov = 1;
                                            }
                                        }
                                        if (bomba->active)
                                        {
                                            if (bomba->x + al_get_bitmap_width(bomba->bitmap) > x &&
                                                bomba->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                bomba->y + al_get_bitmap_height(bomba->bitmap) > y &&
                                                bomba->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite->hits++;
                                                printf("Bomba atingiu - linha 742");

                                                bomba->x = X_SCREEN + 100;
                                                bomba->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba->bitmap));
                                                bomba_mov =0;
                                            }
                                        }

                                        if (bomba2->active)
                                        {
                                            if (bomba2->x + al_get_bitmap_width(bomba2->bitmap) > x &&
                                                bomba2->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                bomba2->y + al_get_bitmap_height(bomba2->bitmap) > y &&
                                                bomba2->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite->hits++;

                                                bomba2->x = X_SCREEN + 100;
                                                bomba2->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2->bitmap));
                                            }
                                        }

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets[i].active)
                                            {
                                                // Colisão com a bomba
                                                if (bullets[i].x >= bomba->x && bullets[i].x <= bomba->x + al_get_bitmap_width(bomba->bitmap) &&
                                                    bullets[i].y >= bomba->y && bullets[i].y <= bomba->y + al_get_bitmap_height(bomba->bitmap))
                                                {
                                                    bomba->hits++;
                                                    pontuacao += 3;
                                                    bullets[i].active = false;
                                                }

                                                if (bullets[i].x >= Aviao1->x && bullets[i].x <= Aviao1->x + al_get_bitmap_width(Aviao1->bitmap) &&
                                                    bullets[i].y >= Aviao1->y && bullets[i].y <= Aviao1->y + al_get_bitmap_height(Aviao1->bitmap))
                                                {
                                                    Aviao1->hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }
                                                if (bullets[i].x >= bomba2->x && bullets[i].x <= bomba2->x + al_get_bitmap_width(bomba2->bitmap) &&
                                                    bullets[i].y >= bomba2->y && bullets[i].y <= bomba2->y + al_get_bitmap_height(bomba2->bitmap))
                                                {
                                                    bomba2->hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }                      
                                        }
                                            if (especial[i].active)
                                            {
                                                // Colisão com a bomba
                                                if (especial[i].x >= bomba->x && especial[i].x <= bomba->x + al_get_bitmap_width(bomba->bitmap) &&
                                                    especial[i].y >= bomba->y && especial[i].y <= bomba->y + al_get_bitmap_height(bomba->bitmap))
                                                {
                                                    bomba->hits++;
                                                    pontuacao += 3;
                                                    especial[i].active = false;
                                                }

                                                if (especial[i].x >= Aviao1->x && especial[i].x <= Aviao1->x + al_get_bitmap_width(Aviao1->bitmap) &&
                                                    especial[i].y >= Aviao1->y && especial[i].y <= Aviao1->y + al_get_bitmap_height(Aviao1->bitmap))
                                                {
                                                    Aviao1->hits++;
                                                    pontuacao += 5;
                                                    especial[i].active = false;
                                                }
                                                if (especial[i].x >= bomba2->x && especial[i].x <= bomba2->x + al_get_bitmap_width(bomba2->bitmap) &&
                                                    especial[i].y >= bomba2->y && especial[i].y <= bomba2->y + al_get_bitmap_height(bomba2->bitmap))
                                                {
                                                    bomba2->hits++;
                                                    pontuacao += 5;
                                                    especial[i].active = false;
                                                }     
                                                if (especial2[i].active){
                                                if (especial2[i].x >= bomba->x && especial2[i].x <= bomba->x + al_get_bitmap_width(bomba->bitmap) &&
                                                    especial2[i].y >= bomba->y && especial2[i].y <= bomba->y + al_get_bitmap_height(bomba->bitmap))
                                                {
                                                    bomba->hits++;
                                                    pontuacao += 3;
                                                    especial2[i].active = false;
                                                }

                                                if (especial2[i].x >= Aviao1->x && especial2[i].x <= Aviao1->x + al_get_bitmap_width(Aviao1->bitmap) &&
                                                    especial2[i].y >= Aviao1->y && especial2[i].y <= Aviao1->y + al_get_bitmap_height(Aviao1->bitmap))
                                                {
                                                    Aviao1->hits++;
                                                    pontuacao += 5;
                                                    especial2[i].active = false;
                                                }
                                                if (especial2[i].x >= bomba2->x && especial2[i].x <= bomba2->x + al_get_bitmap_width(bomba2->bitmap) &&
                                                    especial2[i].y >= bomba2->y && especial2[i].y <= bomba2->y + al_get_bitmap_height(bomba2->bitmap))
                                                {
                                                    bomba2->hits++;
                                                    pontuacao += 5;
                                                    especial2[i].active = false;
                                                }                      
                                        }
                                        }

                                        }
                                        if (bomba->active)
                                        {
                                            if (bomba->x + al_get_bitmap_width(bomba->bitmap) > x &&
                                                bomba->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                bomba->y + al_get_bitmap_height(bomba->bitmap) > y &&
                                                bomba->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                printf("Bomba atingiu - linha 803");
                                                bomba->x = X_SCREEN + 100;
                                                bomba->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba->bitmap));
                                                bomba_mov = 0;
                                            }
                                        }
                                        if (bomba2->active)
                                        {
                                            if (bomba2->x + al_get_bitmap_width(bomba2->bitmap) > x &&
                                                bomba2->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                bomba2->y + al_get_bitmap_height(bomba2->bitmap) > y &&
                                                bomba2->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                printf("Bomba2 atingiu - linha 819");
                                                bomba2->x = X_SCREEN + 100;
                                                bomba2->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2->bitmap));
                                            }
                                        }
                                        if (Aviao1->active)
                                        {
                                            if (Aviao1->x + al_get_bitmap_width(Aviao1->bitmap) > x &&
                                                Aviao1->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao1->y + al_get_bitmap_height(Aviao1->bitmap) > y &&
                                                Aviao1->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                printf("Aviao1 atingiu - linha 834");
                                                Aviao1->x = X_SCREEN + 100;
                                                Aviao1->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao1->bitmap));
                                            }
                                        }
                                        for (int d = 0; d < 5; d++) {
                                        if (bullets_inimigos[d].active) {
                                                float largura_tiro = 40;
                                                float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                                float altura_sprite = al_get_bitmap_height(sprite->bitmap);

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

                                                
                                                
                                                if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                                    tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max) {
                                                    
                                                    sprite->hits++;
                                                    pontuacao -= 2;
                                                    bullets_inimigos[d].active = false; // Desativa o tiro
                                                }
                                            }
                                                if (bullets_inimigos2[d].active) {
                                                float largura_tiro = 40;
                                                float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                                float altura_sprite = al_get_bitmap_height(sprite->bitmap);

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

                                                            al_draw_rectangle(
                                                    tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,
                                                    al_map_rgb(255, 0, 0), 2.0
                                                );

                                                            if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                                    tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max) {
                                                    
                                                    sprite->hits++;
                                                    pontuacao -= 2;
                                                    bullets_inimigos2[d].active = false; // Desativa o tiro
                                                }
                                            }
                                        }
                                        if (bomba->hits >= 3){
                                            bomba->x = X_SCREEN + 100;
                                            bomba->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba->bitmap));
                                            bomba->hits = 0;
                                            bomba->active = true;
                                            bomba_mov  =0;
                                        }

                                        if (Aviao1->hits >= 3)
                                        {
                                            Aviao1->active = false;
                                        }


                                        if (bomba2->hits >= 2)
                                        {
                                            bomba2->x = X_SCREEN + 100;
                                            bomba2->y = rand() % (Y_SCREEN - al_get_bitmap_height(bomba2->bitmap));
                                            bomba2->hits = 0;
                                            bomba2->active = true;
                                        }

                                        //! desenha os inimigos da fase
                                        if (bomba->active)
                                        {
                                            al_draw_bitmap(bomba->bitmap, bomba->x, bomba->y, 0);
                                        }

                                        if (bomba2->active)
                                        {
                                            al_draw_bitmap(bomba2->bitmap, bomba2->x, bomba2->y, 0);
                                        }
                                        if (Aviao1->active)
                                        {
                                            al_draw_bitmap(Aviao1->bitmap, Aviao1->x, Aviao1->y, 0);
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
                            }

                        }else{ 
                   //* ativa o vilão ou desativa
                            if (vilao1->hits >= 15){
                                personagem=2;
                                    al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "NÃOOOOOOOOOO");
                                
                            }

                            if (vilao1->hits >= 20){
                            
                                vilao1->active = false;
                                sprite->hits = 0;
                                final = 0;
                                fase = 4;
                                personagem =0;
                            }

                            //! BOSS FINAL DA FASE
                            if(segundos > primeira_fase_comeco ){
                            
                            final = 1;


                            if(segundos >= primeira_fase_comeco && segundos < (primeira_fase_comeco+3)){   
                            personagem=2;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Vamos ver se você");
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "encara minha arma secreta!");
                            }if(segundos > (primeira_fase_comeco+3)){
                                personagem =0;
                            }

                            if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)) {
                                    vilao1->x = X_SCREEN -100;
                            
                                al_draw_bitmap(vilao1->bitmap, vilao1->x, vilao1->y, 0);
                            
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
                                    vilao1->y -= ps;
                                    if (vilao1->y <= 60)
                                    {
                                        mov_v1 = 2;
                                    }
                                }
                                else
                                {
                                    vilao1->y += ps;
                                    if (vilao1->y + al_get_bitmap_height(vilao1->bitmap) >= Y_SCREEN)
                                    {
                                        mov_v1 = 1;
                                    }
                                }
                            } //? acaba aqui os primeiros 10 segundos do vilão

                            if (segundos == final_branco ){ 
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                            }

                            if ((segundos > final_branco && segundos <= final_com_vertical)  || (segundos > final_branco3 && segundos <= final_vertical2) ){
                                al_draw_bitmap(vilao1->bitmap, vilao1->x, 20, 0);
                                ps = 3;
                                if (mov_v1 == 1){
                                    vilao1->x -= ps;
                                    if (vilao1->x <= 0)
                                    {
                                        mov_v1 = 2;
                                    }
                                }else{
                                    vilao1->x += ps;
                                    if (vilao1->x + al_get_bitmap_height(vilao1->bitmap) >= X_SCREEN)
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
                                                        30, 30, 0);

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
                                vilao1->y = Y_SCREEN / 2;
                            }

                            if (milisegundos % 30 == 0){
                                if (vilao1->active){
                                    if (vilao1->x + al_get_bitmap_width(vilao1->bitmap) > x &&
                                        vilao1->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                        vilao1->y + al_get_bitmap_height(vilao1->bitmap) > y &&
                                        vilao1->y < y + al_get_bitmap_height(sprite->bitmap)){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                    }
                                }
                            }

                            for (int i = 0; i < 5; i++){
                                if (bullets[i].active)
                                {
                                    if (bullets[i].x >= vilao1->x && bullets[i].x <= vilao1->x + al_get_bitmap_width(vilao1->bitmap) &&
                                        bullets[i].y >= vilao1->y && bullets[i].y <= vilao1->y + al_get_bitmap_height(vilao1->bitmap))
                                    {
                                        bullets[i].active = false; 
                                        vilao1->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (bullets[i].x < 0 || bullets[i].x > X_SCREEN || bullets[i].y < 0 || bullets[i].y > Y_SCREEN)
                                    {
                                        bullets[i].active = false;
                                    }
                                }
                                if (especial[i].active){
                                    if (especial[i].x >= vilao1->x && especial[i].x <= vilao1->x + al_get_bitmap_width(vilao1->bitmap) &&
                                        especial[i].y >= vilao1->y && especial[i].y <= vilao1->y + al_get_bitmap_height(vilao1->bitmap))
                                    {
                                        especial[i].active = false; 
                                        vilao1->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (especial[i].x < 0 || especial[i].x > X_SCREEN || especial[i].y < 0 || especial[i].y > Y_SCREEN)
                                    {
                                        especial[i].active = false;
                                    }
                                }
                                if (especial2[i].active){
                                    if (especial2[i].x >= vilao1->x && especial2[i].x <= vilao1->x + al_get_bitmap_width(vilao1->bitmap) &&
                                        especial2[i].y >= vilao1->y && especial2[i].y <= vilao1->y + al_get_bitmap_height(vilao1->bitmap))
                                    {
                                        especial2[i].active = false; 
                                        vilao1->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (especial2[i].x < 0 || especial2[i].x > X_SCREEN || especial2[i].y < 0 || especial2[i].y > Y_SCREEN)
                                    {
                                        especial2[i].active = false;
                                    }
                                }
                            }


                            for (int d = 0; d < 5; d++){
                                if (tiros_v1[d].active){
                                    float largura_tiro = 40;
                                    
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

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

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                            tiros_v1[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (tiros_v1_2[d].active){
                                    float largura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1_2[d].x;
                                    float tiro_x_max = tiros_v1_2[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_2[d].y;
                                    float tiro_y_max = tiros_v1_2[d].y + 40;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        sprite->hits++;
                                        pontuacao -= 2;
                                        tiros_v1_2[d].active = false; // Desativa o tiro
                                    }
                                }
                                //! <!-=====================================================               consertar essa parte comentada +++++++++++++++++++++++++++++++++++++++++++- >
                                if (tiros_v1_vertical[d].active){
                                    
                                    float largura_tiro = 40;
                                    float altura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    // Retângulo do tiro (área de detecção vertical)
                                    float tiro_x_min = tiros_v1_vertical[d].x;
                                    float tiro_x_max = tiros_v1_vertical[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_vertical[d].y;
                                    float tiro_y_max = tiros_v1_vertical[d].y + altura_tiro;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        sprite->hits++;
                                        pontuacao -= 2;
                                        tiros_v1_vertical[d].active = false; // Desativa o tiro após a colisão
                                    }
                                }
                            }
                        }
                    }

                    
                    if(segundos >= 3){
                    
                        if(tiro_longo_map->active){    al_draw_scaled_bitmap(
                                tiro_longo_map->bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map->bitmap),
                                al_get_bitmap_height(tiro_longo_map->bitmap),
                                tiro_longo_map->x, tiro_longo_map->y,
                                60, 60, 0);
                            
                            tiro_longo_map->x -=3;
                        }
                    }
                    if (tiro_longo_map->active)
                    {
                        if (tiro_longo_map->x + al_get_bitmap_width(tiro_longo_map->bitmap) > x &&
                            tiro_longo_map->x < x + al_get_bitmap_width(sprite->bitmap) &&
                            tiro_longo_map->y + al_get_bitmap_height(tiro_longo_map->bitmap) > y &&
                            tiro_longo_map->y < y + al_get_bitmap_height(sprite->bitmap))
                        {
                            tiro_longo_map->active=false;
                            pontuacao +=5;
                            tiro_longo = true;
                        }
                    }

                    if(segundos == 10){
                        tiro_longo_map->x = X_SCREEN +80;
                        tiro_longo_map->active = true;

                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map->active){    
                            al_draw_scaled_bitmap(
                                tiro_longo_map->bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map->bitmap),
                                al_get_bitmap_height(tiro_longo_map->bitmap),
                                tiro_longo_map->x, tiro_longo_map->y,
                                60, 60, 0);
                            
                            tiro_longo_map->x -=3;
                        }
                        if(tiro_longo_map->x < 0){
                            tiro_longo_map->active = false;
                        }
                    }
                    if (tiro_longo_map->active)
                    {
                        if (tiro_longo_map->x + al_get_bitmap_width(tiro_longo_map->bitmap) > x &&
                            tiro_longo_map->x < x + al_get_bitmap_width(sprite->bitmap) &&
                            tiro_longo_map->y + al_get_bitmap_height(tiro_longo_map->bitmap) > y &&
                            tiro_longo_map->y < y + al_get_bitmap_height(sprite->bitmap))
                        {
                            tiro_longo_map->active=false;
                            pontuacao +=5;                            

                            if(segundos >=8){
                                tiro_longo =1;
                            }
                            if(segundos <10){
                            tiro_transv = 1;
                            }
                            ppower =4;
                        
                        }
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
                                        if(segundos <=2){   
                                        personagem=1;
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Vou te derrotar ");
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "ZURC!");
                                        }
                                        if(segundos >= 3 && segundos <= 5){   
                                        personagem=2;
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Seu fim será amargp");
                                            al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "Buzz Lightear!");
                                        }
                                        if(segundos > 5){
                                            personagem =0;
                                        }


                                        ps = 5;
                                            if (mov_v1 == 1)
                                            {
                                                Aviao3->y -= ps;
                                                Aviao3->x -=ps;
                                                if (Aviao3->y <= 60)
                                                {
                                                    mov_v1 = 2;
                                                }
                                            }
                                            else
                                            {
                                                Aviao3->y += ps;
                                                Aviao3->x -=ps;
                                                if (Aviao3->y + al_get_bitmap_height(Aviao3->bitmap) >= (Y_SCREEN/2))
                                                {
                                                    mov_v1 = 1;
                                                }
                                            }
                                        

                                        if (mov_v2 == 1)
                                        {
                                            Aviao4->y -= ps;
                                                Aviao4->x -=ps;
                                                if (Aviao4->y <= (Y_SCREEN/2))
                                                {
                                                    mov_v2 = 2;
                                                }
                                            }
                                            else
                                            {
                                                Aviao4->y += ps;
                                                Aviao4->x -=ps;
                                                if (Aviao4->y + al_get_bitmap_height(Aviao4->bitmap) >= Y_SCREEN)
                                                {
                                                    mov_v2 = 1;
                                                }
                                        }


                                        //! Atualiza a posição do avião inimigo vertical
                                        if (segundos % 4 == 0) {
                                            Aviao2->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao2->bitmap));
                                        }


                                        if (Aviao3->active)
                                        {
                                            if (Aviao3->x + al_get_bitmap_width(Aviao3->bitmap) > x &&
                                                Aviao3->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao3->y + al_get_bitmap_height(Aviao3->bitmap) > y &&
                                                Aviao3->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite->hits++;
                                                printf("Bomba atingiu - linha 742");

                                                Aviao3->x = X_SCREEN + 100;
                                                Aviao3->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3->bitmap));
                                            }
                                        }


                                        /////////////////////////!
                                        if (Aviao4->active)
                                        {
                                            if (Aviao4->x + al_get_bitmap_width(Aviao4->bitmap) > x &&
                                                Aviao4->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao4->y + al_get_bitmap_height(Aviao4->bitmap) > y &&
                                                Aviao4->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {
                                                pontuacao -= 5;
                                                sprite->hits++;
                                                printf("Bomba atingiu - linha 742");

                                                Aviao4->x = X_SCREEN + 100;
                                                Aviao4->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4->bitmap));
                                            }
                                        }

                                        for (int i = 0; i < 5; i++)
                                        {
                                            if (bullets[i].active)
                                            {
                                                // Colisão com a bomba
                                                if (bullets[i].x >= Aviao4->x && bullets[i].x <= Aviao4->x + al_get_bitmap_width(Aviao4->bitmap) &&
                                                    bullets[i].y >= Aviao4->y && bullets[i].y <= Aviao4->y + al_get_bitmap_height(Aviao4->bitmap))
                                                {
                                                    Aviao4->hits++;
                                                    pontuacao += 3;
                                                    bullets[i].active = false;
                                                }

                                                if (bullets[i].x >= Aviao3->x && bullets[i].x <= Aviao3->x + al_get_bitmap_width(Aviao3->bitmap) &&
                                                    bullets[i].y >= Aviao3->y && bullets[i].y <= Aviao3->y + al_get_bitmap_height(Aviao3->bitmap))
                                                {
                                                    Aviao3->hits++;
                                                    pontuacao += 3;
                                                    bullets[i].active = false;
                                                }

                                                if (bullets[i].x >= Aviao2->x && bullets[i].x <= Aviao2->x + al_get_bitmap_width(Aviao2->bitmap) &&
                                                    bullets[i].y >= Aviao2->y && bullets[i].y <= Aviao2->y + al_get_bitmap_height(Aviao2->bitmap))
                                                {
                                                    Aviao2->hits++;
                                                    pontuacao += 5;
                                                    bullets[i].active = false;
                                                }
                                        }
                                        if (especial2[i].active)
                                            {
                                                if (especial2[i].x >= Aviao4->x && especial2[i].x <= Aviao4->x + al_get_bitmap_width(Aviao4->bitmap) &&
                                                    especial2[i].y >= Aviao4->y && especial2[i].y <= Aviao4->y + al_get_bitmap_height(Aviao4->bitmap))
                                                {
                                                    Aviao4->hits++;
                                                    pontuacao += 3;
                                                    especial2[i].active = false;
                                                }

                                                if (especial2[i].x >= Aviao3->x && especial2[i].x <= Aviao3->x + al_get_bitmap_width(Aviao3->bitmap) &&
                                                    especial2[i].y >= Aviao3->y && especial2[i].y <= Aviao3->y + al_get_bitmap_height(Aviao3->bitmap))
                                                {
                                                    Aviao3->hits++;
                                                    pontuacao += 3;
                                                    especial2[i].active = false;
                                                }

                                                if (especial2[i].x >= Aviao2->x && especial2[i].x <= Aviao2->x + al_get_bitmap_width(Aviao2->bitmap) &&
                                                    especial2[i].y >= Aviao2->y && especial2[i].y <= Aviao2->y + al_get_bitmap_height(Aviao2->bitmap))
                                                {
                                                    Aviao2->hits++;
                                                    pontuacao += 5;
                                                    especial2[i].active = false;
                                                }
                                        }
                                        if (especial[i].active)
                                            {
                                                if (especial[i].x >= Aviao4->x && especial[i].x <= Aviao4->x + al_get_bitmap_width(Aviao4->bitmap) &&
                                                    especial[i].y >= Aviao4->y && especial[i].y <= Aviao4->y + al_get_bitmap_height(Aviao4->bitmap))
                                                {
                                                    Aviao4->hits++;
                                                    pontuacao += 3;
                                                    especial[i].active = false;
                                                }

                                                if (especial[i].x >= Aviao3->x && especial[i].x <= Aviao3->x + al_get_bitmap_width(Aviao3->bitmap) &&
                                                    especial[i].y >= Aviao3->y && especial[i].y <= Aviao3->y + al_get_bitmap_height(Aviao3->bitmap))
                                                {
                                                    Aviao3->hits++;
                                                    pontuacao += 3;
                                                    especial[i].active = false;
                                                }

                                                if (especial[i].x >= Aviao2->x && especial[i].x <= Aviao2->x + al_get_bitmap_width(Aviao2->bitmap) &&
                                                    especial[i].y >= Aviao2->y && especial[i].y <= Aviao2->y + al_get_bitmap_height(Aviao2->bitmap))
                                                {
                                                    Aviao2->hits++;
                                                    pontuacao += 5;
                                                    especial[i].active = false;
                                                }
                                        }
                                        }
                                        if(Aviao3->x<0){
                                            Aviao3->x = X_SCREEN + 100;
                                            Aviao3->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3->bitmap));
                                        }
                                        if (Aviao3->active)
                                        {
                                            if (Aviao3->x + al_get_bitmap_width(Aviao3->bitmap) > x &&
                                                Aviao3->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao3->y + al_get_bitmap_height(Aviao3->bitmap) > y &&
                                                Aviao3->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                Aviao3->x = X_SCREEN + 100;
                                                Aviao3->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3->bitmap));
                                            }
                                        }
                                        if(Aviao4->x<0){
                                            Aviao4->x = X_SCREEN + 100;
                                            Aviao4->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4->bitmap));
                                        }
                                        if (Aviao4->active)
                                        {
                                            if (Aviao4->x + al_get_bitmap_width(Aviao4->bitmap) > x &&
                                                Aviao4->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao4->y + al_get_bitmap_height(Aviao4->bitmap) > y &&
                                                Aviao4->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                Aviao4->x = X_SCREEN + 100;
                                                Aviao4->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4->bitmap));
                                            }
                                        }

                                        if (Aviao2->active)
                                        {
                                            if (Aviao2->x + al_get_bitmap_width(Aviao2->bitmap) > x &&
                                                Aviao2->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                                Aviao2->y + al_get_bitmap_height(Aviao2->bitmap) > y &&
                                                Aviao2->y < y + al_get_bitmap_height(sprite->bitmap))
                                            {

                                                sprite->hits++;
                                                pontuacao -= 2;
                                                printf("Aviao2 atingiu - linha 834");
                                                Aviao2->x = X_SCREEN + 100;
                                                Aviao2->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao2->bitmap));
                                            }
                                        }
                                        if (Aviao3->hits >= 4)
                                        {
                                            Aviao4->x = X_SCREEN + 100;
                                            Aviao4->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao3->bitmap));
                                            Aviao4->hits = 0;
                                            Aviao4->active = true;
                                        }

                                        if (Aviao4->hits >= 4)
                                        {
                                            Aviao3->x = X_SCREEN + 100;
                                            Aviao3->y = rand() % (Y_SCREEN - al_get_bitmap_height(Aviao4->bitmap));
                                            Aviao3->hits = 0;
                                            Aviao3->active = true;
                                        }

                                        if (Aviao2->hits >= 4)
                                        {
                                            Aviao2->active = false;
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
                                        if (Aviao3->active)
                                        {
                                            al_draw_bitmap(Aviao3->bitmap, Aviao3->x, Aviao3->y, 0);
                                        }
                                        if (Aviao4->active)
                                        {
                                            al_draw_bitmap(Aviao4->bitmap, Aviao4->x, Aviao4->y, 0);
                                        }
                                        if (Aviao2->active)
                                        {
                                            al_draw_bitmap(Aviao2->bitmap, Aviao2->x, Aviao2->y, 0);
                                        }

                                        if(segundos >= 7){
                                            al_draw_scaled_bitmap(
                                                    tiro_longo_map->bitmap,
                                                    0, 0,
                                                    al_get_bitmap_width(tiro_longo_map->bitmap),
                                                    al_get_bitmap_height(tiro_longo_map->bitmap),
                                                    tiro_longo_map->x, tiro_longo_map->y,
                                                    40, 40, 0);
                                                
                                                tiro_longo_map->x +=3;
                                        }

                        }else{ 
                   //* ativa o vilão ou desativa
                            if (vilao2->hits >= 30){
                                vilao2->active = false;
                                sprite->hits = 0;
                                final = 0;
                                segundos_total = segundos;
                                fase = 3;
                            }

                        if (vilao2->hits >= 20 && vilao2->hits < 28){
                                personagem=2;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "EU NÃO CAIREI");

                            }
                            if (vilao2->hits > 28){
                                personagem=2;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "NÃOOOOO!");

                            }
                            //! BOSS FINAL DA FASE
                            if(segundos > primeira_fase_comeco ){
                            final = 1;

                            //consertar os tiros horizontais
                            if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)) {
                                    vilao2->x = X_SCREEN -100;
                            
                            if(segundos >= primeira_fase_comeco && segundos < (primeira_fase_comeco+2)){   
                            personagem=2;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Parece que terei");
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "que fazer tudo sozinho!");
                            }
                            if(segundos < (primeira_fase_comeco+3) && segundos >= (primeira_fase_comeco+2)){   
                            personagem=2;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "Enfrente seu fim");
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "MEU FILHO!");
                            }
                            if(segundos <= (primeira_fase_comeco+4) && segundos > (primeira_fase_comeco+3)){   
                            personagem=1;
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -60, 0, "O QUE?");
                                al_draw_text(font2, al_map_rgb(1, 1, 1), X_SCREEN-480, Y_SCREEN -30, 0, "NÃOOOOOOOO!");
                            }

                            if(segundos == (primeira_fase_comeco+5)){
                                personagem =0;
                            }
                            
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
                                    vilao2->y -= ps;
                                    if (vilao2->y <= 60)
                                    {
                                        mov_v1 = 2;
                                    }
                                }
                                else
                                {
                                    vilao2->y += ps;
                                    if (vilao2->y + al_get_bitmap_height(vilao2->bitmap) >= Y_SCREEN)
                                    {
                                        mov_v1 = 1;
                                    }
                                }
                                al_draw_bitmap(vilao2->bitmap, vilao2->x, vilao2->y, 0);

                            } //? acaba aqui os primeiros 10 segundos do vilão

                            if (segundos == final_branco ){ 
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                                vilao2->y = Y_SCREEN / 2;
                                vilao2->x = X_SCREEN / 2;                                
                                ctnl_tiros_vertical_v2 =true;
                            }


                            if(segundos > final_branco3 && segundos <= final_vertical2){

                                al_draw_bitmap(vilao2->bitmap, vilao2->x, vilao2->y, 0);
                                ps = 30;
                                if((vilao2->y <=600 && mov_v1!=2 &&  mov_v1!=3)){
                                    vilao2->x -= ps+5; // Movimento na direção direita
                                    vilao2->y += ps; // Movimento na direção para baixo
                                    if((vilao2->y > 600)||(vilao2->x <= 0)){
                                        mov_v1 =2;
                                        ctnl_vilao2 =1;
                                    }
                                } 
                                if(mov_v1 == 2){
                                    vilao2->y-=ps;
                                    if(vilao2->y <= 50 && ctnl_vilao2 ==1){
                                        mov_v1=3;
                                    }
                                    if(vilao2->y <= 50 && ctnl_vilao2 ==0){
                                        mov_v1=1;
                                    }
                                }
                                if(mov_v1 ==3){
                                    vilao2->x += ps+5; 
                                    vilao2->y += ps; 
                                    if((vilao2->y > 600)||(vilao2->x >= X_SCREEN)){
                                        mov_v1 =2;
                                        ctnl_vilao2 =0;
                                    }
                                    
                                }

                            }


                            if (segundos == final_branco2 || segundos == final_vertical2){
                                al_clear_to_color(al_map_rgb(255, 255, 255));

                                vilao2->y = 50;
                                mov_v1=1;

                            }
                            if(segundos == final_branco3 ){  
                                al_clear_to_color(al_map_rgb(255, 255, 255));
                                vilao2->y = Y_SCREEN / 2;
                                vilao2->x = X_SCREEN / 2;;
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
                                            0, 0,
                                            al_get_bitmap_width(bullets_inimigos2->bitmap),
                                            al_get_bitmap_height(bullets_inimigos2->bitmap),
                                            bullets_inimigos2[r].x, bullets_inimigos2[r].y,         
                                            40,40, 
                                            0                         
                                        );                                     
                                    }
                                    //!funciona uma vez?
                                    if(bullets_inimigos[r].active){
                    
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
                                            tiros_v1_vertical[r].active = false; 
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
                                                40, 40, 0
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
                                al_draw_bitmap(vilao2->bitmap, vilao2->x, vilao2->y, 0);

                            }


                            if (milisegundos % 30 == 0){
                                if (vilao2->active){
                                    if (vilao2->x + al_get_bitmap_width(vilao2->bitmap) > x &&
                                        vilao2->x < x + al_get_bitmap_width(sprite->bitmap) &&
                                        vilao2->y + al_get_bitmap_height(vilao2->bitmap) > y &&
                                        vilao2->y < y + al_get_bitmap_height(sprite->bitmap)){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                    }
                                }
                            }

                            for (int i = 0; i < 5; i++){
                                if (bullets[i].active)
                                {
                                    if (bullets[i].x >= vilao2->x && bullets[i].x <= vilao2->x + al_get_bitmap_width(vilao2->bitmap) &&
                                        bullets[i].y >= vilao2->y && bullets[i].y <= vilao2->y + al_get_bitmap_height(vilao2->bitmap))
                                    {
                                        bullets[i].active = false; 
                                        vilao2->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (bullets[i].x < 0 || bullets[i].x > X_SCREEN || bullets[i].y < 0 || bullets[i].y > Y_SCREEN)
                                    {
                                        bullets[i].active = false;
                                    }
                                }
                                if (especial[i].active)
                                {
                                    if (especial[i].x >= vilao2->x && especial[i].x <= vilao2->x + al_get_bitmap_width(vilao2->bitmap) &&
                                        especial[i].y >= vilao2->y && especial[i].y <= vilao2->y + al_get_bitmap_height(vilao2->bitmap))
                                    {
                                        especial[i].active = false; 
                                        vilao2->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (especial[i].x < 0 || especial[i].x > X_SCREEN || especial[i].y < 0 || especial[i].y > Y_SCREEN)
                                    {
                                        especial[i].active = false;
                                    }
                                }
                                if (especial2[i].active)
                                {
                                    if (especial2[i].x >= vilao2->x && especial2[i].x <= vilao2->x + al_get_bitmap_width(vilao2->bitmap) &&
                                        especial2[i].y >= vilao2->y && especial2[i].y <= vilao2->y + al_get_bitmap_height(vilao2->bitmap))
                                    {
                                        especial2[i].active = false; 
                                        vilao2->hits++;             
                                        pontuacao += 3;            
                                    }
                                    if (especial2[i].x < 0 || especial2[i].x > X_SCREEN || especial2[i].y < 0 || especial2[i].y > Y_SCREEN)
                                    {
                                        especial2[i].active = false;
                                    }
                                }
                            } 
                        }
                    }

                    if(segundos >= 3){
                    
                        if(tiro_longo_map->active){    al_draw_scaled_bitmap(
                                tiro_longo_map->bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map->bitmap),
                                al_get_bitmap_height(tiro_longo_map->bitmap),
                                tiro_longo_map->x, tiro_longo_map->y,
                                60, 60, 0);
                            
                            tiro_longo_map->x -=3;
                        }
                    }
                    if (tiro_longo_map->active)
                    {
                        if (tiro_longo_map->x + al_get_bitmap_width(tiro_longo_map->bitmap) > x &&
                            tiro_longo_map->x < x + al_get_bitmap_width(sprite->bitmap) &&
                            tiro_longo_map->y + al_get_bitmap_height(tiro_longo_map->bitmap) > y &&
                            tiro_longo_map->y < y + al_get_bitmap_height(sprite->bitmap))
                        {
                            tiro_longo_map->active=false;
                            pontuacao +=5;
                            tiro_transv = 1;
                            tiro_longo =1;
                        }
                    }
                    if(segundos == 10){
                        tiro_longo_map->x = X_SCREEN +80;
                        tiro_longo_map->active = true;

                    }
                    if(segundos >= 3){
                    
                        if(tiro_longo_map->active){    al_draw_scaled_bitmap(
                                tiro_longo_map->bitmap,
                                0, 0,
                                al_get_bitmap_width(tiro_longo_map->bitmap),
                                al_get_bitmap_height(tiro_longo_map->bitmap),
                                tiro_longo_map->x, tiro_longo_map->y,
                                60, 60, 0);
                            
                            tiro_longo_map->x -=3;
                        }
                        if(tiro_longo_map->x < 0){
                            tiro_longo_map->active = false;
                        }
                    }

                    if (tiro_longo_map->active)
                    {
                        if (tiro_longo_map->x + al_get_bitmap_width(tiro_longo_map->bitmap) > x &&
                            tiro_longo_map->x < x + al_get_bitmap_width(sprite->bitmap) &&
                            tiro_longo_map->y + al_get_bitmap_height(tiro_longo_map->bitmap) > y &&
                            tiro_longo_map->y < y + al_get_bitmap_height(sprite->bitmap))
                        {
                            tiro_longo_map->active=false;
                            pontuacao +=5;
                            
                            tiro_transv =1;
                            tiro_longo =1;
                            ppower =4;
                        }
                    }


                }

                //! ----------------------------------- FIM SEGUNDA FASE ------------------------------




                for (int d = 0; d < 5; d++){
                
                                if (bullets_inimigos[d].active){
                                    float largura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = bullets_inimigos[d].x;
                                    float tiro_x_max = bullets_inimigos[d].x + largura_tiro;
                                    float tiro_y_min = bullets_inimigos[d].y;
                                    float tiro_y_max = bullets_inimigos[d].y + 40;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                            bullets_inimigos[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (bullets_inimigos2[d].active){
                                    float largura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = bullets_inimigos2[d].x;
                                    float tiro_x_max = bullets_inimigos2[d].x + largura_tiro;
                                    float tiro_y_min = bullets_inimigos2[d].y;
                                    float tiro_y_max = bullets_inimigos2[d].y + 40;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                            bullets_inimigos2[d].active = false; // Desativa o tiro
                                    }
                                }
                                if (tiros_v1[d].active){
                                    float largura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    // Retângulo do tiro
                                    float tiro_x_min = tiros_v1[d].x;
                                    float tiro_x_max = tiros_v1[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1[d].y;
                                    float tiro_y_max = tiros_v1[d].y + 40;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max){
                                            sprite->hits++;
                                            pontuacao -= 2;
                                            tiros_v1[d].active = false; 
                                    }
                                }
                                if (tiros_v1_2[d].active){
                                    float largura_tiro = 40;
                                    float largura_sprite = al_get_bitmap_width(sprite->bitmap);
                                    float altura_sprite = al_get_bitmap_height(sprite->bitmap);

                                    float tiro_x_min = tiros_v1_2[d].x;
                                    float tiro_x_max = tiros_v1_2[d].x + largura_tiro;
                                    float tiro_y_min = tiros_v1_2[d].y;
                                    float tiro_y_max = tiros_v1_2[d].y + 40;

                                    float sprite_x_min = x;
                                    float sprite_x_max = x + largura_sprite;
                                    float sprite_y_min = y;
                                    float sprite_y_max = y + altura_sprite;

                                    al_draw_rectangle(tiro_x_min, tiro_y_min, tiro_x_max, tiro_y_max,al_map_rgb(255, 0, 0), 2.0);

                                    if (tiro_x_max >= sprite_x_min && tiro_x_min <= sprite_x_max &&
                                        tiro_y_max >= sprite_y_min && tiro_y_min <= sprite_y_max)
                                    {

                                        sprite->hits++;
                                        pontuacao -= 2;
                                        tiros_v1_2[d].active = false; // Desativa o tiro
                                    }
                                }
                                //! <!-===================================================== consertar essa parte comentada +++++++++++++++++++++++++++++++++++++++++++- >
                }
                //! desenha os tiros do jogador

                for (int i = 0; i < 5; i++)
                {
                    if (bullets[i].active)
                    {
                        al_draw_filled_rectangle(bullets[i].x, bullets[i].y, bullets[i].x + 30, bullets[i].y + 15, al_map_rgb(255, 255, 0));
                    }
                    if (especial2[i].active)
                    {
                        al_draw_filled_rectangle(especial2[i].x, especial2[i].y, especial2[i].x + 30, especial2[i].y + 15, al_map_rgb(255, 255, 0));
                        
                    }
                    if (especial[i].active)
                    {
                        al_draw_filled_rectangle(especial[i].x, especial[i].y, especial[i].x + 30, especial[i].y + 15, al_map_rgb(255, 255, 0));
                    }
            
                }

                //! desenha as infos de tempo e pontuação na tela
                sprintf(tempo, "%02d:%02d", minutos, segundos);
                al_draw_text(font, al_map_rgb(255, 255, 255), 20, 10, 0, tempo);

                sprintf(pont, "Pontuação: %d", pontuacao);
                al_draw_text(font, al_map_rgb(255, 255, 255), 10, Y_SCREEN-60, 0, pont);


               //!  power scalling
                ALLEGRO_COLOR cor = al_map_rgb(0, 255, 0);

                if(ppower >=3){ cor = al_map_rgb(255, 0, 0);}
                for (int t = 0; t < ppower; t++) {
                    al_draw_filled_rectangle(20 + 60*t, Y_SCREEN-50, 50 + 60*t, Y_SCREEN-40, cor);

                }
                //! CONTROLA OS CORAÇÕES

                int coracoes_num = sprite->hits - 1;
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
                        if(Aviao1->active){
                            atiraInimigo(al_get_bitmap_width(Aviao1->bitmap), al_get_bitmap_height(Aviao1->bitmap), bullets_inimigos, Aviao1->x, Aviao1->y);
                        }
                        if(bomba->active){
                            atiraInimigo(al_get_bitmap_width(bomba->bitmap), al_get_bitmap_height(bomba->bitmap), bullets_inimigos2, bomba->x, bomba->y);
                        if(bomba->x == X_SCREEN-200 && bomba_mov < 90){
                            bomba_mov = 90;

                        }}
                    }

            if (final == 1) {
                if ((segundos < final_horizontal) || 
                    (segundos > final_branco2 && segundos <= final_horizontal2) || 
                    (segundos > final_vertical2)) {
                    if (segundos % 3 == 0) {
                        atiraInimigo(al_get_bitmap_width(vilao1->bitmap), al_get_bitmap_height(vilao1->bitmap), tiros_v1_2, vilao1->x, vilao1->y);
                        atiraInimigo(al_get_bitmap_width(vilao1->bitmap), al_get_bitmap_height(vilao1->bitmap), tiros_v1, vilao1->x, vilao1->y + 25);
                    } else {
                        atiraInimigo(al_get_bitmap_width(vilao1->bitmap), al_get_bitmap_height(vilao1->bitmap), tiros_v1, vilao1->x, vilao1->y + 25);
                    }
                }                        
            }

                }

                if(milisegundos %60 == 0 && fase == 1 && final ==1){
                    if ((segundos > final_branco && segundos < final_com_vertical) || (segundos > final_branco3 && segundos < final_vertical2)){
                            atiraInimigo_vertical(al_get_bitmap_width(vilao1->bitmap), al_get_bitmap_height(vilao1->bitmap), tiros_v1_vertical, vilao1->x, vilao1->y);
                    }
                }
                

                if (milisegundos %60 == 0 && fase == 2)
                {
                    if(final ==0){
                        if(Aviao2->active){
                            atiraInimigo(al_get_bitmap_width(Aviao2->bitmap), al_get_bitmap_height(Aviao2->bitmap), bullets_inimigos2, Aviao2->x, Aviao2->y+25);

                        }
                        if(Aviao3->active){
                            atiraInimigo(al_get_bitmap_width(Aviao3->bitmap), al_get_bitmap_height(Aviao3->bitmap), tiros_v1, Aviao3->x, Aviao3->y+25);

                        }
                        if(Aviao4->active){
                            atiraInimigo(al_get_bitmap_width(Aviao4->bitmap), al_get_bitmap_height(Aviao4->bitmap), tiros_v1_2, Aviao4->x, Aviao4->y+25);
                    }
                }
                if(final ==1){
                        if ((segundos >= primeira_fase_comeco && segundos <= final_horizontal)|| (segundos > final_branco2 && segundos <= final_horizontal2)||(segundos > final_vertical2)){
                        
                            atiraInimigo(al_get_bitmap_width(vilao2->bitmap), al_get_bitmap_height(vilao2->bitmap), bullets_inimigos2, vilao2->x, vilao2->y+25);
                        }
                        if ((segundos > final_branco && segundos <= final_com_vertical)){
                            atiraInimigo(al_get_bitmap_width(vilao2->bitmap), al_get_bitmap_height(vilao2->bitmap), tiros_v1, vilao2->x, vilao2->y+25);

                            atiraInimigo_verticalv2(al_get_bitmap_width(vilao2->bitmap), al_get_bitmap_height(vilao2->bitmap), tiros_v1_vertical, vilao2->x, vilao2->y);
                            atiraInimigo_verticalv2(al_get_bitmap_width(vilao2->bitmap), al_get_bitmap_height(vilao2->bitmap), bullets_inimigos, vilao2->x, vilao2->y);
                        }}

                }
                if (tiro_transv == 1) {

                    atira(sprite_width, sprite_height, especial2, x, y);
                    atira(sprite_width, sprite_height, especial, x, y);                

                    ctln_transversal++;
                    if (ctln_transversal == 10 || ctln_transversal == 60 || ctln_transversal == 120 || ctln_transversal == 180) {
                        ppower--;
                    }

                    if (ctln_transversal >= 180) { 
                        tiro_transv = 0;
                        ctln_transversal =0;
                    }
                }

                
                al_draw_bitmap(auxiliar1->bitmap, x_auxiliar1, y_auxiliar1,0);
            
                if(tiro_longo ==1){

                    atira(sprite_width, sprite_height, especial2, x_auxiliar1, y_auxiliar1);
                    atira(sprite_width, sprite_height, especial, x_auxiliar1, y_auxiliar1);                

                    x_auxiliar1+=5;
                    
                    if(x_auxiliar1 >=50){
                        ctln_longo++;
                        if (ctln_longo == 10 || ctln_longo == 60 || ctln_longo == 120 || ctln_longo == 180) {
                            
                        }

                        if (ctln_longo >= 180) { 
                            tiro_longo = 0;
                            ctln_longo =0;
                        }
                    }
                }
                
                al_flip_display();

                al_rest(0.01);

            }
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            sprite->hits =0;
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            if (event.keyboard.keycode == 13)
                ctln_transversal = 1;

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
            if (event.keyboard.keycode == 13)
            ctln_transversal =1;

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

        if (key_space){
            atira(sprite_width, sprite_height, bullets, x, y);
        
        }
    }

    
    for(int g =0; g<5; g++){
    al_destroy_bitmap(especial2[g].bitmap);
    al_destroy_bitmap(especial[g].bitmap);
    al_destroy_bitmap(tiros_v1[g].bitmap);
    al_destroy_bitmap(tiros_v1_2[g].bitmap);
    al_destroy_bitmap(bullets_inimigos2[g].bitmap);
    al_destroy_bitmap(bullets_inimigos[g].bitmap);
    al_destroy_bitmap(tiros_v1_vertical[g].bitmap);
    }
    al_destroy_bitmap(tiro_longo_map->bitmap);
    al_destroy_bitmap(zurc);
    al_destroy_bitmap(zurc_icon);
    al_destroy_bitmap(fundo);
    al_destroy_bitmap(cor1[0].bitmap);
    al_destroy_bitmap(cor1[1].bitmap);
    al_destroy_bitmap(cor1[2].bitmap);
    al_destroy_bitmap(vilao1->bitmap);
    al_destroy_bitmap(vilao2->bitmap);
    al_destroy_bitmap(sprite->bitmap);
    al_destroy_bitmap(bomba->bitmap);
    al_destroy_bitmap(Aviao1->bitmap);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(display);
    free(Aviao1);
    free(Aviao2);
    free(Aviao3);
    free(Aviao4);
    free(vilao1);
    free(bomba);
    free(bomba2);
    free(vilao2);
    free(sprite);
    free(auxiliar1);
    free(tiro_longo_map);
    return 0;
}
