#include <Stepper.h>


#define STEPS 200 //200 steps/rotation
#define MAXSPEED 90 //"Base Speed"
#define INPERSTP 0.0075 //Inches Per step

#define JOG 50

#define LEFT 0
#define RIGHT 1

#define ARM 14.25

Stepper stpR(STEPS,2,3,4,5);
Stepper stpL(STEPS,6,7,8,9);

//Globals
int posStpR = 0, posStpL = 0; //Current step position of axis

//Protos
char GetCmd(void);
int Move_Absolute(float posR,float posL);
void Move_Relative(byte motor,int steps,byte spd);
void debug(void);
void GetXY(float &X, float &Y);
int Reverse_Kinematics(float X, float Y, float &Lft, float &Rht);

void setup()
{
  stpR.setSpeed(MAXSPEED);
  stpL.setSpeed(MAXSPEED);
  
  Serial.begin(9600);

  Serial.println("---My name is Sparks. I'm a Robot---");
  Serial.println("       ---Walker is my dad---\n\n");
}
void loop()
{
  char cmd = 0;
  float X,Y; // X/Y absolute position
  float Lft, Rt; // Lft/Rht linear absolute
  cmd = GetCmd();

  Serial.println(cmd);

  switch ( cmd ) {
  case 65: //A
    Move_Relative(LEFT,-JOG);
    break;
  case 66: //B
    Move_Relative(LEFT,JOG);
    break;
  case 67: //C 
    Move_Relative(RIGHT,-JOG);
    break;
  case 68: //D
    Move_Relative(RIGHT,JOG);
    break;
  case 69: //E
   debug();
    break;
  case 70: //F - In Progress
   GetXY(X,Y);
   Serial.print("X:");
   Serial.println(X);
   Serial.print("Y:");
   Serial.println(Y);
   
   if(Reverse_Kinematics(X,Y,Lft, Rt) == 0) //if we're OKAY
    Move_Absolute(Rt,Lft);
    break;
  case 71: //G
   posStpL = 10/INPERSTP;
   break;
  case 72: //H
   posStpR = 20/INPERSTP;
   break;
  default:
    // Code
    break;
}
  
}
char GetCmd()
{
  char cmd[1];

  Serial.println("----------CMD----------");
  Serial.println("A - Jog left motor left");
  Serial.println("B - Jog left motor right");
  Serial.println("C - Jog right motor left");
  Serial.println("D - Jog right motor right");
  Serial.println("E - Debug routine");
  Serial.println("F - Absolute Move");
  Serial.println("G - Datum Left - 10in");
  Serial.println("H - Datum Right - 20in");
  
  Serial.print("L/R Mot (Steps): ");
  Serial.print(posStpL);
  Serial.print("/");
  Serial.println(posStpR);

  Serial.print("L/R Mot (in): ");
  Serial.print(posStpL*INPERSTP);
  Serial.print("/");
  Serial.println(posStpR*INPERSTP);
  
  
  Serial.println("");
  
  while(!Serial.available()); //Wait here until cmd entered
                return(Serial.read());
  
}

//Return == 1 - Unreachable position
int Move_Absolute(float posR,float posL)
{
  int StepsR = 0, StepsL = 0;
  int RelR = 0, RelL = 0;

  StepsR = posR/INPERSTP; //Get it into steps
  StepsL = posL/INPERSTP;

  Serial.println(StepsR);
  Serial.println(StepsL);

  RelR = StepsR - posStpR;
  RelL = StepsL - posStpL;

  Serial.println("HERE");

  Serial.println(RelL);
  Serial.println(RelR);

  Move_Relative(LEFT,RelL);
  Move_Relative(RIGHT,RelR);
}

void Move_Relative(bool motor,int steps)
{
  if(motor == LEFT){
    stpL.step(steps);
    posStpL = posStpL + steps;
  }
  
  else{
    stpR.step(-1*steps);
    posStpR = posStpR + steps ;
  }
}

void debug(void)
{
  Move_Relative(LEFT,100);
  Move_Relative(RIGHT,100);
}

void GetXY(float &X,float &Y)
{
  String str;

  Serial.println("X?:");
  while(!Serial.available()); //Wait here until cmd entered
    {
        str = Serial.readStringUntil('\n');
        X = str.toFloat();
    }

  Serial.println("Y?:");
  while(!Serial.available()); //Wait here until cmd entered
    {
        str = Serial.readStringUntil('\n');
        Y = str.toFloat();
    }
  
}

//X,Y -> Real world X/Y abs. Position
//LftStp, RtStp -> Left / Right abs. linear position in steps 
int Reverse_Kinematics(float X, float Y, float &Lft, float &Rht)
{
  Lft = X - sqrt((ARM*ARM) - Y*Y);
  Rht = X + sqrt((ARM*ARM) - Y*Y);

  Serial.println("");
  Serial.print("Rev Kin - L/R (OKAY?! Y/N):");

  Serial.print(Lft);
  Serial.print(",");
  Serial.println(Rht);
  Serial.println("");

   while(!Serial.available()); //Wait here until cmd entered
    if(Serial.read() != 89)
      return(1);

  Serial.println("On it"); //Alright lets go
  return(0);
  
}



