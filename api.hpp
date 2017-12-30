#pragma once

#include <functional>
#include <map>
#include <string>
#include <tgbot/tgbot.h>

struct Prompt
{
    std::string nameOfPrompt;
    std::string TextOfPrompt;
    int dTm;
    std::string dateInString;

};

extern std::map<int64_t, std::vector<Prompt>> list;

void loop(TgBot::Bot& bot);
std::string readToken(const std::string& filename);

void initBot(TgBot::Bot& bot);


// Обязательно, требуется реализовать эту функцию.
std::map<std::string, std::function<void(TgBot::Bot&, TgBot::CallbackQuery::Ptr)>> getAllCallbacks();

// Обязательно, требуется реализовать эту функцию.
std::map<std::string, std::function<void(TgBot::Bot&, TgBot::Message::Ptr)>> getAllCommands();

// Обязательно, требуется реализовать эту функцию.
void onAnyMessage(TgBot::Bot& bot, TgBot::Message::Ptr message);
