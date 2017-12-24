#include <chrono>
#include "api.hpp"
#include "keyboard.hpp"
#include "utils.hpp"
#include <thread>


struct tm* getTime(const std::string& dataOfPrompt)
{
    char str[dataOfPrompt.size()+1];
    strcpy(str, dataOfPrompt.c_str());
    time_t rawtime;
    struct tm* timeinfo;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int seconds = 0;
    std::replace(str, str + strlen(str), '/', ' ');
    std::replace(str, str + strlen(str), ':', ' ');
    std::istringstream(str) >> day >> month >> year >> hour >> min >> seconds;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    timeinfo->tm_year = year;
    timeinfo->tm_mon = month - 1;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = min;
    timeinfo->tm_sec = seconds;
    mktime(timeinfo);
    return timeinfo;
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
    struct tm* dataTm = getTime(message->text);
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
        struct tm *u = list[message->chat->id][position].dTm;
        char str[100];
        strftime(str, sizeof(str), "%c", u);
        bot.getApi().sendMessage(message->chat->id, list[message->chat->id][position].nameOfPrompt);
        bot.getApi().sendMessage(message->chat->id, list[message->chat->id][position].TextOfPrompt);
        bot.getApi().sendMessage(message->chat->id, str);
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

