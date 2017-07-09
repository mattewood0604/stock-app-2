//
//  ZigZag.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Candle.hpp"
#include "ZigZag.hpp"

ZigZag::ZigZag(const unsigned int& _candleOffset, const unsigned int& _movingAverageLength) {
    this->lastPoint = 0;

    this->candleOffset = _candleOffset;
    this->movingAverageLength = _movingAverageLength;
}

ZigZag::ZigZag(const ZigZag& _zigZag) {
    this->lastPoint = 0;

    this->candleOffset = _zigZag.candleOffset;
    this->movingAverageLength = _zigZag.movingAverageLength;
}

BuySell ZigZag::indicator(const Stock& _stock) {
    BuySell buySell;
    
    if (!this->isValid()) {
        return buySell;
    }

    const Candle& currentCandle = _stock.candles[_stock.candles.size() - 1];
    if (currentCandle.getClose() == 0) {
        return buySell;
    }

    if (_stock.candles.size() < 2 * this->candleOffset + this->movingAverageLength) {
        return buySell;
    }

    int lh = 0;
    int ll = 0;

    int searchIndex = 2 * this->candleOffset + 1;

    float highMa = 0;
    float lowMa = 0;
    for (unsigned int i = 0; i < this->movingAverageLength; i++) {
        const Candle& maCandle = _stock.candles[_stock.candles.size() - i - this->candleOffset - 1];
        highMa += maCandle.getHigh();
        lowMa += maCandle.getLow();
    }
    highMa /= this->movingAverageLength;
    lowMa /= this->movingAverageLength;

    float high = highMa;
    float highest = 0;
    float low = lowMa;
    float lowest = 1000000;
    for (int i = 0; i < searchIndex; i++) {
        highMa = 0;
        lowMa = 0;
        for (unsigned int j = 0; j < this->movingAverageLength; j++) {
            const Candle& maCandle = _stock.candles[_stock.candles.size() - i - j - 1];
            highMa += maCandle.getHigh();
            lowMa += maCandle.getLow();
        }
        highMa /= this->movingAverageLength;
        lowMa /= this->movingAverageLength;

        float localHigh = highMa;
        if (highest < localHigh) {
            highest = localHigh;
        }

        float localLow = lowMa;
        if (lowest > localLow) {
            lowest = localLow;
        }
    }

    if (high >= highest) {
        lh = 1;
    }
    else {
        lh = 0;
    }

    if (low <= lowest) {
        ll = -1;
    }
    else {
        ll = 0;
    }

    float topX = 0;
    float topY = 0;
    float botX = 0;
    float botY = 0;
    if (lh == 1) {
        topY = high;
        topX = _stock.candles.size();
    }

    if (ll == -1) {
        botY = low;
        botX = _stock.candles.size();
    }

    if (lh > 0 && (this->lastPoint == -1 || this->lastPoint == 0)) {
        this->lastPoint = 1;
        buySell.isSell = true;
    }

    if (ll < 0 && (this->lastPoint == 1 || this->lastPoint == 0)) {
        this->lastPoint = -1;
        buySell.isBuy = true;
    }
    
    return buySell;
}

bool ZigZag::isValid() const {
    return this->candleOffset && this->movingAverageLength;
}
