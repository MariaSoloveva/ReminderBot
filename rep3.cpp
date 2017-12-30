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
    
