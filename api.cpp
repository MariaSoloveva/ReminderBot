#include "api.hpp"
#include <thread>

using namespace TgBot;


std::string readToken(const std::string& filename)
{
    std::ifstream telegramTokenFile(filename);
    std::string token;
    telegramTokenFile >> token;
    return token;
}
void loop(Bot& bot)
{
    struct tm* u;
    const time_t timer = time(NULL);
    u = localtime(&timer);
    while(true)
    {
        for(auto &g : list)
        {
            for (auto &f : g.second)
            {
                if ((u->tm_year == f.dTm->tm_year) && (u->tm_mon == f.dTm->tm_mon)
                    && (u->tm_mday == f.dTm->tm_mday) && (u->tm_hour == f.dTm->tm_hour)
                    && (u->tm_min == f.dTm->tm_min) && (abs(u->tm_sec - f.dTm->tm_sec) < 30))
                    bot.getApi().sendMessage(g.first, f.TextOfPrompt);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void initBot(Bot& bot)
{
    for (const auto& c : getAllCommands())
    {
        auto func = c.second;
        bot.getEvents().onCommand(c.first, [&bot, func](Message::Ptr message) {
            func(bot, message);
        });
    }

    bot.getEvents().onCallbackQuery([&bot](CallbackQuery::Ptr query) {

        for (const auto& c : getAllCallbacks())
        {
            if (c.first == query->data)
            {
                auto func = c.second;
                func(bot, query);
                return;
            }
        }

        bot.getApi().sendMessage(query->message->chat->id, "Unknown control!!!");
    });

    bot.getEvents().onAnyMessage([&bot](Message::Ptr message) {
        onAnyMessage(bot, message);
    });
}
