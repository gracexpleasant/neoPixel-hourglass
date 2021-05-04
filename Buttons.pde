
class Button {
  int buttonX;
  int buttonY;
  int buttonW;
  int buttonH;
  color buttonColor;

  //constructor
  Button(int x, int y, int w, int h, int butColor) {
    buttonX = x;
    buttonY = y;
    buttonW = w;
    buttonH = h;
    buttonColor = butColor;
  }
  
  void render(){ 
    fill(buttonColor);
    rect(buttonX, buttonY, buttonW, buttonH);
  }
  
    boolean isBetween (int mouseLocation, int firstBound, int secondBound) {
    if (mouseLocation >= firstBound && mouseLocation <= secondBound) {
      return true;
    } else {
      return false;
    }
  }

  boolean isInButton (int x, int y, int w, int h) {
    if (isBetween (mouseX, buttonX, buttonX + buttonW)) {
      if (isBetween (mouseY, buttonY, buttonY + buttonH)) {
        return true;
      }
    }
    return false;
  }

  boolean isPressed() {
    if (isInButton(buttonX, buttonY, buttonW, buttonH) == true && mousePressed == true) {
      return true;
    } else {
      return false;
    }
  }
}
