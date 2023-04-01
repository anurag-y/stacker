#include <LedControl.h>
#define NUM_DEVICES 4
#define push 2
#define buzzerPin 7

int DELAY_MS=400, size=4, blc_num=0, x=0;
int dataInPin = 11, clkPin = 13, loadPin = 3; //loadPin aka CS_PIN
bool game_on=true, sign =true;
volatile bool buttonPressed=0;
int minY_Next=9, minY1=9, flag=0, x_pos=0, score=0, high_score=0;
int level=0;
LedControl lc = LedControl(dataInPin, clkPin, loadPin, NUM_DEVICES);


void set_level(){
  unsigned long startTime=millis()/1000;
  unsigned long timeElapsed=0;
  int i=0;
    for(i; i<32 && !buttonPressed; i=i+4)
    {
      for(int j=1;j<7;j++){
        for(int k=0;k<3;k++)
         lc.setLed(i/8, j, 7-((i+k)%8), true);
      }
      delay(1000);
    }
  if(buttonPressed==true)
  level= i/4;
  else if(level<8)
  level++;
  else level=8;
  buttonPressed=false;
  DELAY_MS=450 - level * 50;
  clearAll();
  return;
 }
void buttonPressed_check(){
  buttonPressed = true;
}
int drawLine() {
  int y = random(8-size);
  int yStep = 1;
  bool down = true;
  bool stop = false;
  int lowestY = 0;
  
  while (!stop) {    
    // Draw the line
    for(int i=0;i<8;i++)
    {
      
      if(i>=y && i<y+size)
      lc.setLed(blc_num, i, x, true);
      else
      lc.setLed(blc_num, i, x, false);
    }
    
    // Update the x_pos-coordinate
    if (down) {
      y += yStep;
      if (y >= 7 -size+1) {
        y = 7 -size+1;
        down = false;
      }
    } else {
      y -= yStep;
      if (y <= 0) {
        y = 0;
        down = true;
      }
    }
    delay(DELAY_MS);
    
    // Check for button press to stop
    
    if (buttonPressed) {
      stop = true;
      lowestY = y;
      if(flag==1)
      {
        onCode(lowestY);
        keepon();
        delay(100);        
      }
      buttonPressed = false;
      
    }
  }
  return lowestY;
 }

void keepon(){
  for(int i=0;i<8; i++)
  {
    lc.setLed(blc_num, i, x, false);
  }

  for(int i=minY1;i<minY1+size; i++)
  {
   lc.setLed(blc_num, i, x, true);
  }    
 }

void clearAll(){
  for(int i=0; i<4; i++)
  lc.clearDisplay(i);
 }

void playMusic(int musicType) {
  int note;
  int duration;

  if (musicType == 0) { // Play game lost music
    note = 3000;
    duration = 500;
  } else if (musicType == 1) { // Play game won music
    note = 5000;
    duration = 1000;
  } else {
    return; // If musicType is not 0 or 1, return without playing music
  }

  tone(buzzerPin, note, duration); // Play the specified note for the specified duration
  delay(duration); // Wait for the note to finish playing
  noTone(buzzerPin); // Stop the buzzer
 }

void game2(){
  game_on=true;
  while(game_on) {
    coordinate_update();
    if(flag==0){
     minY1 = drawLine();
     flag=1;
     minY_Next=minY1;
     onCode(minY_Next);
     keepon();
     delay(200);
    }
   else {
     minY_Next = drawLine();
    }
   delay(200);
   x_pos++;

   if(x_pos==32) {
     x_pos=0;
     playMusic(1);
     reset();
     game_on=false;
    }
  }
  return;
 }
void coordinate_update(){
  x=7-(x_pos%8);
  blc_num=x_pos/8;
 }
void reset(){
   clearAll();
   score=x_pos*32 -1;
   if(score>high_score)
   high_score=score;
   x_pos=-1;
   flag=0;
   minY1=9;
   minY_Next=9;
   size=4;
   sign=0;
   playMusic(0); 
 }

void printStates(){
  // Serial.print("MinY1: ");
  // Serial.print(minY1);
  // Serial.print("\tMinY_Next: ");
  // Serial.print(minY_Next);
  // Serial.print("\tx_pos: ");
  // Serial.print(x_pos);
  // Serial.print("\tscore: ");
  // Serial.print(score);
  // Serial.print("\tHIGH_SCORE: ");
  // Serial.print(high_score);
  Serial.print("\tlevel: ");
  Serial.println(level);
  // Serial.print("\tsize: ");
  // Serial.println(size);
 }
void onCode(int currY){
  if(blc_num==1 && size ==4)
  size = 3;
  if(blc_num==2 && size ==3)
  size = 2;
  if(blc_num==3 && size ==2)
  size = 1;
  if(abs(minY1 - currY) < size){
    size=size - abs(minY1 - currY);
    if(minY1<currY)
    minY1=currY;
  }
  else{
    game_on=false;    
    reset(); 
  }
  return;
 }
void setup() {
  pinMode(push, INPUT);  
  for (int y = 0; y < NUM_DEVICES; y++) {// Initialize the MAX7219 chips and clear the screen
    lc.shutdown(y, false);
    lc.setIntensity(y, 8);
    lc.clearDisplay(y);
  }
  attachInterrupt(digitalPinToInterrupt(push), buttonPressed_check, RISING);
  Serial.begin(9600);
 }

void loop() {
 set_level();
 printStates();
 game2();
 status_display();
 }

void status_display() {
   print_binary(3, high_score);
   print_binary(2, score);
   print_binary(1, level);
   if(score==high_score)   {
     for(int j=1;j<7;j++){
        for(int k=1;k<7;k++)
         lc.setLed(0, j, k, true);
      }
   }
   delay(2000);
   clearAll();
 }

void print_binary(int div, int number){
  bool binaryArray[8];
  for (int i = 7; i >= 0; i--){
    if (number >= pow(2, i)) {
      binaryArray[7-i] = true;
      number -= pow(2, i);
    } else {
      binaryArray[7-i] = false;
    }
  }
  for(int i=0;i<8; i++){
    for(int j=1;j<7;j++){
      if(binaryArray[i])
      lc.setLed(div, 7-i ,j , true);
       else
      lc.setLed(div, 7-i ,j , false);
    }
  }
  
 }





