#include "api.hpp"
#include "utils.hpp"

// Файл, для демонстрации работы с клавиатурой

using namespace std;
using namespace TgBot;
using namespace Utils;


// Функция для демонстрации работы с Inline Keyboard 
// https://core.telegram.org/bots/2-0-intro
// Для вызова необходимо прислать сообщение /keyboard
void onCommandKeyboard(Bot& bot, Message::Ptr message)
{
    // Создаем объект клавиатуры
    InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);

    // создаем новую строку в клавиалуре
    // обычный вектор
    vector<InlineKeyboardButton::Ptr> row0;

    // создаем кнопку
    InlineKeyboardButton::Ptr buttonOne(new InlineKeyboardButton);
    // текс кнопки
    buttonOne->text = fromLocale("Проверка напоминания");
    // текстовый идентификатор кнопки
    // при нажатии на кнопку будет приходить именно этот текст
    // смотри функцию getAllCallbacks
    buttonOne->callbackData = "id_button_one";

    InlineKeyboardButton::Ptr buttonTwo(new InlineKeyboardButton);
    // чтобы вывести русский текст, надо использовать функцию fromLocale
    buttonTwo->text = fromLocale("Удаление напоминания");
    buttonTwo->callbackData = "id_button_two";

    // добавляем кнопки на строку
    row0.push_back(buttonOne);
    row0.push_back(buttonTwo);

    // добавляем строку кнопок в клавиатуру
    keyboard->inlineKeyboard.push_back(row0);


    // создаем еще строку в клавиатуре
    vector<InlineKeyboardButton::Ptr> row1;

    InlineKeyboardButton::Ptr buttonThree(new InlineKeyboardButton);
    buttonThree->text = fromLocale("Создание нового напоминания");
    buttonThree->callbackData = "id_button_three";

    row1.push_back(buttonThree);
    keyboard->inlineKeyboard.push_back(row1);

    // отсылаем сообщение вместе с клавиатурой
    bot.getApi().sendMessage(message->chat->id, "Keyboard is showed!", false, 0, keyboard, "Markdown");
}

// Функция вызывается каждый раз, когда нажимают на кнопку 1
// (а точнее с callbackData == id_button_one)
void onClickButtonOne(Bot& bot, CallbackQuery::Ptr query)
{
    bot.getApi().sendMessage(query->message->chat->id, fromLocale("Введите название напоминания, о котором хотите получить информацию"));
}

// Функция вызывается каждый раз, когда нажимают на кнопку
void onClickButtonTwo(Bot& bot, CallbackQuery::Ptr query)
{
    bot.getApi().sendMessage(query->message->chat->id, fromLocale("Введите название напоминания, которое хотите удалить"));
}
// Функция вызывается каждый раз, когда нажимают на кнопку  3
void onClickButtonThree(Bot& bot, CallbackQuery::Ptr query)
{
    bot.getApi().sendMessage(query->message->chat->id, fromLocale("Создаем новое напоминание.Как Вы хотите его назвать?"));
}
// Регистрируем наши кнопки
std::map<std::string, std::function<void(Bot&, CallbackQuery::Ptr)>> getAllCallbacks()
{
    // Ключом является идентификатор кнопки
    // значением является функция-обработчик кнопки
    // т.е. при нажатии на кнопку с идентификатором, произойдет вызов соответствующей функции
    // Например, при нажании на кнопку с идентификатором id_button_one вызовется функция onClickButtonOne
    std::map<std::string, std::function<void(Bot&, CallbackQuery::Ptr)>> callbacks =
    {
        {"id_button_one", onClickButtonOne},
        {"id_button_two", onClickButtonTwo},
        {"id_button_three", onClickButtonThree}
    };

    return callbacks;
}

