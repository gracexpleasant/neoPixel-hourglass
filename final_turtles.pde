
//case stuff 
int caseValue = 1;

//bacground stuff
Background b;
PImage startScreen;

//trashbin stuff
TrashBin tb;

//button things
Button instructions;
Button challenge;
Button levels;
Button easy;
Button medium;
Button hard;
Button home;

//turtle stuff
ArrayList<Turtles> turtleList;
int turtleStartTime;
int turtleInterval;
int numTurtles;
PImage[] turtleImages = new PImage[60];
Animation turtleAnimation;
int turtleSafe;
int turtleDead;
int totalTurtles;
int easyTurtles;
int mediumTurtles;
int hardTurtles;
int totalSafeTurtles;
int easySafe;
int mediumSafe;
int hardSafe;

//trash stuff
ArrayList<Trash> trashList;
int trashStartTime;
int trashInterval;
int numTrash;
PImage trashImage;
int totalTrash;
int easyTrash;
int mediumTrash;
int hardTrash;

//gull stuff 
ArrayList<Gulls> gullList;
int gullStartTime;
int gullInterval;
int numGulls;
PImage[] gullImages = new PImage[5];
Animation gullAnimation;
int totalGulls;
int easyGulls;
int mediumGulls;
int hardGulls;

//sound stuff
//import processing sound file
import processing.sound.*;
SoundFile dead;
SoundFile beachSound;
SoundFile safe;

void setup() {
  size(1200, 700);

  //background stuff
  startScreen = loadImage("background.png");

  //button stuff
  levels = new Button(970, 410, 150, 40, color(255));
  easy = new Button(width/2 - 75, 420, 150, 60, color(255));
  medium = new Button(width/2 - 75, 500, 150, 60, color(0));
  hard = new Button(width/2 - 75, 580, 150, 60, color(255));
  challenge = new Button(970, 465, 150, 40, color(255));
  instructions = new Button(970, 520, 150, 40, color(255));
  home = new Button(970, 600, 150, 40, color(0));

  //turtle timer
  turtleStartTime = millis();
  turtleInterval = 800;

  //trash timer
  trashStartTime = millis();
  trashInterval = 600;

  //trash stuff
  numTrash = 0;
  easyTrash = 5;
  mediumTrash = 10;
  hardTrash = 15;

  //turtle stuff
  turtleSafe = 0;
  turtleDead = 0;
  easyTurtles = 15;
  mediumTurtles = 25;
  hardTurtles = 40;
  numTurtles = 0;
  easySafe = 5;
  mediumSafe = 10;
  hardSafe = 20;

  //gull stuff
  numGulls = 0;
  easyGulls = 2;
  mediumGulls = 5;
  hardGulls = 7;

  turtleList = new ArrayList<Turtles>();
  trashList = new ArrayList<Trash>();  
  gullList = new ArrayList<Gulls>();
  b = new Background();
  tb = new TrashBin();

  //gull stuff
  for (int i=0; i<gullImages.length; i++) {
    gullImages[i] = loadImage("gulls"+i+".png");
  }
  gullAnimation = new Animation(gullImages, 0.2, 10);

  //turtle stuff
  for (int i=0; i<turtleImages.length; i++) {
    turtleImages[i] = loadImage("turtle"+i+".gif");
  }
  turtleAnimation = new Animation(turtleImages, 5, .3);

  trashImage = loadImage("trash.png");

  //sound stuff
  dead = new SoundFile(this, "dead.wav");
  dead.rate(2);
  dead.amp(.3);

  beachSound = new SoundFile(this, "beachSound.wav");
  beachSound.amp(0.08);
  
  safe = new SoundFile(this, "safe.wav");
  safe.amp(.3);
}

void draw() {
  switch(caseValue) {
  case 1:
    //beginning screen
    image(startScreen, 0, 0);
    textAlign(CENTER);
    textSize(40);
    fill(0, 0, 0);
    text("Welcome to", width/2.75, height - 200);
    text("levels", 1045, 440);
    text("challenge", 1045, 495);
    text("instructions", 1045, 550);
    textSize(100);
    fill(0, 0, 0);
    text("Save the Turtles", width/2.75, height - 100);

    if (levels.isPressed() == true) {
      caseValue = 4;
    }
    if (instructions.isPressed() == true) {
      caseValue = 3;
    }
    if (challenge.isPressed() == true) {
      caseValue = 2;
    }

    break;
  case 2:
    // the story/challenge screen
    image(startScreen, 0, 0);
    textAlign(LEFT);
    textSize(40);
    fill(0, 0, 0);
    text("I am glad you have decided to accept the", 20, 450);
    text("challenege of saving the turtles.", 20, 490);
    text("In this game you have to protect the sea ", 20, 530);
    text("turtles while they make their journey to the ", 20, 570);
    text("ocean.", 20, 610);

    if (home.isPressed() == true) {
      caseValue = 1;
    }

    textAlign(CENTER);
    textSize(40);
    fill(0, 0, 0);
    text("home", 1045, 630);

    break;
  case 3:
    //instructions screen
    image(startScreen, 0, 0);
    textAlign(LEFT);
    textSize(40);
    fill(0, 0, 0);
    text("To protect the baby turtles you have to", 20, 450);
    text("click the trash and move it to the trash can.", 20, 490);
    text("Unfortunately thats not all you have todo.", 20, 530);
    text("You also have to click on the sea gulls to", 20, 570);
    text("scare them away from the turtles.", 20, 610);

    if (home.isPressed() == true) {
      caseValue = 1;
    }

    textAlign(CENTER);
    textSize(40);
    fill(0, 0, 0);
    text("home", 1045, 630);

    break;
  case 4:
    //levels screen
    image(startScreen, 0, 0);

    textAlign(CENTER);
    textSize(60);
    fill(0, 0, 0);
    text("easy", width/2, 480);
    text("medium", width/2, 560);
    text("hard", width/2, 640);

    if (home.isPressed() == true) {
      caseValue = 1;
    }

    textAlign(CENTER);
    textSize(40);
    fill(0, 0, 0);
    text("home", 1045, 630);

    if (easy.isPressed() == true) {
      totalTurtles = easyTurtles;
      totalTrash = easyTrash;
      totalGulls = easyGulls;
      totalSafeTurtles = easySafe;
      caseValue = 5;
    }
    if (medium.isPressed() == true) {
      totalTurtles = mediumTurtles;
      totalTrash = mediumTrash;
      totalGulls = mediumGulls; 
      totalSafeTurtles = mediumSafe;
      caseValue = 5;
    }
    if (hard.isPressed() == true) {
      totalTurtles = hardTurtles;
      totalTrash = hardTrash;
      totalGulls = hardGulls;
      totalSafeTurtles = hardSafe;
      caseValue = 5;
    }
    break;
  case 5:
    //the game

    //background stuff
    imageMode(CORNER);
    image(startScreen, 0, 0);

    //score
    textAlign(RIGHT);
    textSize(40);
    fill(0, 0, 0);
    text("Turtles Safe:" + turtleSafe, 1050, 50);

    //trash bin stuff
    tb.render();

    //randomly generates turtles
    if (numTurtles < totalTurtles) {
      if (millis() - turtleStartTime>= turtleInterval) {
        turtleList.add(new Turtles(turtleAnimation));
        numTurtles += 1;
        turtleStartTime = millis();
      }
    }

    //turtle stuff
    for (Turtles t : turtleList) {
      t.render();
      t.move();
      t.safe(b);
      t.turtleBounds();
    }

    //goes through turtle list and trash list to kill turtles if they hit the trash
    for (Trash d : trashList) {
      for (Turtles t : turtleList) {
        t.trashKill(d);
      }
    }

    //makes turtles disappear when they hit water
    for (int i=turtleList.size()-1; i>=0; i--) {
      if (turtleList.get(i).isSafe == true) {
        turtleList.remove(i);
        turtleSafe += 1;
        if (safe.isPlaying()) {
          safe.stop();
        }
        safe.play();
      }
    }

    //makes turtle disappear if they hit the trash
    for (int i=turtleList.size()-1; i>=0; i--) {
      if (turtleList.get(i).isKilled == true) {
        turtleList.remove(i);
        turtleDead += 1;
        if (dead.isPlaying()) {
          dead.stop();
        }
        dead.play();
      }
    }

    //randomly generates trash 
    if (numTrash < totalTrash) {
      if (millis() - trashStartTime>= trashInterval) {
        trashList.add(new Trash(trashImage));
        numTrash += 1;
        trashStartTime = millis();
      }
    }

    //trash stuff
    for (Trash t : trashList) {
      t.render();
      t.isPressed();
      t.trashBounds();
      t. trashed(tb);
    }

    //removes the trash when it hits the trash bin
    for (int i=trashList.size()-1; i>=0; i--) {
      if (trashList.get(i).isTrashed == true) {
        trashList.remove(i);
      }
    }

    //gull stuff
    for (Gulls g : gullList) {
      g.render();
      g.gullBounds();
      g.move();
      g.isPressed();
    }

    //generates gulls randomly
    if (numGulls < totalGulls) {
      if (millis() - gullStartTime>= gullInterval) {
        gullList.add(new Gulls(gullAnimation));
        numGulls += 1;
        gullStartTime = millis();
      }
    }

    for (int i=gullList.size()-1; i>=0; i--) {
      if (gullList.get(i).gullGone == true) {
        gullList.remove(i);
      }
    }

    // kills turtles if the gulls hit them
    for (Gulls g : gullList) {
      for (Turtles t : turtleList) {
        t.gullKill(g);
      }
    }


    if (numTurtles >= totalTurtles && turtleList.size() <= 0) {
      if (turtleSafe >= totalSafeTurtles) {
        caseValue = 6;
      } else {
        caseValue = 7;
      }
    }

    if (!beachSound.isPlaying()) {
      beachSound.play();
    }
    break;
  case 6:
    //winner screen
    imageMode(CORNER);
    image(startScreen, 0, 0);
    textAlign(CENTER);
    textSize(50);
    fill(0, 0, 0);
    text("Congrats! You saved " + turtleSafe + " turtles", width/2, height-250);
    text("Press 'h' to go home or 'l' to try a different level", width/2, height - 200);
    break;
  case 7: 
    //loser screen 
    imageMode(CORNER);
    image(startScreen, 0, 0);
    textAlign(CENTER);
    textSize(50);
    fill(0, 0, 0);
    text("Oh no! You didnt save enough turtles.", width/2, height-250);
    text("Press 'h' to go home or 'l' to try a different level", width/2, height - 200);
  }
}

void keyPressed() {
  //goes to the home screen from the win or lose screen
  if (key == 'h') {
    caseValue = 1;
    setup();
  }
  //goes to the level screen from the win or lose screen
  if (key == 'l') {
    caseValue = 4;
    setup();
  }
}
