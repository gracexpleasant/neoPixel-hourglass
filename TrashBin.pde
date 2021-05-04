class TrashBin {
  int trashBinX;
  int trashBinY;
  int trashBinW;
  int trashBinH;
  color trashBinColor;
  int trashBinTop;
  int trashBinBottom;
  int trashBinLeft;
  int trashBinRight;

  TrashBin() {
    trashBinX = 100;
    trashBinY = 350;
    trashBinW = 70;
    trashBinH = 100;
    trashBinColor = color (0, 0, 0);
    trashBinTop = trashBinY;
    trashBinBottom = trashBinY + trashBinH;
    trashBinLeft = trashBinX;
    trashBinRight = trashBinX + trashBinW;
  }

  void render() {
    fill(trashBinColor);
    rect(trashBinX, trashBinY, trashBinW, trashBinH);
  }

  void trashBinBounds() {
    trashBinTop = trashBinY;
    trashBinBottom = trashBinY + trashBinH;
    trashBinLeft = trashBinX;
    trashBinRight = trashBinX + trashBinW;
  }
}
