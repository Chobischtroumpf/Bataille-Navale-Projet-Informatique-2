#pragma once

typedef enum {
  // Flags:
  IS_SHIP  = 0b001,
  IS_KNOWN = 0b010,  //< was a target
  IS_SUNK  = 0b100,

  // Non-ship types:
  WATER = 0,         //< water (my side) or unknown (assumed water, their side)
  OCEAN = IS_KNOWN,  //< was empty target

  // Ship states:
  UNDAMAGED = IS_SHIP,                       //< undamaged ship, used for my side
  HIT       = IS_SHIP | IS_KNOWN,            //< hit ship
  SUNK      = IS_SHIP | IS_KNOWN | IS_SUNK,  //< sunk ship
} CellType;