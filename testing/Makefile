all: chat menu

chat: test_chat.cc ChatController.cc ChatConsole.cc ChatView.cc
	@echo "Building chat..."
	@g++ test_chat.cc ChatController.cc ChatConsole.cc ChatView.cc -o chat

menu: test_menu.cpp menu.cpp
	@echo "Building menu..."
	@g++ test_menu.cpp menu.cpp -o menu

clean:
	@echo "Cleaning up..."
	@rm -f chat menu
