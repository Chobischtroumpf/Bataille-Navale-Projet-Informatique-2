#pragma once

enum class PlayerRole {
        Leader, // Player A
        Opponent, // Player B
        Spectator // Not directly interacting, but may need game state info
};