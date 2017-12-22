#include "api.hpp"
#include "keyboard.hpp"
#include "utils.hpp"


struct tm* getTime(std::string dataOfPrompt)
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
struct Prompt
{
    std::string TextOfPrompt;
    struct tm* dTm;
};

std::map<int64_t, std::map<std::string, Prompt>> list;
// Файл, для демонстрации работы с командами и другими сообщениями

using namespace std;
using namespace TgBot;
using namespace Utils;

// Функция вызывается каждый раз, когда приходит сообщение, которое начинается со /photo
// В ответ пользователю придет сообщение с картинкой
void onCommandCreate(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, please");
    //получить название напоминания
    std::string nameOfPrompt = message->text;
    //получить текст напоминания
    bot.getApi().sendMessage(message->chat->id, "Enter text of prompt");
    std::string textOfPrompt = message->text;
    //получить дату
    bot.getApi().sendMessage(message->chat->id, "Enter data of prompt(for example, DD/MM/YY HH:MM:SS)");
    std::string dataOfPrompt = message->text;
    //записать дату в структуру
    struct tm* dataTm = getTime(dataOfPrompt);
    //добавить текст и дату в структуру
    Prompt PromptNewStruct
            {
                    textOfPrompt,
                    dataTm
            };
    //создаем map чтобы записать в него информацию о напоминании, сможем взять информацию по "названию"
    std::map<std::string, Prompt> newPrompt;
    newPrompt[nameOfPrompt] = PromptNewStruct;
    //добавить в map id и всю информацию
    list[message->chat->id] = newPrompt;
    bot.getApi().sendMessage(message->chat->id, "Done!");
}
void onCommandDelete(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, you want to delete");
    std::string nameOfPrompt = message->text;
    //удалить напоминание
    list[message->chat->id].erase(nameOfPrompt);
    bot.getApi().sendMessage(message->chat->id, "Done!");
}
void onCommandTest(Bot& bot, Message::Ptr message)
{
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, you want to know about");
    //Получить название напоминания
    std::string nameOfPrompt = message->text;
    bot.getApi().sendMessage(message->chat->id, "Enter a name of the prompt, you want to know about");
    std::string s = list[message->chat->id][nameOfPrompt].TextOfPrompt;
    struct tm* u = list[message->chat->id][nameOfPrompt].dTm;
    char str[256];
    //вывести информацию о напоминании
    strftime(str, sizeof(str), "%c", u);
    bot.getApi().sendMessage(message->chat->id, nameOfPrompt);
    bot.getApi().sendMessage(message->chat->id, s);
    bot.getApi().sendMessage(message->chat->id, str);
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
        return;
    }
    if (StringTools::startsWith(message->text, "/delete"))
    {
        return;
    }
    if (StringTools::startsWith(message->text, "/test"))
    {
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
