const int  p =20;
int startTime;
int endTime;

int End = 0;
int irPin = 12; 
int val = 0; 

int var = 0;
int mode = 0;

int Sensor_read1=20;
int Sensor_read2=21 ; 

int Motor_A_Reverse = 3;
int Motor_A_Forward = 2;

int Motor_B_Reverse = 4;
int Motor_B_Forward = 5;


int trigPinR = 48;
int echoPinR = 46;
int trigPinM = 50;
int echoPinM = 52;
int trigPinL = 49;
int echoPinL = 47;

long distanceR;
long durationR;
long distanceL;
long durationL;
long distanceM;
long durationM;
 int counter;
 int dist;
 
////////////////
char path[100] = " ";
unsigned char pathLength = 0; // the length of the path
int pathIndex = 0; // used to reach an specific array element.
////////////////
unsigned int status = 0;

void setup() {
Serial.begin(9600);
 pinMode (Sensor_read1,INPUT);
 pinMode (Sensor_read2,INPUT);
 pinMode(Motor_A_Forward, OUTPUT);
 pinMode(Motor_A_Reverse, OUTPUT);
 pinMode(Motor_B_Forward, OUTPUT);
 pinMode(Motor_B_Reverse, OUTPUT);
 pinMode(trigPinR, OUTPUT);
 pinMode(echoPinR, INPUT);
 pinMode(trigPinL, OUTPUT);
 pinMode(echoPinL, INPUT);
 pinMode(trigPinM, OUTPUT);
 pinMode(echoPinM, INPUT);
 pinMode(irPin, INPUT);

int Sensor_read1=0;
int Sensor_read2=0;

 int counter=0;
 int dist=0; 
}


void loop() {
mazeSolve();
delay(30000);
mazeOptimization ();

 
}


void mazeSolve(void)
{    
    forward();
    while (!status)
    {  /*
        distanceR= 2;
        distanceM= 22;
        distanceL= 9;
        */
        Serial.println("hi");
        readLFSsensors();
        test_dest();
        Serial.println(distanceR);
        Serial.println(distanceL);
        Serial.println(distanceM);
        Serial.println(var);
        
          //read_sensors ();**
        
           switch(var){
              case 1:  
                forward();
                Serial.println("forward");
                break;
                
              case 2:  
                left();
                Serial.println("left");
                break;
                
              case 3:
                right();
                Serial.println("right");
                break;
                
             case 4:
                stop_();
                Serial.println("stop_");
                break;
                
             case 5:
                uTurn();
                Serial.println("uTurn");
                break;
                
               default: 
               // if unwanted charachter
               // nothing to do
               break;
              }
    }
}
void read_sensors (){
  if (Sensor_read1==1 & Sensor_read2 == 1 )
     counter++;
     dist=(15.7*(counter/20));
//Serial.println(dist);
  
}

void test_dest () {
  ultraL();
  ultraM();
  ultraR();

    if(distanceL > 20 && distanceR < 20 && distanceM < 20 ) //Left Only
    {
      var = 2;
      recIntersection('L');
    }
    else if (distanceL < 20 && distanceR > 20 && distanceM < 20 )//Right Only
    {
      var = 3;
      recIntersection('R');
    }
    else if (distanceL < 20 && distanceR < 20 && distanceM < 20)//Dead End
    {
      var = 5;
      recIntersection('B');
    }
    else if (distanceL < 20 && distanceR > 20 && distanceM > 20)//Straight or Right
    {
      var = 1;
      recIntersection('S');
    }
    else if (distanceL > 20 && distanceR < 20 && distanceM > 20)//Straight or Left
    {
      var = 2;
      recIntersection('L');
    }
    else if (distanceL > 20 && distanceR > 20 && distanceM < 20)//T Cross
    {
      var = 2;
      recIntersection('L');
    }
    else if (distanceL > 20 && distanceR > 20 && distanceM > 20)//X Cross
    {
      var = 2;
      recIntersection('L');
    }
    else if (distanceL < 20 && distanceR < 20 && distanceM > 20)//Straight
    {
      var = 1;
    }
  }

  void test_dest2 () {
  ultraL();
  ultraM();
  ultraR();

    if(distanceL > 20 && distanceR < 20 && distanceM > 20 ) //Straight
    {
      mode = 1;
    }
    else 
    {
      mode = 0;
    }
    
  }


void mazeOptimization (void)
{
  while (!status)
  {
    readLFSsensors();
    test_dest2 ();  
    switch (mode)
    {
      case 1:
        forward();
        break;    
      case 0:
        mazeTurn (path[pathIndex]); 
        pathIndex++;
        break;   
    }    
   }  
}

void mazeTurn (char dir) 
{
  switch(dir)
  {
    case 'L': // Turn Left
       left ();      
       break;   
    
    case 'R': // Turn Right
       right();     
       break;   
       
    case 'B': // Turn Back
       uTurn();     
       break;   
       
    case 'S': // Go Straight
       forward(); 
       break;
  }
}

void readLFSsensors(){

    val = digitalRead(irPin);     // read the input pin
    if (val == 0)
    {
      status = 1;
      End = 1;
      }
      else{
       //
      }
}
  
  
void recIntersection(char direction)
{
  path[pathLength] = direction; // Store the intersection in the path variable.
  pathLength ++;
  simplifyPath(); // Simplify the learned path.
}

void simplifyPath()
{
  // only simplify the path if the second-to-last turn was a 'B'
  if(pathLength < 3 || path[pathLength-2] != 'B')
    return;

  int totalAngle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[pathLength-i])
    {
      case 'R':
        totalAngle += 90;
  break;
      case 'L':
  totalAngle += 270;
  break;
      case 'B':
  totalAngle += 180;
  break;
    }
  }

  // Get the angle as a number between 0 and 360 degrees.
  totalAngle = totalAngle % 360;

  // Replace all of those turns with a single one.
  switch(totalAngle)
  {
    case 0:
  path[pathLength - 3] = 'S';
  break;
    case 90:
  path[pathLength - 3] = 'R';
  break;
    case 180:
  path[pathLength - 3] = 'B';
  break;
    case 270:
  path[pathLength - 3] = 'L';
  break;
  }

  // The path is now two steps shorter.
  pathLength -= 2;
  
} 


void forward (){
  
  digitalWrite(Motor_A_Reverse , LOW ) ;
  digitalWrite(Motor_A_Forward , HIGH ) ;
  digitalWrite(Motor_B_Reverse , LOW ) ;
  digitalWrite(Motor_B_Forward , HIGH ) ;

  }
 
void left () {
 
  digitalWrite(Motor_A_Reverse , LOW ) ;
  digitalWrite(Motor_A_Forward , LOW ) ;
  digitalWrite(Motor_B_Reverse , LOW ) ;
  digitalWrite(Motor_B_Forward , HIGH ) ;
  }
  
void right (){
 
  digitalWrite(Motor_A_Reverse , LOW ) ;
  digitalWrite(Motor_A_Forward , HIGH ) ;
  digitalWrite(Motor_B_Reverse , LOW ) ;
  digitalWrite(Motor_B_Forward , LOW ) ;
  }

void uTurn (){
 
  digitalWrite(Motor_A_Reverse , LOW ) ;
  digitalWrite(Motor_A_Forward , HIGH ) ;
  digitalWrite(Motor_B_Reverse , LOW ) ;
  digitalWrite(Motor_B_Forward , LOW ) ;
  }
  
  void stop_ (){
 
  digitalWrite(Motor_A_Reverse , LOW ) ;
  digitalWrite(Motor_A_Forward , LOW ) ;
  digitalWrite(Motor_B_Reverse , LOW ) ;
  digitalWrite(Motor_B_Forward , LOW ) ;
  }

  void ultraR(){
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  durationR = pulseIn(echoPinR, HIGH);
  distanceR = durationR*0.034/2;
  }

  void ultraL(){
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  durationL = pulseIn(echoPinL, HIGH);
  distanceL = durationL*0.034/2;
  }  
 
  void ultraM(){
  digitalWrite(trigPinM, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinM, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinM, LOW);
  durationM = pulseIn(echoPinM, HIGH);
  distanceM = durationM*0.034/2;
  }
