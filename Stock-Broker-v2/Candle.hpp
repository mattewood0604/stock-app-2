//
//  Candle.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Candle_hpp
#define Candle_hpp

#include <stdint.h>

#include "Tick.hpp"

class Candle {
public:
    Candle(const unsigned int& _maxTime);

    void setLastTime(const uint64_t& _lastTime);
    void addTick(const Tick& _tick);

    float getAveragePrice() const;
    bool isComplete() const;

    float getOpen() const;
    float getClose() const;
    float getHigh() const;
    float getLow() const;

    void log() const;

private:
    unsigned int maxTime;

    uint64_t lastTime;
    unsigned int totalTime;
    float high;
    float low;
    float close;
    float open;
};

#endif /* Candle_hpp */
