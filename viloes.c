#include "viloes.h"

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
        if (!bullets[i].active) {  
            bullets[i].x = x + sprite_width / 2 - 2; 
            bullets[i].y = 60;     
            bullets[i].active = true;
            break;                   
        }
    }
}
void atiraInimigo_verticalv2(int sprite_width, int sprite_height, Bullet bullets[], int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        if (!bullets[i].active) {  
            bullets[i].x = x + sprite_width / 2 - 2;
            bullets[i].y = y ;        
            bullets[i].active = true; 
            break;                    
        }
    }
}
