#pragma once

/*
 * Types possibles des cases du jeu
 */
enum CellType {
  // Flags:
  IS_SHIP = 0b000001,
  IS_MINE = 0b000010,
  IS_KNOWN = 0b000100,
  IS_HIT = 0b001000,
  IS_SUNK = 0b010000,

  // Non-ship types:
  WATER = 0,        //< water (my side) or unknown (assumed water, their side)
  OCEAN = IS_KNOWN, //< was empty target

  // Mine:
  UNDAMAGED_MINE = IS_MINE,
  SCANNED_MINE = IS_MINE | IS_KNOWN,
  HIT_MINE = IS_MINE | IS_KNOWN | IS_HIT,

  // Ship states:
  UNDAMAGED_SHIP = IS_SHIP,
  SCANNED_SHIP = IS_SHIP | IS_KNOWN,
  HIT_SHIP = IS_SHIP | IS_KNOWN | IS_HIT,
  SUNK_SHIP = IS_SHIP | IS_KNOWN | IS_HIT | IS_SUNK,
};


/*
is_ship
is_mine
is_kwown
is_hit
is_sunk

/plateau adverse/           - /plateau joueur/
eau non decouverte          - eau                     - 0
eau decouverte ou scanné  ╳ - eau scanné ou touché  ╳ - is_knwon

None                        - case mine             ¤ - is_mine
case mine scanné          █ - case mine             ¤ - is_mine | is_kwown
case mine touché          ¤ - case mine touché      * - is_mine | is_kwown | is_hit

None                        - case bateau           █ - is_ship
case bateau scanné        █ - case bateau scanné    █ - is_ship | is_kwown
case bateau touché        ▒ - case bateau touché    ▒ - is_ship | is_kwown | is_hit
case bateau coulé         ░ - case bateau coulé     ░ - is_ship | is_kwown | is_hit | is_sunk
*/