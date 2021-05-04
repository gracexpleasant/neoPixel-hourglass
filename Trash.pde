
class Trash {
  float trashX;
  float trashY;
  int trashW;
  int trashH;
  color trashColor;
  float trashTop;
  float trashBottom;
  float trashLeft;
  float trashRight;
  boolean isHit;
  boolean isPicked;
  boolean isTrashed;
  PImage trash;

  //constructor
  Trash(PImage p) {
    trash = p;
    trashX = random(300, 800);
    trashY = random(450, 650);
    trashW = 30;
    trashH = 80;
    trashColor = color(255, 0, 0);
    trashTop = trashY;
    trashBottom = trashY + trashH;
    trashLeft = trashX;
    trashRight = trashX + trashW;
    isHit = false;
    isPicked = false;
    isTrashed = false;
  }


  void render() {
    fill(trashColor);
    //rect(trashX, trashY, trashW, trashH);
    imageMode(CENTER);
    image(trash, trashX+15, trashY+35, 100, 100);
  }

  void trashBounds() {
    trashTop = trashY;
    trashBottom = trashY + trashH;
    trashLeft = trashX;
    trashRight = trashX + trashW;
  }

  void trashed(TrashBin tb) {
    if (tb.trashBinTop <= trashBottom) {
      if (tb.trashBinBottom >= trashTop) {
        if (tb.trashBinLeft <= trashRight) {
          if (tb.trashBinRight >= trashLeft) {
            isTrashed = true;
            println("trashed");
          }
        }
      }
    }
  }


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
    if (isInButton(trashX, trashY, trashW, trashH) == true && mousePressed == true) {
      isPicked = true;
    }

    if (isPicked) {
      trashX = mouseX - trashW/2;
      trashY = mouseY - trashH/2;
    }
  }
}
