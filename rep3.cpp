struct timeFromString
{
    int time_year;
    int time_mon;
    int time_mday;
    int time_hour;
    int time_min;
    int time_sec;
};
struct timeFromString* getTime(const std::string& dataOfPrompt)
{
    char str[dataOfPrompt.size()+1];
    strcpy(str, dataOfPrompt.c_str());
    //time_t rawtime;
    timeFromString* timeS;
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int min = 0;
    int seconds = 0;
    std::replace(str, str + strlen(str), '/', ' ');
    std::replace(str, str + strlen(str), ':', ' ');
    std::istringstream(str) >> day >> month >> year >> hour >> min >> seconds;
    //time(&rawtime);
    //timeinfo = localtime(&rawtime);
    timeS->time_year = year - 1900;
    timeS->time_mon = month - 1;
    timeS->time_mday = day;
    timeS->time_hour = hour;
    timeS->time_min = min;
    timeS->time_sec = seconds;
    return timeS;
}



struct tm *u;
    const time_t timer = time(NULL);
    u = localtime(&timer);
for(auto &g : list)
    {
        for (auto &f : g.second)
        {
            if ((u->tm_year == f.dTm->time_year) && (u->tm_mon == f.dTm->time_mon) &&
                    abs((u->tm_mday * 24 * 3600 + u->tm_hour * 3600 + u->tm_min * 60 + u->tm_sec)
                    - (list[id1][inner].dTm->time_mday * 24 * 3600 + list[id1][inner].dTm->time_hour * 3600
                       + list[id1][inner].dTm->time_min * 60 + list[id1][inner].dTm->time_sec)) < 30)
                std::cout << g.first << " " << f.TextOfPrompt << std::endl;
        }
    }
    







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
    long long int timeInSeconds = abs(seconds + min * 60 + hour * 3600 + numberOfDays * 3600 * 24 + yearV * 366 * 24 * 3600 + year *365 * 24 * 3600 - 10800);
    std::cout << timeInSeconds << std::endl;
    return timeInSeconds;
}

struct Prompt
{
    std::string NameOfPrompt;
    std::string TextOfPrompt;
    double dTm;
    std::string dateInString;
};

auto now = std::chrono::system_clock::now();
    time_t now_t = std::chrono::system_clock::to_time_t(now);
    time(&now_t);
