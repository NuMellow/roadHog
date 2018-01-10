#include <Servo.h>
#include <string.h>

Servo r_Servo;
Servo l_Servo;

int rightLedPin = 5;
int leftLedPin = 4;

const byte numChars = 32;
char receivedChars[numChars];
char tempChars[numChars]; //temporary array for use by strtok() function

int x = 0;
int y = 0;

boolean newData = false;

//int servo

void setup() {
  // put your setup code here, to run once:
  r_Servo.attach(7);
  l_Servo.attach(8);

  r_Servo.write(90);
  l_Servo.write(90);

  pinMode(rightLedPin, OUTPUT);
  pinMode(leftLedPin, OUTPUT);
  
  Serial.begin(9600);

  Serial.println("Enter in the format");

}

void blink(int led)
{
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}

void front()
{
  r_Servo.write(0);
  l_Servo.write(180);
}

void halt()
{
  r_Servo.write(90);
  l_Servo.write(90);
}

void right()
{
  int i = 0;
  blink(rightLedPin);
  while (i < 15000)
  {
    r_Servo.write(0);
    l_Servo.write(0);  
    i++;
  }
  
}

void left()
{
  int i = 0;
  blink(leftLedPin);
  while (i < 15000)
  {
    r_Servo.write(180);
    l_Servo.write(180);  
    i++;
  }
  
}

void reverse()
{
  r_Servo.write(180);
  l_Servo.write(0); 
}
void softRight()
{
  r_Servo.write(180);
  l_Servo.write(90);
}

void softLeft()
{
  int i = 0;
  while (i < 500)
  {
    r_Servo.write(0);
    l_Servo.write(90); 
    i++;
  }
  
}



void recvWithStartEndMarkers()
{
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData ==false)
  {
    rc = Serial.read();

    if (recvInProgress ==true)
    {
      if (rc != endMarker)
      {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars)
        {
          ndx = numChars - 1;
        }
      }
      else
      {
        receivedChars[ndx] = '\0'; //terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc ==startMarker)
    {
      recvInProgress = true;
    }
  }
}

void parseData()
{
  char * strtokIndx; //this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ","); //get the first coordinate
  x = atoi(strtokIndx); //convert this part to integer

//  strtokIndx = strtok(NULL, ","); //continues where the previous one left off
//  y = atoi(strtokIndx);  //convert to integer
}

void showParsedData()
{
  Serial.print("X: ");
  Serial.println(x);
//  Serial.print("Y: ");
//  Serial.println(y);
}

void loop() {
  // put your main code here, to run repeatedly:
  recvWithStartEndMarkers();
  
  if (newData == true)
  {
    strcpy(tempChars, receivedChars);
    
    //this temporary copy is necessary to protect the original data 
    //because strtok() replaces the commas with \0

    parseData();
    showParsedData();
    //moveTo(x,y);
    if (x == 0)
      halt();
    else if (x == 1)
      front();
    else if (x == 2)
    {
      left();
      front();
    }      
    else if (x ==3)
    {
      right();
      front();
    }
    else if(x == 4)
      reverse();
      
          
    newData = false; 
  }
  
  
}
