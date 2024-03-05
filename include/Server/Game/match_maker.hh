#pragma once

#include <vector>

#include "pending_match.hh"

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