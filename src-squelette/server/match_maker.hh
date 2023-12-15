#pragma once

#include <vector>
#include "pending_match.hh"

class MatchMaker {
public:
  void getNewPlayer();
  std::vector<PendingMatch> getPendingMatches();
  void addPendingMatch(PendingMatch newPendingMatch);

private:
  std::vector<PendingMatch> PendingMatches;
};