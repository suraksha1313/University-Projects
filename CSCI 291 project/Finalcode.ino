//defnine which pin is connected to which component
#define speedPin_M1  5   
#define speedPin_M2  6   
#define directionPin_M1  4    
#define directionPin_M2  7 

#define R 8
#define MA 9
#define L 10
 
int rightSpeed = 50;
int leftSpeed = 50;
void setup()
{
//M1 RIGHT 
//M2 LEFT
Serial.begin(9600);
pinMode(R,INPUT); // Refering the pins as an input or output
pinMode(MA,INPUT);
pinMode(L,INPUT);

pinMode(speedPin_M1, OUTPUT); 
pinMode(speedPin_M2, OUTPUT);
pinMode(directionPin_M1, OUTPUT);
pinMode(directionPin_M2, OUTPUT);


}


void loop(){ 
int rv = digitalRead(R); //saving the outputs of the sensors to int values
int lv = digitalRead(L);
int cv = digitalRead(MA);

if(lv == 0 && cv == 1 && rv == 0) // if statement to go forward if only middle IR has detected a black line
{
  carF();
  
  
}
else if(lv == 0 && cv ==1 && rv == 1) //if statement to go forward if only right and middle IR has detected a black line
{
  carF();
   
}
else if(lv == 0 && cv ==0 && rv == 1) //if statement to go right if only right IR has detected a black line
{
  carR();
  
}
else if(lv==1 && cv==1 && rv == 0)//if statement to go left if left and middle IR has detected a black line
{
  carL();
  
}

else if(lv==1 && cv==1 && rv == 1)//if statement to go left if all IR has detected a black line
{
  carL();
  
}
else if(lv==0 && cv==0 && rv == 0)//if statement to take  u turn f no black lines detected.
{
  carL();
  delay(1);
  
}
}


void carStop() {                //  Motor Stop
  digitalWrite(speedPin_M2, 0);
  digitalWrite(directionPin_M1, LOW);
  digitalWrite(speedPin_M1, 0);
  digitalWrite(directionPin_M2, LOW);
}

void carF() {       //Move forward
  analogWrite (speedPin_M2, 50);             
  digitalWrite(directionPin_M1, LOW);
  analogWrite (speedPin_M1, 50);
  digitalWrite(directionPin_M2, LOW);
}

void carBack() {     //Move backward
  analogWrite (speedPin_M2, 50);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite (speedPin_M1, 50);
  digitalWrite(directionPin_M2, HIGH);
}

void carR() {  //Turn Right
  analogWrite (speedPin_M2, 50);
  digitalWrite(directionPin_M1, HIGH);
  analogWrite (speedPin_M1, 50);
  digitalWrite(directionPin_M2, LOW);
}



void carL() {         //Turn Left
    analogWrite (speedPin_M2, 50);
  digitalWrite(directionPin_M1, LOW);
  analogWrite (speedPin_M1, 50);
  digitalWrite(directionPin_M2, HIGH);

}
