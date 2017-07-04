//
//  Stock.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Candle.hpp"
#include "Constants.hpp"
#include "Stock.hpp"

#include <iostream>

Stock::Stock(const std::string& _symbol, const ZigZag& _zigZag) : zigZag(_zigZag) {
    this->symbol = _symbol;

    this->maxCandleTime = 60000;
    this->perTradeLoss = 0.01;

    this->candles = std::vector<Candle>();
    this->candles.push_back(Candle(this->maxCandleTime));

    this->dailyProfits = 0;
    this->totalProfits = 0;
}

void Stock::clearData() {
    this->ticks = std::vector<Tick>();

    this->candles = std::vector<Candle>();
    this->candles.push_back(Candle(this->maxCandleTime));
}

void Stock::addTick(const Tick& _tick) {
    _tick.log();
    this->ticks.push_back(_tick);
}

void Stock::addTickToCandle(const Tick& _tick) {
    this->currentTick = _tick;

    Candle& candle = this->candles[this->candles.size() - 1];
    if (candle.isComplete()) {
        candle.log();
        
        //IndicatorAlgorithms::calculateWaveTrend(*this);
        //  IndicatorAlgorithms::macd(*this);
        //  IndicatorAlgorithms::knowSureThing(*this);
        //IndicatorAlgorithms::williamsVix(*this);
        //IndicatorAlgorithms::simpleMovingAverages(*this);

        Candle nextCandle(this->maxCandleTime);
        nextCandle.addTick(_tick);
        this->candles.push_back(nextCandle);
    }
    else {
        candle.addTick(_tick);
    }
}

void Stock::indicators() {
    if (this->buyOrder.isValid()) {
        if ((this->currentTick.price - this->buyOrder.price) / this->buyOrder.price <= -this->perTradeLoss) {
            this->buySell.isSell = true;
            return;
        }
    }

    BuySell zigZag = this->zigZag.indicator(*this);
    this->buySell.merge(zigZag);
}

void Stock::order() {
    if (!this->getLastCandle().isComplete()) {
        return;
    }

    if (this->buySell.isBuy) {
        std::cout << "BUY  (" << this->candles.size() << "):\t" << this->currentTick.price << std::endl;
        this->buyOrder = Order(this->currentTick.price, 1);
    }
    else if (this->buySell.isSell && this->buyOrder.isValid()) {
        std::cout << "SELL (" << this->candles.size() << "):\t" << this->currentTick.price << std::endl;

        float profit = this->currentTick.price - this->buyOrder.price;
        this->dailyProfits += profit;
        this->totalProfits += profit;
        this->buyOrder = Order();
    }

    this->buySell.reset();
}

void Stock::logProfits() const {
    std::cout << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "Profit: " << std::endl;
    std::cout << "  Daily: " << this->dailyProfits << std::endl;
    std::cout << "  Total: " << this->totalProfits << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << std::endl;
}

void Stock::resetProfits() {
    this->dailyProfits = 0;

    if (Constants::clearOrderAtDayEnd) {
        this->buyOrder = Order();
    }
}

const std::string& Stock::getSymbol() const {
    return this->symbol;
}

const Tick& Stock::getTick(const unsigned int &_time) const {
    return this->ticks[_time];
}

const unsigned int Stock::totalTicks() const {
    return (unsigned int)this->ticks.size();
}

const Candle& Stock::getLastCandle() const {
    return this->candles[this->candles.size() - 1];
}

const float& Stock::getTotalProfits() const {
    return this->totalProfits;
}

//////////////////////////////////////////////////////////////////////////////
//                                 Zig Zag                                  //
//////////////////////////////////////////////////////////////////////////////
Stock::ZigZag::ZigZag(const unsigned int& _candleOffset, const unsigned int& _movingAverageLength) {
    this->lastPoint = 0;

    this->candleOffset = _candleOffset;
    this->movingAverageLength = _movingAverageLength;
}

Stock::BuySell Stock::ZigZag::indicator(const Stock& _stock) {
    Stock::BuySell buySell;

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

    float ma = 0;
    for (unsigned int i = 0; i < this->movingAverageLength; i++) {
        const Candle& maCandle = _stock.candles[_stock.candles.size() - i - this->candleOffset - 1];
        ma += maCandle.getHigh();
    }
    ma /= this->movingAverageLength;

    float high = ma;
    float highest = 0;
    for (int i = 0; i < searchIndex; i++) {
        ma = 0;
        for (unsigned int j = 0; j < this->movingAverageLength; j++) {
            const Candle& maCandle = _stock.candles[_stock.candles.size() - i - j - 1];
            ma += maCandle.getHigh();
        }
        ma /= this->movingAverageLength;

        float localHigh = ma;
        if (highest < localHigh) {
            highest = localHigh;
        }
    }

    if (high >= highest) {
        lh = 1;
    }
    else {
        lh = 0;
    }

    ma = 0;
    for (unsigned int i = 0; i < this->movingAverageLength; i++) {
        const Candle& maCandle = _stock.candles[_stock.candles.size() - i - this->candleOffset - 1];
        ma += maCandle.getLow();
    }
    ma /= this->movingAverageLength;

    float low = ma;
    float lowest = 1000000;
    for (int i = 0; i < searchIndex; i++) {
        ma = 0;
        for (unsigned int j = 0; j < this->movingAverageLength; j++) {
            const Candle& maCandle = _stock.candles[_stock.candles.size() - i - j - 1];
            ma += maCandle.getLow();
        }
        ma /= this->movingAverageLength;

        float localLow = ma;
        if (lowest > localLow) {
            lowest = localLow;
        }
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

    if (lh > 0 && (this->lastPoint == -1 || this->lastPoint == 0)) {// && _stock.isBought)  {
        this->lastPoint = 1;
        buySell.isSell = true;
    }

    if (ll < 0 && (this->lastPoint == 1 || this->lastPoint == 0)) {
        this->lastPoint = -1;
        buySell.isBuy = true;
    }
    
    return buySell;
}

//Stock::Stock() : currentQuote(), stockModel("") {
//    this->testQuotes = std::vector<TimeQuote>();
//
//    this->reset();
//}
//
//Stock::Stock(const std::string& _symbol) : currentQuote(), stockModel(_symbol) {
//    this->symbol = _symbol;
//    //std::string url = RestCall::urlForStockSymbol(_symbol);
//    //if (url.length() == 0) {
//    //  std::cout << _symbol << std::endl;
//    //}
//    //this->instrumentUrl = url.length() ? url : "";
//
//    //std::cout << this->instrumentUrl << std::endl;
//
//    this->testQuotes = std::vector<TimeQuote>();
//
//    this->reset();
//}
//
//void Stock::reset() {
//    this->waveTrendComplete = false;
//    this->macdComplete = false;
//
//    this->averagePriceEMA = -100000;
//    this->apESA = 0;
//    this->apESACalculated = 0;
//    this->ci = 0;
//    this->ciCalculated = 0;
//    this->previousW1 = std::vector<float>();
//    this->previousKST = std::vector<float>();
//    this->previousMACDSignals = std::vector<float>();
//
//    this->kst = 0;
//    this->kstSignalLine = 0;
//    this->kstComplete = false;
//
//    this->isBought = false;
//    this->isBuy = false;
//    this->isSell = false;
//
//    this->moneyMade = 0;
//    this->buyPrice = 0;
//
//    this->shortMultiplier = 2.0f / (this->stockModel.getShortTimePeriods() + 1);
//    this->longMultiplier = 2.0f / (this->stockModel.getLongTimePeriods() + 1);
//
//    this->maxLossTaken = false;
//    this->maxMade = 0;
//
//    this->numberOfTrades = 0;
//    this->negativeTrades = 0;
//
//    this->numberOfQuotes = 0;
//
//    this->canStartBuying = false;
//
//    this->isReady = false;
//    this->isSet = false;
//    this->isGo = false;
//}
//
//void Stock::addTimeToCandles(TimeQuote& _timeQuote) {
//    this->currentQuote = _timeQuote;
//
//    const unsigned int& maxCandleTime = this->stockModel.getMaxCandleTime();
//
//    Candle& candle = this->getLastCandle();
//    if (candle.getTotalTime() >= maxCandleTime) {
//        //IndicatorAlgorithms::calculateWaveTrend(*this);
//        //  IndicatorAlgorithms::macd(*this);
//        //  IndicatorAlgorithms::knowSureThing(*this);
//        //IndicatorAlgorithms::williamsVix(*this);
//        //IndicatorAlgorithms::simpleMovingAverages(*this);
//
//        Candle nextCandle = Candle(maxCandleTime);
//        nextCandle.addTimeQuote(_timeQuote);
//        this->candles.push_back(nextCandle);
//    }
//    else {
//        candle.addTimeQuote(_timeQuote);
//    }
//}
//
//Candle& Stock::getLastCandle() {
//    return this->candles[this->candles.size() - 1];
//}
//
//unsigned int Stock::getNumberOfCandles() const {
//    return (unsigned int)this->candles.size();
//}
//
//const Candle& Stock::getCandleAtIndex(const unsigned int& _index) const {
//    return this->candles[_index];
//}
//
//void Stock::addQuoteToTestData(TimeQuote& _timeQuote) {
//    this->testQuotes.push_back(_timeQuote);
//}
//
//void Stock::setWaveTrendComplete(const bool& _complete) {
//    this->waveTrendComplete = _complete;
//}
//
//float Stock::ema(const float& _price, const float& _previousEMA, const float& _multiplier) const {
//    return (_price - _previousEMA) * _multiplier + _previousEMA;
//}
//
//void Stock::logMoneyMade() const {
//    std::string holding = this->isBought ? "holding" : "not holding";
//    float percentage = (this->buyPrice == 0) ? 0 : (this->moneyMade / this->buyPrice);
//    std::cout << this->symbol << "\t"
//    << this->moneyMade << "\t"
//    << percentage * 100 << "\t"
//    << holding << "\n"
//    << "Trades: \t" << this->numberOfTrades << "\n"
//    << "Negatives: \t" << this->negativeTrades << "\n"
//    << "Candle #: \t" << this->getNumberOfCandles() << "\n"
//    << std::endl << std::endl;
//}
//
//StockModel& Stock::getStockModel() {
//    return this->stockModel;
//}
//
//float Stock::getPercentageMade() const {
//    if (this->buyPrice == 0) {
//        return 0.0;
//    }
//
//    return (this->moneyMade) / this->buyPrice;
//}
//
//const float& Stock::getShortMultiplier() const {
//    return this->shortMultiplier;
//}
//
//const float& Stock::getLongMultiplier() const {
//    return this->longMultiplier;
//}
//
//TimeQuote& Stock::getTestQuote(const unsigned int& _marketTime) {
//    return this->testQuotes[_marketTime];
//}
