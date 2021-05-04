
class Background {
  int beachX;
  int beachY;
  int beachW;
  int beachH;
  color beachColor;
  int oceanX;
  int oceanY;
  int oceanW;
  int oceanH;
  color oceanColor;
  int oceanLeft;
  int skyX;
  int skyY;
  int skyW;
  int skyH;
  color skyColor;

  //constructor
  Background() {
    beachX = 0;
    beachY = 400;
    beachW = 950;
    beachH = 300;
    beachColor = color(240, 215, 126);
    oceanX = 900;
    oceanY = 400;
    oceanW = 300;
    oceanH = 300;
    oceanColor = color(126, 218, 240);
    oceanLeft = oceanX;
    skyX = 0;
    skyY = 0;
    skyW = 1200;
    skyH = 400;
    skyColor = color(29, 125, 219);
  }

  void beach() {
    fill(beachColor);
    rect(beachX, beachY, beachW, beachH);
  }

  void ocean() {
    fill(oceanColor);
    rect(oceanX, oceanY, oceanW, oceanH);
  }

  void sky() {
    fill(skyColor);
    rect(skyX, skyY, skyW, skyH);
  }
}
