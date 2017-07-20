//
//  Dates.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "Dates.hpp"
#include "FileManager.hpp"

Dates::Dates() {
    std::vector<std::string> dates = FileManager::getDatesWithData();

    std::sort(dates.begin(), dates.end(), sortDates);

    std::vector<std::string> filteredDates;
    for (unsigned int i = 0; i < dates.size(); i++) {
        std::string date = dates[i];
        if (date == "07_11_2017" ||
            date == "07_12_2017" ||
            date == "07_13_2017" ||
            date == "07_14_2017" ||
            date == "07_17_2017" ||
            date == "07_18_2017" ||
            date == "07_19_2017" ||
            date == "07_20_2017") {
            filteredDates.push_back(date);
        }
    }

    this->dates = filteredDates;
}

unsigned int Dates::numberOfDates() const {
    return (unsigned int)this->dates.size();
}

const std::string& Dates::getDate(const unsigned int& _index) const {
    return this->dates[_index];
}

bool Dates::sortDates(std::string _date1, std::string _date2) {
    time_t time1 = Dates::createTime(_date1);
    time_t time2 = Dates::createTime(_date2);
    return time2 > time1;
}

time_t Dates::createTime(std::string _date) {
    int day = Dates::getDay(_date);
    int month = Dates::getMonth(_date);
    int year = Dates::getYear(_date);

    time_t rawtime;
    time(&rawtime);

    struct tm tm;
    tm = *localtime(&rawtime);
    tm.tm_year = year - 1900;
    tm.tm_mon = month - 1;
    tm.tm_mday = day;
    return mktime(&tm);
}

int Dates::getDay(std::string _date) {
    int firstUnderscore = (int)_date.find('_') + 1;
    int secondUnderscore = (int)_date.find('_', firstUnderscore + 1);
    int length = secondUnderscore - firstUnderscore;
    return atoi(_date.substr(firstUnderscore, length).c_str());
}

int Dates::getMonth(std::string _date) {
    int firstUnderscore = (int)_date.find('_') + 1;
    int length = firstUnderscore;
    return atoi(_date.substr(0, length).c_str());
}

int Dates::getYear(std::string _date) {
    int firstUnderscore = (int)_date.find('_') + 1;
    int secondUnderscore = (int)_date.find('_', firstUnderscore + 1);
    return atoi(_date.substr(secondUnderscore + 1, 4).c_str());
}

void Dates::outputCurrentTime() {
    time_t currentTime;
    time(&currentTime);
    struct tm* now = localtime(&currentTime);

    std::cout << std::setw(2) << std::setfill('0') << now->tm_hour << ":";
    std::cout << std::setw(2) << std::setfill('0') << now->tm_min << " ";
    std::cout << std::setw(2) << std::setfill('0') << now->tm_mon + 1 << "/";
    std::cout << std::setw(2) << std::setfill('0') << now->tm_mday << "/";
    std::cout << now->tm_year + 1900 << std::endl;
}
