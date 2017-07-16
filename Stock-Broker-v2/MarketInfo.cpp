//
//  MarketInfo.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/16/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "MarketInfo.hpp"

#include <iomanip>

MarketInfo::MarketInfo() {
    this->nextOpenDay = "";
    this->opensToday = false;
}

time_t MarketInfo::nextOpenDayAsTime() const {
    int day = this->getDay();
    int month = this->getMonth();
    int year = this->getYear();
    
    time_t rawtime;
    time(&rawtime);

    struct tm tm;
    tm = *localtime(&rawtime);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    tm.tm_hour = 9;
    tm.tm_min = 25;
    return mktime(&tm);
}

int MarketInfo::getDay() const {
    int firstUnderscore = (int)this->nextOpenDay.find('-') + 1;
    int secondUnderscore = (int)this->nextOpenDay.find('-', firstUnderscore + 1);
    return atoi(this->nextOpenDay.substr(secondUnderscore + 1, 4).c_str());
}

int MarketInfo::getMonth() const {
    int firstUnderscore = (int)this->nextOpenDay.find('-') + 1;
    int secondUnderscore = (int)this->nextOpenDay.find('-', firstUnderscore + 1);
    int length = secondUnderscore - firstUnderscore;
    return atoi(this->nextOpenDay.substr(firstUnderscore, length).c_str());
}

int MarketInfo::getYear() const {
    int firstUnderscore = (int)this->nextOpenDay.find('-') + 1;
    int length = firstUnderscore;
    return atoi(this->nextOpenDay.substr(0, length).c_str());
}
