#pragma once

#include "pending_match.hh"
#include <vector>

/*
 * Classe qui créer et manage les parties en cours de creation
 */
class MatchMaker {
public:
  void getNewPlayer();
  std::vector<PendingMatch> getPendingMatches();
  void addPendingMatch(PendingMatch newPendingMatch);

private:
  std::vector<PendingMatch> PendingMatches;
};