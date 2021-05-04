
class Animation {
  PImage[] images;
  float speed;
  float scale;

  float index;
  boolean isAnimating;

  //constructor
  Animation(PImage [] i, float sp, float sc) {
    images = i;
    speed = sp;
    scale = sc;
    index = 0;
    isAnimating = false;
  }

  void display(int x, int y) {
    imageMode(CENTER);
    if (isAnimating) {
      int imageIndex = int(index);
      PImage img = images [imageIndex];
      image(img, x, y, img.width*scale, img.height*scale);

      next();
    } else {
      PImage img = images[0];
      image(img, x, y, img.width*scale, img.height*scale);
      ;
    }
  }

  void next() {
    //println(index);
    index += speed;
    if (index >= images.length) {
      index = 0;
      isAnimating = false;
    }
  }
}
