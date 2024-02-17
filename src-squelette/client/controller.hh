#pragma once

#include <string>

/*
 * Interface du Controller (ce qui permet d'envoyer des données)
 */
class Controller {
public:
  virtual void sendData(std::string data) {}
};