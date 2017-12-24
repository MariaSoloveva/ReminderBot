#include <signal.h>
#include <iostream>
#include <exception>

#include <tgbot/tgbot.h>
#include <thread>

#include "api.hpp"
#include "utils.hpp"


using namespace std;
using namespace TgBot;
using namespace Utils;

bool sigintGot = false;


int main()
{
    // в файле token.cfg должен лежать токен вашего бота
    // никогда не показывайте его никому
    // и не заливайте в github
    Bot bot(readToken("token.cfg"));
    initBot(bot);

	signal(SIGINT, [](int s) {
		printf("SIGINT got");
		sigintGot = true;
	});

 	try
    {

		TgLongPoll longPoll(bot);
		printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
		std::thread(loop, std::ref(bot)).detach();
		while (!sigintGot) 
        {
			printf("Long poll started\n");
			longPoll.start();
		}
	}
    catch (exception& e) 
    {
		printf("error: %s\n", e.what());
	}

	return 0;
}
