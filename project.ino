#include <LedControl.h>
#define NUM_DEVICES 4
#define push 2
#define buzzerPin 7
const float songSpeed = 1.0;

// Defining frequency of each music note
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988

// Music notes of the song, 0 is a rest/pulse
int notes[] = {
    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, 0,
    NOTE_A4, NOTE_G4, NOTE_A4, 0,

    NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_D5, NOTE_D5, 0,
    NOTE_D5, NOTE_E5, NOTE_F5, NOTE_F5, 0,
    NOTE_E5, NOTE_D5, NOTE_E5, NOTE_A4, 0,

    NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, 0,
    NOTE_D5, NOTE_E5, NOTE_A4, 0,
    NOTE_A4, NOTE_C5, NOTE_B4, NOTE_B4, 0,
    NOTE_C5, NOTE_A4, NOTE_B4, 0,

    NOTE_A4, NOTE_A4,

};
// Durations (in ms) of each music note of the song
// Quarter Note is 250 ms when songSpeed = 1.0
int durations[] = {
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 125,

    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 250, 125, 125,
    125, 125, 125, 250, 125,

    125, 125, 250, 125, 125,
    250, 125, 250, 125,
    125, 125, 250, 125, 125,
    125, 125, 375, 375,

    250, 125,

    
};
int DELAY_MS=400, size=4, blc_num=0, x=0;
int dataInPin = 11, clkPin = 13, loadPin = 3; //loadPin aka CS_PIN
bool game_on=true, sign =true;
volatile bool buttonPressed=0;
int stopping_Y=9, reference_Y=9, first_time=0, x_pos=0, score=0, high_score=0;
int game_level=0;
LedControl lc = LedControl(dataInPin, clkPin, loadPin, NUM_DEVICES);

void set_level(){
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
  game_level= i/4;
  else if(game_level<8)
  game_level++;
  else game_level=8;
  buttonPressed=false;
  DELAY_MS=450 - game_level * 50;
  clearAll();
  return;
 }
void buttonPressed_check(){
  buttonPressed = true;
 }
int runArray() {  // prints the moving array and returns lowest stopping coordinate on button_press
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
      buttonPressed = false;      
    }
  }
  return lowestY;
 }

void keepon(){          // Keeps required LEDs on based on stopping coordinate to array and its size
  for(int i=0;i<8; i++)
  {
    lc.setLed(blc_num, i, x, false);
  }

  for(int i = reference_Y; i < reference_Y + size; i++)
  {
   lc.setLed(blc_num, i, x, true);
  }  
  return;  
 }

void clearAll(){        // clears whole screen
  for(int i=0; i<4; i++)
  lc.clearDisplay(i);
 }

void playMusic(int musicType) { //plays buzzer
  int note;
  int duration;

  if (musicType == 0) { // Play game lost music
    note = 300;
    duration = 500;
  } else if (musicType == 1) { // Play game won music
    note = 600;
    duration = 1000;
  }else if(musicType ==3){
    const int totalNotes = sizeof(notes) / sizeof(int);
  // Loop through each note
  for (int i = 0; i < totalNotes; i++)
  {
    const int currentNote = notes[i];
    float wait = durations[i] / songSpeed;
    // Play tone if currentNote is not 0 frequency, otherwise pause (noTone)
    if (currentNote != 0)
    {
      tone(buzzerPin, notes[i], wait); // tone(pin, frequency, duration)
    }
    else
    {
      noTone(buzzerPin);
    }
    // delay is used to wait for tone to finish playing before moving to next loop
    delay(wait);
  }
  } else if(musicType==5){
    note = 450;
    duration =100;
  }
  else {
    return; // If musicType is not 0 or 1, return without playing music
  }

  tone(buzzerPin, note, duration); // Play the specified note for the specified duration
  delay(duration); // Wait for the note to finish playing
  noTone(buzzerPin); // Stop the buzzer
 }

void game(){
  playMusic(0);
  game_on=true;
  while(game_on) {
    coordinate_update();
    if(first_time==0){
     reference_Y = runArray();
     first_time=1;
     stopping_Y=reference_Y;
     keepon();
    }
   else {
     stopping_Y = runArray();
     check_alignemt();
     keepon();
    }
   delay(400);
   if(x_pos==31) {
     playMusic(1);
     reset();
    }
  }
  return;
 }
void coordinate_update(){// calculates block number and row number where the moving array is to be displayed
  x=7-(x_pos%8);
  blc_num=x_pos/8;
  x_pos++;
  buttonPressed=0;
  return ;
 }
void reset(){
   clearAll();
   score=x_pos*game_level -1;
   if(score>high_score)
   high_score=score;
   x_pos=0;
   first_time=0;
   reference_Y=9;
   stopping_Y=9;
   size=4;
   sign=0;
   game_on=false;
   playMusic(0); 
 }

void printStates(){
  // Serial.print("reference_Y: ");
  // Serial.print(reference_Y);
  // Serial.print("\tMinY_Next: ");
  // Serial.print(stopping_Y);
  // Serial.print("\tx_pos: ");
  // Serial.print(x_pos);
  // Serial.print("\tscore: ");
  // Serial.print(score);
  // Serial.print("\tHIGH_SCORE: ");
  // Serial.print(high_score);
  Serial.print("\tlevel: ");
  Serial.println(game_level);
  // Serial.print("\tsize: ");
  // Serial.println(size);
 }
void check_alignemt(){ // checks the stopping positions based on alignmrnt modifies size or resets
  if(blc_num==1 && size ==4)
  size = 3;
  if(blc_num==2 && size ==3)
  size = 2;
  if(blc_num==3 && size ==2)
  size = 1;
  if(abs(reference_Y - stopping_Y) < size){
    size=size - abs(reference_Y - stopping_Y);
    if(reference_Y < stopping_Y)
    reference_Y = stopping_Y;
    playMusic(5);
  }
  else reset();
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
 game();
 status_display();
 }

void status_display() {
   print_binary(3, high_score);
   print_binary(2, score);
   print_binary(1, game_level);
   playMusic(3);
   clearAll();
   return ;
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





