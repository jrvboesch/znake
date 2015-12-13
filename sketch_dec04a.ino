#include "VGA.h"
#include <string.h>
using namespace std;

 int windowx = VGA.getHSize();
 int windowy = VGA.getVSize();
 int direction = 0;
 int state = 0;
 int snake_size = 1;  
 int manzanitas_size = 1;  
 int input = 0;
 int time_step  = 350;

struct block{
  int posX;
  int posY;
  int active;
  int type;
  int width;
  int height;
  unsigned char *image;
};

//entities
struct block snake [100];
struct block manzanitas[10];

unsigned char pixel[]={
  WHITE,WHITE,
  WHITE,WHITE
};

unsigned char obstaculo[]={
  YELLOW,YELLOW,
  YELLOW,YELLOW
};

unsigned char manzana[]={
  RED,RED,
  RED,RED
};

void init_snake(){
  
  
      for(int i = 0; i < snake_size; i++)
      init_block(&snake[0],1,-99,-99,2,2,0);
  
  init_block(&snake[0],1,26,30,2,2);
  
  
}

void setup() {
  VGA.begin(VGAWISHBONESLOT(9),CHARMAPWISHBONESLOT(10));
}

void move_body(int pos,struct block dad){
    if(pos == snake_size)
      return;
  
  struct block temp;
  init_block( &temp,1,snake[pos].posX, snake[pos].posY,2,2 );
  init_block( &snake[pos] ,1, dad.posX,dad.posY, 2,2 );
  move_body( pos+1, temp);
}

void menu(){
  state = 0;
  VGA.clear();
  VGA.setColor(WHITE);   
  VGA.printtext((windowx/2)-40, (windowy/2), "Snake Game");
  VGA.printtext((windowx/2)-40, (windowy/2)+10, "press BTN 0");
  while(!digitalRead(FPGA_BTN_0)){
  
  }
  init_snake();
  init_manzanitas();
  generate_manzanitas();
}
void init_manzanitas(){
  for(int x = 0; x < manzanitas_size; x++){
  int ex = x*4+10;
  int yey = x*4+8;
  
    init_block(&manzanitas[x],2,ex,yey,2,2);
    manzanitas[x].active=0;
  }
}
void generate_manzanitas(){
   manzanitas[manzanitas_size].active=1;
    if(manzanitas_size < 10){
      manzanitas_size++;
    }
}

void move_snake(int dir){
    switch(dir){
      case 0:
        snake[0].posX+=2;
        VGA.printtext((windowx/2)-40, (windowy/2), "right");
        break;
      case 1:
        snake[0].posY+=2;
        VGA.printtext((windowx/2)-40, (windowy/2), "down");
        break;
      case 2:
        snake[0].posX-=2;
        VGA.printtext((windowx/2)-40, (windowy/2), "left");
        break;
      case 3: 
        snake[0].posY-=2;
        VGA.printtext((windowx/2)-40, (windowy/2), "up");
        break;
    }
    
    move_body(1,snake[0]);
    for(int i = 0; i < snake_size; i++)
     renderBlock(snake[i]);
    
    for(int i = 0; i < manzanitas_size; i++){
           renderBlock(manzanitas[i]);
           if(Collision(snake[0], manzanitas[i])  && manzanitas[i].active == 1 ){
              manzanitas[i].active = 0;
              snake_size+=2;
              generate_manzanitas();
          }
    }
      

}    


void renderBlock(block _block){
  if(_block.active)
     VGA.writeArea(_block.posX,_block.posY, _block.width, _block.height, _block.image);
}

void field_init(){
  
//    top
  for(int x= 4; x < 38*4;x=x+2)
    VGA.writeArea(x,0, 2, 2, pixel);

//    bot
  for(int x= 4; x < 38*4;x=x+2)
    VGA.writeArea(x,windowy-4, 2, 2, pixel);
    
//    right
  for(int x= 0; x < 29*4;x=x+2)
    VGA.writeArea(windowx-10,x, 2, 2, pixel);
    
//    left
  for(int x= 0; x < 29*4;x=x+2)
    VGA.writeArea(4,x, 2, 2, pixel);
}

void loop() {
    menu();
    while(state == 0){
    
    if(input == 0)
      manage_input();
    if(time_step-- == 0){
      input = 0;
      time_step = 500;
      VGA.clear();
      field_init();
      move_snake(direction);
    }

      delay(1);
      
    }
}


void manage_input(){
    input = 1;
    if(digitalRead(FPGA_BTN_1)){

    if(direction == 3){
      direction = 0;
    }else{
      direction++; 
    }
  }
  if(digitalRead(FPGA_BTN_2)){
    if(direction == 0){
      direction = 3;
    }else{
      direction--; 
    }
  }
  if(digitalRead(FPGA_BTN_3)){
   snake_size++;
  }
}

void init_block(block* _block, int type, int x, int y, int width, int height){
  _block->posX = x;
  _block->posY = y;
  switch(type){
    case 1 : _block->image = pixel; break;
    case 2 : _block->image = manzana; break;
    case 3 : _block->image = obstaculo; break;
    default : break;
  }
  _block->active = 1;
  _block->width = width;
  _block->height = height;
  _block->type = type;
  
}

int PointInRectangle(int x,int y,int x1,int y1,int x2,int y2){
        return ((( (x) >= (x1)) && ((y) >= (y1))) && (((x) <= (x2)) && ((y) <= (y2))));
}
 
int Collision(block s1, block s2)
{
        return (PointInRectangle(s1.posX,            s1.posY,               s2.posX, s2.posY,    s2.posX + s2.width,             s2.posY + s2.height)  ||
                PointInRectangle(s1.posX + s1.width, s1.posY + s1.height,   s2.posX, s2.posY,    s2.posX + s2.width,             s2.posY + s2.height)  ||
                PointInRectangle(s1.posX,            s1.posY + s1.height,   s2.posX, s2.posY,    s2.posX + s2.width,             s2.posY +  s2.height) ||
                PointInRectangle(s1.posX + s1.width, s1.posY,               s2.posX, s2.posY,    s2.posX + s2.width,             s2.posY +  s2.height));
}


void init_block(block* _block, int type, int x, int y, int width, int height, int active){
  _block->posX = x;
  _block->posY = y;
  switch(type){
    case 1 : _block->image = pixel; break;
    case 2 : _block->image = manzana; break;
    case 3 : _block->image = obstaculo; break;
    default : break;
  }
  _block->active = active;
  _block->width = width;
  _block->height = height;
  _block->type = type;
  
}
