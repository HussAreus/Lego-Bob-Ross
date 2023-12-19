#include <Stepper.h>
#include <Servo.h>

// initialize the stepper library on pins 8 through 11:
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define joyX A0
#define joyY A1
#define SERVOX 3
#define SERVOY 5
#define SERVOZ 6

const int stepsPerRevolution = 2048;
//Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);
//Stepper stepperMotor = Stepper(stepsPerRevolution, IN1, IN3, IN2, IN4);
Servo servoX;
Servo servoY;
Servo servoZ;

class Vector3D{
  public:
    int x, y, z;
    Vector3D(int x, int y, int z){
      this.x = x;
      this.y = y;
      this.z = z;
    }

}

class Needle{
  public:
    Needle(int x=0, int y=0, int z=0){
      this.position = 
    }
  private:
    Vector3D position;
}

int degree = 0;
int stepsFromCenter = 0;

void setup() {
  Serial.begin(9600);
  //stepperMotor.setSpeed(15);
  servoX.attach(SERVOX);
  servoY.attach(SERVOY);
  servoZ.attach(SERVOZ);
}

void rotateLeft(){
  if(stepsFromCenter > -stepsPerRevolution/2){
    stepsFromCenter -= 10;
    stepperMotor.step(-10);
  }
}

void rotateRight(){
  if(stepsFromCenter < stepsPerRevolution/2){
    stepsFromCenter += 10;
    stepperMotor.step(10);
  }
}

void rotateUp(){
  degree += 5;
  if(degree > 10*180){
    degree = 10*180;
  }
  servoMotor.write(degree);
}

void rotateDown(){
  degree -= 5;
  if(degree < 0){
    degree = 0;
  }
  servoMotor.write(degree);
}


int count = 0;
int multiplier = 1;

void loop() {
  // step one revolution  in one direction:
  Serial.print(count);
  if(multiplier == 1){
    Serial.print("UP\n");
    rotateUp();
  }
  else{
    Serial.print("DOWN\n");
    rotateDown();
  }
  delay(100);

  if(count > 50){
    multiplier = -1;
  }
  else if (count <-50){
    multiplier = 1;
  }

  count += multiplier;


  // step one revolution in the other direction:
  //Serial.println("counterclockwise");
  //myStepper.step(-stepsPerRevolution);
  //delay(500);
}