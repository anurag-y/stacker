#define NUM_DEVICES 4
#define push 7
int DELAY_MS=250;
int dataInPin = 11;
int clkPin = 13;
int loadPin = 3; // marked as CS_PIN on MAX 7219
int size=4;
int blc_num;
int x;
bool game_on=true;
bool sign=0;
#include <LedControl.h>
LedControl lc = LedControl(dataInPin, clkPin, loadPin, NUM_DEVICES);
int x_pos=0;
int minY_Next=9, minY1=9, flag=0;

int drawLine() {
  int y = random(6);
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
    if (digitalRead(push) == HIGH) {
      stop = true;
      lowestY = y;
      if(flag==1)
      {
        onCode(lowestY);
        keepon();
      }
      
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
  int buzzerPin = 2;
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
    }
   else {
     minY_Next = drawLine();
    } 
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
   x_pos=-1;
   flag=0;
   minY1=9;
   minY_Next=9;
   size=4;
   sign=0;
   playMusic(0); 
 }

void printStates(){
  Serial.print("MinY1: ");
  Serial.print(minY1);
  Serial.print("\tMinY_Next: ");
  Serial.print(minY_Next);
  Serial.print("\tx_pos: ");
  Serial.print(x_pos);
  Serial.print("\tsize: ");
  Serial.println(size);

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
  Serial.begin(9600);
 }

void loop() {
  printStates();
  game2();
 // status_display();
  print_binary(123);
  Serial.println("Out of game2");
 }

// void status_display()
//  {
//    print_binary();
//  }

void print_binary(int number)
{
  bool binaryArray[8];
  for (int i = 7; i >= 0; i--){
    if (number >= pow(2, i)) {
      binaryArray[7-i] = true;
      number -= pow(2, i);
    } else {
      binaryArray[7-i] = false;
    }
  }
  Serial.print("Binary Array: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(binaryArray[i]);
  }
}





