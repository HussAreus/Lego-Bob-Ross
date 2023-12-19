#include <Stepper.h>
#include <Servo.h>
#include <Vector.h>
#include <math.h>

#define SERVOX 3
#define SERVOY 5
#define SERVOZ 6
#define START_STOP_BTN 2
#define LOAD_BTN 4

Servo servoX;
Servo servoY;
Servo servoZ;

enum SIGNAL{
  DRAW,
  WAIT,
  STOP,
  LOAD
};

SIGNAL state = WAIT;

class Vector2D{
  public:
    float x, y;
    Vector2D(float x, float y){
      this->x = x;
      this->y = y;
    }
};

class Needle{
  public:
    // Up/Down delay
    static const int delayZ = 50;
    // X/Y delay
    static const int delayXY = 15;

    Needle(float x=0, float y=0){
      this->position = new Vector2D(x, y);
      raise();
    }

    void reset(){
      move(0, 0);
      raise();
    }

    void move(float x, float y){
      float deltax = x - this->position->x;
      float deltay = y - this->position->y;
      this->position->x = x;
      this->position->y = y;
      servoX.write(round(this->position->x));
      servoY.write(round(this->position->y));
      delay((deltax + deltay)/2 * delayXY);
      Serial.print("Moved to (");
      Serial.print(this->position->x);
      Serial.print(", ");
      Serial.print(this->position->y);
      Serial.println(")");
    }

    void raise(){
      servoZ.write(0);
      delay(delayZ);
    }

    void lower(){
      servoZ.write(180);
      delay(delayZ);
    }

    void draw(Vector<Vector2D*> points){
      move(points[0]->x, points[0]->y);
      lower();
      for(int i = 1; i < points.size(); i++){
        if(state == STOP){
          raise();
          state = WAIT;
        }
        while(state == WAIT){
          delay(100);
        }

        if(state != DRAW){
          break;
        }

        move(points[i]->x, points[i]->y);
      }
      raise();
    }

  private:
    Vector2D* position;
};

class Shape{
  public:
    Shape(Vector<Vector2D*> points, int scale = 1){
      this->points = points;
      rescale(scale);
    }

    void print(){
      for(int i = 0; i < points.size(); i++){
        Serial.print("(");
        Serial.print(points[i]->x);
        Serial.print(", ");
        Serial.print(points[i]->y);
        Serial.print(") ");
      }
      Serial.println();
    }

    void rescale(float scale){
      for(int i = 0; i < points.size(); i++){
        points[i]->x *= scale;
        points[i]->y *= scale;
      }
    }

    Vector2D* upperLeft(){
      float minX = points[0]->x;
      float minY = points[0]->y;
      for(int i = 1; i < points.size(); i++){
        if(points[i]->x < minX){
          minX = points[i]->x;
        }
        if(points[i]->y < minY){
          minY = points[i]->y;
        }
      }
      return new Vector2D(minX, minY);
    }

    Vector2D* lowerRight(){
      float maxX = points[0]->x;
      float maxY = points[0]->y;
      for(int i = 1; i < points.size(); i++){
        if(points[i]->x > maxX){
          maxX = points[i]->x;
        }
        if(points[i]->y > maxY){
          maxY = points[i]->y;
        }
      }
      return new Vector2D(maxX, maxY);
    }

    Vector<Vector2D*> points;
};

void rescaleImage(Vector<Shape*> shapes){
  Vector2D* upperLeft = shapes[0]->upperLeft();
  Vector2D* lowerRight = shapes[0]->lowerRight();
  for(int i = 1; i < shapes.size(); i++){
    Vector2D* shapeUpperLeft = shapes[i]->upperLeft();
    Vector2D* shapeLowerRight = shapes[i]->lowerRight();
    if(shapeUpperLeft->x < upperLeft->x){
      upperLeft->x = shapeUpperLeft->x;
    }
    if(shapeUpperLeft->y < upperLeft->y){
      upperLeft->y = shapeUpperLeft->y;
    }
    if(shapeLowerRight->x > lowerRight->x){
      lowerRight->x = shapeLowerRight->x;
    }
    if(shapeLowerRight->y > lowerRight->y){
      lowerRight->y = shapeLowerRight->y;
    }
  }
  if(lowerRight->x - upperLeft->x == 0 || lowerRight->y - upperLeft->y == 0){
    Serial.println("No Shapes Found");
    return;
  }
  float xScale = 180/(lowerRight->x - upperLeft->x);
  float yScale = 180/(lowerRight->y - upperLeft->y);
  float scale = xScale;
  if(yScale < xScale){
    scale = yScale;
  }
  for(int i = 0; i < shapes.size(); i++){
    shapes[i]->rescale(scale);
  }
}

void startStop(){
  if(state == WAIT){
    Serial.println("START");
    state = DRAW;
  }else if(state == DRAW){
    Serial.println("STOP");
    state = STOP;
  }else if(state == WAIT){
    Serial.println("START");
    state = DRAW;
  }
}

void load(){
  if(state == WAIT){
    Serial.println("LOAD");
    state = LOAD;
  }
  else if(state == DRAW){
    Serial.println("STOP");
    state = STOP;
  }
}

void setup() {
  Serial.begin(19200);
  Serial.println("Started Logging");
  servoX.attach(SERVOX);
  servoY.attach(SERVOY);
  servoZ.attach(SERVOZ);


  // Two interupts for the two buttons: one for start/stop, one for load
  attachInterrupt(digitalPinToInterrupt(START_STOP_BTN), startStop, RISING);
  attachInterrupt(digitalPinToInterrupt(LOAD_BTN), load, RISING);
}

Vector<Shape*> loadImages(){
  Vector<Shape*> shapes;
  Vector<Vector2D*> points;
  points.push_back(new Vector2D(0, 0));
  points.push_back(new Vector2D(0, 1));
  points.push_back(new Vector2D(1, 1));
  points.push_back(new Vector2D(1, 0));
  shapes.push_back(new Shape(points));
  return shapes;
}

Needle* needle = new Needle();

void loop() {
  Serial.print("L");
  switch (state)
  {
  case LOAD: case DRAW:
  {
    Vector<Shape*> shapes = loadImages();
    rescaleImage(shapes);
    for(int i = 0; i < shapes.size(); i++){
      needle->draw(shapes[i]->points);
    }
    state = WAIT;
    break;
  } 
  case WAIT:
    needle->reset();
  }
}