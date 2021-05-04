
class Gulls {
  int gullX;
  int gullY;
  int gullW;
  int gullH;
  color gullColor;
  int gullSpeed;

  int gullTop;
  int gullBottom;
  int gullLeft;
  int gullRight;
  boolean gullGone;
  Animation gullAnimation;

  //constructor
  Gulls(Animation a) {
    gullX = int(random(100, 900));
    gullY = int(random(0, 200));
    gullW =  100;
    gullH = 40;
    gullColor = color(255, 0, 0);
    gullSpeed = 10;

    gullAnimation = a;

    gullTop = gullY;
    gullBottom = gullY + gullH;
    gullLeft = gullX;
    gullRight = gullX + gullW;

    gullGone = false;
  }

  void render() {
    fill(gullColor);
    //rect(gullX, gullY, gullW, gullH);
    gullAnimation.display(gullX+45, gullY+30);
    gullAnimation.isAnimating = true;
  }

  void gullBounds() {
    gullTop = gullY;
    gullBottom = gullY + gullH;
    gullLeft = gullX;
    gullRight = gullX + gullW;
  }

  void move() {
    gullY += gullSpeed;
    if (gullY >= 600) {
      gullSpeed =  -gullSpeed;
    } else if (gullY <= 20) {
      gullSpeed = -gullSpeed;
    }
  }

  //void isPressed(){
  //  if(gullTop <= mouseX){
  //    if(gullBottom >= mouseX){
  //      if (gullLeft <= mouseY){
  //        if(gullRight >= mouseY){
  //          gull

  //}

  boolean isBetween (float mouseLocation, float firstBound, float secondBound) {
    if (mouseLocation >= firstBound && mouseLocation <= secondBound) {
      return true;
    } else {
      return false;
    }
  }

  boolean isInButton (float x, float y, float w, float h) {
    if (isBetween (mouseX, x, x + w)) {
      if (isBetween (mouseY, y, y + h)) {
        return true;
      }
    }
    return false;
  }

  void isPressed() {
    if (isInButton(gullX, gullY, gullW, gullH) == true && mousePressed == true) {
      gullGone = true;
    } else {
      gullGone = false;
    }
    println(gullGone);
  }
}
