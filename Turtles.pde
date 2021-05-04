
class Turtles {
  int turtleX;
  int turtleY;
  int turtleW;
  int turtleH;
  color turtleColor;
  boolean isMoving;    //may not need this
  int turtleSpeed;
  boolean isSafe;
  boolean isKilled;
  float turtleTop;
  float turtleBottom;
  float turtleLeft;
  float turtleRight;
  float turtleMiddle;
  int turtlesSafe;
  Animation turtleAnimation;

  //constructor
  Turtles(Animation a) {
    turtleX = int(random(100, 200));
    turtleY = int(random(450, 650));
    turtleW = 50;
    turtleH = 25;
    turtleSpeed = 3;
    turtleColor = color(87, 196, 104);
    turtleTop = turtleY;
    turtleBottom = turtleY + turtleH;
    turtleLeft = turtleX;
    turtleRight = turtleX + turtleW;
    turtleMiddle = turtleX + turtleW/2;
    turtleAnimation = a;
    turtlesSafe = 0;
  }

  void render() {
    fill(turtleColor);
    //ellipse(turtleX, turtleY, turtleW, turtleH);
    turtleAnimation.display(turtleX, turtleY);
    turtleAnimation.isAnimating = true;
  }

  void move() {
    turtleX += turtleSpeed;
  }

  void turtleBounds() {
    turtleTop = turtleY;
    turtleBottom = turtleY + turtleH;
    turtleLeft = turtleX;
    turtleRight = turtleX + turtleW;
    turtleMiddle = turtleX + turtleW/2;
  }

  //detects if the turtles have made it off the screen (will be updated when I have water)
  void safe(Background ocean) {   
    if (turtleMiddle >= ocean.oceanLeft) {
      isSafe = true;
      println("safe");
    }
  }

  void trashKill(Trash debris) {
    if (debris.trashTop <= turtleBottom) {
      if (debris.trashBottom >= turtleTop) {
        if (debris.trashLeft <= turtleRight) {
          if (debris.trashRight >= turtleLeft) {
            isKilled = true;
            println("dead");
          }
        }
      }
    }
  }

  void gullKill(Gulls birdy) {
    if (birdy.gullTop <= turtleBottom) {
      if (birdy.gullBottom >= turtleTop) {
        if (birdy.gullLeft <= turtleRight) {
          if (birdy.gullRight >= turtleLeft) {
            isKilled = true;
            println("dead");
          }
        }
      }
    }
  }
}
