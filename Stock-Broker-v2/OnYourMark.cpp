//
//  OnYourMark.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "OnYourMark.hpp"

#include <iostream>

OnYourMark::OnYourMark(int _crossPeriods, int _kijunPeriods, int _overKijunPeriods, float _readySignalPercent) {
    this->crossPeriods = _crossPeriods;
    this->kijunPeriods = _kijunPeriods;
    this->overKijunPeriods = _overKijunPeriods;
    this->readySignalPercent = _readySignalPercent;

    this->isReady = false;
    this->isSet = false;
}

OnYourMark::OnYourMark(const OnYourMark& _onYourMark) {
    this->crossPeriods = _onYourMark.crossPeriods;
    this->kijunPeriods = _onYourMark.kijunPeriods;
    this->overKijunPeriods = _onYourMark.overKijunPeriods;
    this->readySignalPercent = _onYourMark.readySignalPercent;

    this->isReady = false;
    this->isSet = false;
}

bool OnYourMark::determineSignal(Stock& _stock, unsigned int offset) {
    float high = 0;
    float low = 100000;
    for (unsigned int i = 0; i < this->kijunPeriods; i++) {
        float pastHigh = _stock.candles[_stock.candles.size() - 1 - i].getHigh();
        float pastLow = _stock.candles[_stock.candles.size() - 1 - i].getLow();
        if (high < pastHigh) {
            high = pastHigh;
        }

        if (low > pastLow) {
            low = pastLow;
        }
    }

    float kijun = (high + low) / 2.0;
    unsigned int index = (unsigned int)_stock.candles.size() - 1;
    float averagePercent = ((kijun - _stock.candles[index - offset].getClose()) / kijun) * 10000;
    std::cout << averagePercent << std::endl;

    float lastAveragePercent = ((kijun - _stock.candles[index - offset - 1].getClose()) / kijun) * 10000;
    return low < kijun && averagePercent < this->readySignalPercent && averagePercent > 0 && lastAveragePercent < averagePercent;
}

bool OnYourMark::determineKijunCross(Stock& _stock) {
    for (unsigned int i = 0; i < this->crossPeriods; i++) {
        if (determineSignal(_stock, i)) {
            return true;
        }
    }

    return false;
}

bool OnYourMark::determineOverKijun(Stock& _stock) {
    for (unsigned int i = 0; i < this->overKijunPeriods; i++) {
        if (determineKijunCross(_stock)) {
            return true;
        }
    }
    
    return false;
}

BuySell OnYourMark::indicator(Stock& _stock) {
    BuySell buySell;

    if (!this->isValid()) {
        return buySell;
    }

    if (_stock.candles.size() <= this->crossPeriods + this->kijunPeriods + 1) {
        return buySell;
    }

    const Candle& currentCandle = _stock.candles[_stock.candles.size() - 1];
    if (currentCandle.getClose() == 0) {
        return buySell;
    }

    bool signal = determineSignal(_stock);
    if (signal) {
        this->readyCandle = (unsigned int)_stock.candles.size();
        //this->isReady = true;
    }

    float highest = _stock.getLastCandle().getHigh();
    float lowest = _stock.getLastCandle().getLow();
    for (unsigned int i = 0; i < this->kijunPeriods; i++) {
        float pastHigh = _stock.candles[_stock.candles.size() - 1 - i].getHigh();
        float pastLow = _stock.candles[_stock.candles.size() - 1 - i].getLow();

        if (pastHigh > highest) {
            highest = pastHigh;
        }

        if (pastLow < lowest) {
            lowest = pastLow;
        }
    }


    bool kijunCross = false; //determineKijunCross(_stock);
    if (this->readyCandle + this->crossPeriods >= _stock.candles.size()) {
        kijunCross = true;
    }

    float kijun = (highest + lowest) / 2.0;
    if (kijunCross && _stock.getLastCandle().getClose() >= kijun) {
        this->setCandle = (unsigned int)_stock.candles.size();
        //this->isSet = true;
    }

    bool overKijun = false; //determineOverKijun(_stock);
    if (this->setCandle + this->overKijunPeriods >= _stock.candles.size()) {
        overKijun = true;
    }

    if (overKijun && _stock.getLastCandle().getLow() > kijun) {
        //this->isReady = false;
        //this->isSet = false;
        buySell.isBuy = true;
    }

    return buySell;
}

bool OnYourMark::isValid() const {
    return this->crossPeriods && this->kijunPeriods && this->overKijunPeriods && this->readySignalPercent;
}
