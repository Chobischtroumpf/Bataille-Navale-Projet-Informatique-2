#pragma Once

#include "connected_user.hh"

class PendingMatch {
public:
    void add_player(ConnectedUser player);
    void add_spectator(ConnectedUser specator);
    bool is_match_full();
private:
};