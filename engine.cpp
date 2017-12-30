#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include "api.hpp"
#include "keyboard.hpp"
#include "utils.hpp"

int countNumberOfDays(bool leapYearOrNot, int month, int day)
{
    if (leapYearOrNot)
    {
        if (month == 1)
            return day;
        if (month == 2)
            return day + 31;
        if (month == 3)
            return day + 60;
        if (month == 4)
            return day + 91;
        if (month == 5)
            return day + 121;
        if (month == 6)
            return day + 152;
        if (month == 7)
            return day + 182;
        if (month == 8)
            return day + 213;
        if (month == 9)
            return day + 244;
        if (month == 10)
            return day + 274;
        if (month == 11)
            return day + 305;
        if (month == 12)
            return day + 335;
    }
    else
    {
        if (month == 1)
            return day;
        if (month == 2)
            return day + 31;
        if (month == 3)
            return day + 59;
        if (month == 4)
            return day + 90;
        if (month == 5)
            return day + 120;
        if (month == 6)
            return day + 151;
        if (month == 7)
            return day + 181;
        if (month == 8)
            return day + 212;
        if (month == 9)
            return day + 243;
        if (month == 10)
            return day + 273;
        if (month == 11)
            return day + 304;
        if (month == 12)
            return day + 334;
    }
}
int getTimeinSeconds(const std::string& dataOfPrompt)
{
    char str[dataOfPrompt.size()+1];
    strcpy(str, dataOfPrompt.c_str());
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int seconds = 0;
    std::replace(str, str + strlen(str), '/', ' ');
    std::replace(str, str + strlen(str), ':', ' ');
    std::istringstream(str) >> day >> month >> year >> hour >> min >> seconds;
    bool leapYearOrNot = false;
    if (year % 4 == 0)
        leapYearOrNot = true;
    year = year - 1970;
    int yearV = year / 4;
    year = year - yearV;
    int numberOfDays = countNumberOfDays(leapYearOrNot, month, day);
    int timeInSeconds = abs(seconds + min * 60 + hour * 3600 + numberOfDays * 3600 * 24 + yearV * 366 * 24 * 3600 + year *365 * 24 * 3600 - 10800);
    return timeInSeconds;
}

// Файл, для демонстрации работы с командами и другими сообщениями

using namespace std;
using namespace TgBot;
using namespace Utils;

// Функция вызывается каждый раз, когда приходит сообщение, которое начинается со /photo
// В ответ пользователю придет сообщение с картинкой
void onCommandCreate(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, please");
}

int position = -1;
Prompt NewPrompt;

void onCommandCreateDoNameOfPrompt(Bot& bot, Message::Ptr message)
{
    NewPrompt.nameOfPrompt = message->text;
    bot.getApi().sendMessage(message->chat->id, "Enter text of prompt");
}
void onCommandCreateDoTextOfPrompt(Bot& bot, Message::Ptr message)
{
    NewPrompt.TextOfPrompt = message->text;
    bot.getApi().sendMessage(message->chat->id, "Enter data of prompt(for example, DD/MM/YY HH:MM:SS)");
}
void onCommandCreateDoDataOfPrompt(Bot& bot, Message::Ptr message)
{
    int dataTm = getTimeinSeconds(message->text);
    NewPrompt.dateInString = message->text;
    NewPrompt.dTm = dataTm;
    list[message->chat->id].push_back(NewPrompt);
    bot.getApi().sendMessage(message->chat->id, "Done");
}
void onCommandDelete(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, you want to delete");
}
void getPosition(Bot& bot, Message::Ptr message)
{
    std::string nameOfPrompt = message->text;
    for(size_t k = 0; k < list[message->chat->id].size(); ++k)
    {
        if (list[message->chat->id][k].nameOfPrompt == nameOfPrompt)
        {
            position = k;
            break;
        }
    }
}
void deleteFromVector(std::vector<Prompt>& vec)
{
    vec.erase(vec.begin() + position);
}
void onCommandContinueDelete(Bot& bot, Message::Ptr message)
{
    getPosition(bot, message);
    if (position != -1)
    {
        deleteFromVector(list[message->chat->id]);
        bot.getApi().sendMessage(message->chat->id, "Done!");
        position = -1;
    }
    else
    {
        bot.getApi().sendMessage(message->chat->id, "There is no prompt with this name");
    }

}
void onCommandTest(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, you want to know about");
}
void onCommandContinueTest(Bot& bot, Message::Ptr message)
{
    getPosition(bot, message);
    if (position != -1)
    {
        bot.getApi().sendMessage(message->chat->id, list[message->chat->id][position].nameOfPrompt);
        bot.getApi().sendMessage(message->chat->id, list[message->chat->id][position].TextOfPrompt);
        bot.getApi().sendMessage(message->chat->id, list[message->chat->id][position].dateInString);
        position = -1;
    }
    else
    {
        bot.getApi().sendMessage(message->chat->id, "There is no prompt with this name");
    }
}
// Функция для демонстрации работы с командами
// Функция вызывается каждый раз, когда приходит сообщение, которое начинается со /start
void onCommandStart(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Hi!");
    bot.getApi().sendMessage(message->chat->id, "There are commands that i can do: ");
    bot.getApi().sendMessage(message->chat->id, "/start - a command to start communicating");
    bot.getApi().sendMessage(message->chat->id, "/create - a command to start create a new prompt");
    bot.getApi().sendMessage(message->chat->id, "/test - a command to know information about a prompt");
    bot.getApi().sendMessage(message->chat->id, "/delete - a command to delete your prompt");
    bot.getApi().sendMessage(message->chat->id, "/end - a command to end communication ");
}

// Функция для демонстрации работы с командами
// Функция вызывается каждый раз, когда приходит сообщение, которое начинается со /end
void onCommandEnd(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Bye!");
}

// регистрируем команды
std::map<std::string, std::function<void(Bot&, Message::Ptr)>> getAllCommands()
{
    // Ключом является идентификатор команды
    // значением является функция-обработчик коаманды
    // Например, при получении команды /end вызовется функция onCommandEnd
    std::map<std::string, std::function<void(Bot&, Message::Ptr)>> commands =
    {
        {"start", onCommandStart},
        {"end", onCommandEnd},
        {"keyboard", onCommandKeyboard},
        {"create", onCommandCreate},
        {"delete", onCommandDelete},
        {"test", onCommandTest},
    };

    return commands;
}


int value = 0;
int j = 0;
int k = 0;
// Функция, которая вызывается при любом сообщении
void onAnyMessage(Bot& bot, Message::Ptr message)
{
    // логгируем действия в консоль
    //printf("User wrote %s\n", message->text.c_str());

    // игнорируем сообщения, которые начинаются с /start и /end
    if (StringTools::startsWith(message->text, "/start"))
    {
        return;
    }
    if (StringTools::startsWith(message->text, "/end"))
    {
        return;
    }
    if (StringTools::startsWith(message->text, "/create"))
    {
        value = 1;
        return;
    }
    if (value == 1)
    {
        onCommandCreateDoNameOfPrompt(bot, message);
        value = 2;
        return;
    }
    if (value == 2)
    {
        onCommandCreateDoTextOfPrompt(bot, message);
        value = 3;
        return;
    }
    if (value == 3)
    {
        onCommandCreateDoDataOfPrompt(bot, message);
        value = 0;
        return;
    }
    if (StringTools::startsWith(message->text, "/delete"))
    {
        j = 1;
        return;
    }
    if (j == 1)
    {
        onCommandContinueDelete(bot, message);
        j = 0;
        return;
    }
    if (StringTools::startsWith(message->text, "/test"))
    {
        k = 1;
        return;
    }
    if (k == 1)
    {
        getPosition(bot, message);
        onCommandContinueTest(bot, message);
        k = 0;
        return;
    }
    // если в тексте сообщения есть "Привет" приветствуем собеседника
    std::string hello = Utils::fromLocale("Привет");
    if (message->text.find(hello) != std::string::npos)
    {
        bot.getApi().sendMessage(message->chat->id, Utils::fromLocale("Привет, ") + message->from->firstName);
        return;
    }

}

