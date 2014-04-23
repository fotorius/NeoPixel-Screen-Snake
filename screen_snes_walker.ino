#include <SnesController.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>


#define SCREEN_DATA_PIN  6
#define MATRIX_X         8
#define MATRIX_Y         8
#define BUTTONS          12
#define ON               10
#define OFF              0

#define SNES_CLK_PIN     4
#define SNES_STROBE_PIN  3
#define SNES_DATA_PIN    2

#define NORTH            0
#define SOUTH            1
#define EAST             2
#define WEST             3

#define RED             strip.Color(ON,OFF,OFF)
#define GREEN           strip.Color(OFF,ON,OFF)
#define BLUE            strip.Color(OFF,OFF,ON)
#define YELLOW          strip.Color(ON,ON,OFF)
#define MAGENTA         strip.Color(ON,OFF,ON)
#define CYAN            strip.Color(OFF,ON,ON)


Adafruit_NeoPixel strip = Adafruit_NeoPixel(MATRIX_X * MATRIX_Y, SCREEN_DATA_PIN, NEO_GRB + NEO_KHZ800);
SnesController controller = SnesController(SNES_CLK_PIN,SNES_STROBE_PIN,SNES_DATA_PIN);

uint8_t buttons[SNES_BUTTONS];
uint8_t screen[MATRIX_X][MATRIX_Y][3]= {0};
uint8_t dir = NORTH;
uint8_t dot_pos_x = 4;
uint8_t dot_pos_y = 4;
uint8_t food_x = 2;
uint8_t food_y = 2;
uint8_t cicles = 1;
uint8_t cicle = 0;
uint8_t pause = 0;


void setup(){
  //Serial.begin(9600);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  strip.setPixelColor(10, BLUE);
  strip.show();
  
  Timer1.initialize(100000);         // initialize timer1, and set a 1/2 second period
  Timer1.pwm(9, 512);                // setup pwm on pin 9, 50% duty cycle
  Timer1.attachInterrupt(t1_ISR);  // attaches callback() as a timer overflow interrupt
}

void t1_ISR(){
  controllerCommands();
  if(!cicle){
    act();
    cicle = cicles;
  }else{
    cicle--;
  }
}

void act(){
  if(!pause){
    screen[dot_pos_x][dot_pos_y][1] = OFF;
    walk();
    screen[dot_pos_x][dot_pos_y][1] = ON;
  }
  loadScreenData(strip);
}

void controllerCommands(){
  controller.SnesControllerRead(buttons);
  //printSnesController(buttons);
  if(buttons[SNES_L]){
    pause =0;
  }
  if(buttons[SNES_R]){
    pause =1;
  }
  setDirection();
}

void setDirection(){
  if(buttons[SNES_UP]){
    dir = NORTH;
  }
  if(buttons[SNES_DOWN]){
    dir = SOUTH;
  }
  if(buttons[SNES_RIGHT]){
    dir = EAST;
  }
  if(buttons[SNES_LEFT]){
    dir = WEST;
  }
}

boolean walk(){
  if(dir == NORTH){
    if(dot_pos_y >= MATRIX_Y - 1)
      return 0;
    dot_pos_y++;
  }
  if(dir == SOUTH){
    if(dot_pos_y == 0)
      return 0;
    dot_pos_y--;
  }
  if(dir == EAST){
    if(dot_pos_x >= MATRIX_X - 1)
      return 0;
    dot_pos_x++;
  }
  if(dir == WEST){
    if(dot_pos_x == 0)
      return 0;
    dot_pos_x--;
  }
}


void loadScreenData(Adafruit_NeoPixel strip){
  uint8_t i,j,k;
  uint16_t pixelPosition;
  for(i=0;i<MATRIX_X;i++){
    for(j=0;j<MATRIX_Y;j++){
      //matrixPixelPaint(strip,i,j,strip.Color(screen[i][j][0],screen[i][j][1],screen[i][j][2]));
      pixelPosition = (i * MATRIX_X)  + j;
      strip.setPixelColor(pixelPosition, strip.Color(screen[i][j][0],screen[i][j][1],screen[i][j][2]));
    }
  }
  strip.show();
}


void matrixPixelPaint(Adafruit_NeoPixel strip, uint16_t x, uint16_t y,uint32_t c){
  uint16_t pixelPosition;
  pixelPosition = (x * MATRIX_X)  + y;
  strip.setPixelColor(pixelPosition, c);
}

void printSnesController(uint8_t * data){
  int i;
  for(i=0;i<SNES_BUTTONS;i++){
    Serial.print( data[i] );
    Serial.print(" ");
  }
  Serial.println("");
}
void loop()
{
  // your program here...
}
