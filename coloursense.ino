#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3
#define COLOURS1 A2
#define COLOURS0 A1

int colour = 0;
int R = 0;
int G = 0;
int B = 0;

int buzzer = 8;

void setup() {
 Serial.begin(115200);
 pinMode(COLOUROUT,INPUT);
 pinMode(COLOURS1,OUTPUT);
 pinMode(COLOURS0,OUTPUT);
 pinMode(COLOURS2,OUTPUT);
 pinMode(COLOURS3,OUTPUT);
 digitalWrite(COLOURS1, HIGH);
 digitalWrite(COLOURS0, HIGH);
 digitalWrite(COLOURS2, HIGH);
 digitalWrite(COLOURS3, HIGH);
}
void loop() {
 colourRed();
 Serial.print("RED:");
 R = getIntensity();
 Serial.print(R);
 colourGreen();
 Serial.print("GREEN:");
 G = getIntensity();
 Serial.print(G);
 colourBlue();
 Serial.print("BLUE:");
 B = getIntensity();
 Serial.println(B);
 Serial.print("Colour detected: ");
 colour = detectColour(R, G, B);
 switch (colour) {
  case 1:
  Serial.print("RED");
  tone(buzzer, 200);
  delay(50);
  break;
  case 2:
  Serial.print("BLUE");
  tone(buzzer, 1500);
  delay(50);
  break;
  case 3:
  Serial.print("GREEN");
  tone(buzzer, 500);
  delay(50);
  break;
  case 4:
  Serial.print("YELLOW");
  tone(buzzer, 900);
  delay(50);
  break;
  case 5:
  Serial.print("BLACK");
  tone(buzzer, 0);
  delay(50);
  break;
 }
 Serial.println();
 delay(200);
}

void colourRed(){ //select red
 digitalWrite(COLOURS2,LOW);
 digitalWrite(COLOURS3,LOW);
}
void colourBlue(){ //select blue
 digitalWrite(COLOURS2,LOW);
 digitalWrite(COLOURS3,HIGH);
}
void colourWhite(){ //select white
 digitalWrite(COLOURS2,HIGH);
 digitalWrite(COLOURS3,LOW);
}
void colourGreen(){ //select green
 digitalWrite(COLOURS2,HIGH);
 digitalWrite(COLOURS3,HIGH);
}

int getIntensity(){ //measure intensity with oversampling
 int a=0;
 int b=255;
 for(int i=0;i<10;i++){a=a+pulseIn(COLOUROUT,LOW);}
 if(a>9){b=2550/a;}
 return b;
}

int detectColour(int R, int G, int B) {
  if(R>B & R>G & B>G) {
    colour = 1; //Red
  }
  if(R<B & G<B){
    colour = 2; // Blue
  }

  int RBdiff = abs(R-B);
  int RGdiff = abs(R-G);
  int BGdiff = abs(B-G);
  
  if(RBdiff <=5 & RGdiff <=5 & BGdiff<=5){
    colour = 3; // Green
  }
  if (R>G & G>B & R>=25 & G>=25) {
    colour = 4; //Yellow
  }
  if (R<10 & G<10) {
    colour = 5; //Black
  }
  return colour;
}