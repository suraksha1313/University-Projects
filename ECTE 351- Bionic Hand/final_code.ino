#include <Servo.h>

//creating servo object
Servo s1; //defining servo
Servo s2; //defining servo
Servo s3; //defining servo
Servo s4; //defining servo
Servo s5; //defining servo

int f1 = A0; //pin A0 to read analogue input thumb
int f2 = A1; //pin A1 to read analogue input middle
int f3 = A2; //pin A2 to read analogue input index
int f4 = A3; //pin A3 to read analogue input pinkey
int f5 = A4; //pin A4 to read analogue input ring
;
void setup()  
{
  Serial.begin(9600);
  s1.attach(2); //attaching servo to used pin 2
  s2.attach(3); //attaching servo to used pin 3
  s3.attach(4); //attaching servo to used pin 4
  s4.attach(5); //attaching servo to used pin 5
  s5.attach(6); //attaching servo to used pin 6
}


void loop()
{
  //two int variabled declared    //thumb
  int f1_position;  //stores values used for flex sensor reading
  int s1_position;  //stores values for the servo motor position
  f1_position = analogRead(f1); //reads the analog value from flex sensor connected to pin flex1and assigns it to flex1_position
  s1_position = map(f1_position, 730, 795, 0,180);  //maps the reading from flex1_position from range 840-900 to range 180-0
  s1_position = constrain(s1_position, 0, 180);  //ensures calue of flex1_position remains in range 0-180
  s1.write(s1_position);// position of servo motor matched on found specified range
 
  int f3_position;  // index
  int s3_position;  
  f3_position = analogRead(f3);
  s3_position = map(f3_position, 725, 795, 0, 180);
  s3_position = constrain(s3_position, 0, 180);  
  s3.write(s3_position);
 
  int f2_position;  //middle
  int s2_position;  
  f2_position = analogRead(f2);
  s2_position = map(f2_position, 690, 720, 0, 180);
  s2_position = constrain(s2_position, 0, 180);  
  s2.write(s2_position);
 
  int f5_position;  //ring
  int s5_position;  
  f5_position = analogRead(f5);
  s5_position = map(f5_position, 780, 884, 0, 180);
  s5_position = constrain(s5_position, 0, 180);  
  s5.write(s5_position);

 
  int f4_position;  // pinkey
  int s4_position;  
  f4_position = analogRead(f4);
  s4_position = map(f4_position, 735,778, 0, 180);
  s4_position = constrain(s4_position, 0, 180);  
  s4.write(s4_position);
 

 
// testing code on serial monitor
 
Serial.print(s1_position);
Serial.print("*");
Serial.print(s2_position);
Serial.print("*");
Serial.print(s3_position);
Serial.print("*");
Serial.print(s4_position);
Serial.print("*");
Serial.print(s5_position);
Serial.println("*");


Serial.print(f1_position);
Serial.print("--");
Serial.print(f2_position);
Serial.print("--");
Serial.print(f3_position);
Serial.print("--");
Serial.print(f4_position);
Serial.print("--");
Serial.print(f5_position);
Serial.println("--");

  delay(300);

  int y1 = analogRead(A0);
  int y2 = analogRead(A1);
  int y3 = analogRead(A2);
  int y4 = analogRead(A3);
  int y5 = analogRead(A4);

  Serial.print(y1);
  Serial.print(" "); 
  Serial.print(y2);
  Serial.print(" "); 
  Serial.print(y3);
  Serial.print(" "); 
  Serial.print(y4); 
  Serial.print(" "); 
  Serial.println(y5); 

  delay(100);
 
}
