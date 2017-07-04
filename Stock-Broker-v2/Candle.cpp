//
//  Candle.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Candle.hpp"
#include "Constants.hpp"

#include <iostream>

Candle::Candle(const unsigned int& _maxTime) {
    this->lastTime = 0;
    this->totalTime = 0;
    this->high = 0;
    this->low = INT64_MAX;
    this->close = 0;
    this->open = 0;

    this->maxTime = _maxTime;
}

void Candle::setLastTime(const uint64_t& _lastTime) {
    this->lastTime = _lastTime;
}

void Candle::addTick(const Tick& _tick) {
    if (this->lastTime != 0) {
        this->totalTime += _tick.dayTime - this->lastTime;
    }
    else {
        this->open = _tick.price;
    }

    if (this->high < _tick.price) {
        this->high = _tick.price;
    }

    if (this->low > _tick.price) {
        this->low = _tick.price;
    }

    if (this->totalTime >= this->maxTime) {
        this->close = _tick.price;
    }

    this->lastTime = _tick.dayTime;
}

float Candle::getAveragePrice() const {
    return (this->open + this->close) / 2.0f;
    //return (this->high + this->low + this->close + this->open) / 4.0f;
    //return (this->high + this->low + this->close) / 3.0f;
}

bool Candle::isComplete() const {
    return this->totalTime >= this->maxTime;
}

float Candle::getOpen() const {
    return this->open;
}

float Candle::getClose() const {
    return this->close;
}

float Candle::getHigh() const {
    return this->high;
}

float Candle::getLow() const {
    return this->low;
}

void Candle::log() const {
    if (!Constants::logCandles) {
        return;
    }

    std::cout << "{" << std::endl;
    std::cout << "\tOpen: " << this->open << std::endl;
    std::cout << "\tClose: " << this->close << std::endl;
    std::cout << "\tHigh: " << this->high << std::endl;
    std::cout << "\tLow: " << this->low << std::endl;
    std::cout << "}" << std::endl;
}
