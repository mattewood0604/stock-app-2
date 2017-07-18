//
//  Market.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/16/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "Market.hpp"
#include "RestCall.hpp"

#include <iostream>
#include <iomanip>
#include <time.h>
#include <unistd.h>

Market::Market() {
    this->open = false;
    this->closed = true;
}

bool Market::isOpen() const {
    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);

    float timeAsFloat = now->tm_hour + (now->tm_min / 60.0f);
    if (timeAsFloat > 9.5f && timeAsFloat < 16.0f) { // After 9:30AM and before 4:00PM
        return true;
    }

    return false;
}

void Market::waitForNextOpening() const {
    RestCall restCall;
    time_t currentTime = time(0);

    MarketInfo marketInfo = restCall.getInfoForToday();

    int todayTime = (int)(marketInfo.todayAsTime() - currentTime);
    int nextDayTime = (int)(marketInfo.nextOpenDayAsTime() - currentTime);

    int timeToSleep = todayTime > 0 ? todayTime : nextDayTime;
    sleep(timeToSleep);

    while (!this->isOpen()) {
        this->waitForMinute();
    }
}

void Market::waitForMinute() const {
    sleep(60);
}
