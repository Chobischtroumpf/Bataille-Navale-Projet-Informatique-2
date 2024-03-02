#include "ChatConsole.h"

int main() {
	std::string currentUserID = "myUserID"; // L'ID de l'utilisateur actuel(moi)
	std::string friendUsername = "friendUsername";
	ChatConsole chatConsole(currentUserID, friendUsername);
	chatConsole.run();

    return 0;
}
