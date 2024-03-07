#pragma once

/*
 * Types possibles des cases du jeu
 */
enum CellType {
  // Flags:
  IS_SHIP = 0b001,
  IS_KNOWN = 0b010, //< was a target
  IS_SUNK = 0b100,

  // Non-ship types:
  WATER = 0,        //< water (my side) or unknown (assumed water, their side)
  OCEAN = IS_KNOWN, //< was empty target

  // Ship states:
  UNDAMAGED = IS_SHIP,                 //< undamaged ship, used for my side
  HIT = IS_SHIP | IS_KNOWN,            //< hit ship
  SUNK = IS_SHIP | IS_KNOWN | IS_SUNK //< sunk ship

};

/*
is_ship
is_mine
is_kwown
is_hit
is_sunk

eau non decouverte             - eau                   0
eau decouverte (ou scanné)   ╳ - eau scanné ou touché  ╳ - is_hit

                               -   ¤             - is_mine
case mine inconnu (scanné)   █ -   ¤             - is_mine | is_kwown
case mine connu (touché)     ¤ -   * is_mine | is_hit

                               -   █             - is_ship
case bateau inconnu (scanné) █ -   █ is_ship | is_kwown
case bateau touché           ▒ -   ▒ is_ship | is_hit
case bateau coulé            ░ -   ░ is_ship | is_hit | is_sunk
*/