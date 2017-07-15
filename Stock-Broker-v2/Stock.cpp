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

Stock::Stock(const std::string& _symbol, const float& _perTradeLoss, const ZigZag& _zigZag, const OnYourMark& _onYourMark) {
    this->symbol = _symbol;

    this->maxCandleTime = 60000;
    this->perTradeLoss = _perTradeLoss;

    this->totalTrades = 0;
    this->positiveTrades = 0;
    this->totalCandles = 0;
    this->candleSum = 0;

    this->candles = std::vector<Candle>();
    this->candles.push_back(Candle(this->maxCandleTime));

    this->zigZag = new ZigZag(_zigZag);
    this->onYourMark = new OnYourMark(_onYourMark);
}

Stock::~Stock() {
    delete this->zigZag;
}

void Stock::clearData() {
    this->ticks = std::vector<Tick>();

    this->candles = std::vector<Candle>();
    this->candles.push_back(Candle(this->maxCandleTime));
}

void Stock::mockRestCall(unsigned int& _marketTime) {
    if (this->getTotalTicks() == 0) {
        return;
    }

    const Tick& tick = this->ticks[_marketTime];
    this->addTickToCandle(tick);

    this->indicators();
    this->order();
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
        this->candleSum += candle.getAveragePrice();
        this->totalCandles++;

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

    BuySell zigZag = this->zigZag->indicator(*this);
    //zigZag.isBuy = false;
    this->buySell.merge(zigZag);

    BuySell onYourMark = this->onYourMark->indicator(*this);
    this->buySell.merge(onYourMark);
}

void Stock::order() {
    if (!this->getLastCandle().isComplete()) {
        return;
    }

    float buyPrice = this->currentTick.price * 1.001;
    float sellPrice = this->currentTick.price * 0.999;
    if (this->buySell.isBuy) {
        this->buyOrder = Order(buyPrice, 1);

        if (Constants::logBuySell) {
            std::cout << "Buy  (" << this->candles.size() << "):\t" << buyPrice << std::endl;
        }
    }
    else if (this->buySell.isSell && this->buyOrder.isValid()) {
        float profit = sellPrice - this->buyOrder.price;
        if (profit > 0) {
            this->positiveTrades++;
        }

        float percentage = (profit / this->buyOrder.price) * 100;
        if (percentage <= 99) { // Split will usually increase the price > 100%. Haven't run in to a negative split
            this->profits.add(profit);
        }
        else {
            std::cout << "*!*! Split: " << percentage << " !*!*" << std::endl;
        }

        if (Constants::logBuySell) {
            std::cout << "Sell (" << this->candles.size() << "):\t" << sellPrice << std::endl;
        }

        this->buyOrder = Order();
        this->totalTrades++;
    }

    this->buySell.reset();
}

void Stock::logProfits() const {
    this->profits.log();
}

void Stock::resetDailyProfits() {
    this->profits.resetDaily();

    if (Constants::clearOrderAtDayEnd) {
        this->buyOrder = Order();
    }
}

const std::string& Stock::getSymbol() const {
    return this->symbol;
}

const unsigned int Stock::getTotalTicks() const {
    return (unsigned int)this->ticks.size();
}

const unsigned int& Stock::getTotalTrades() const {
    return this->totalTrades;
}

const unsigned int& Stock::getPositiveTrades() const {
    return this->positiveTrades;
}

const unsigned int Stock::getNegativeTrades() const {
    return this->totalTrades - this->positiveTrades;
}

const float Stock::getAveragePrice() const {
    return this->candleSum / this->totalCandles;
}

const Candle& Stock::getLastCandle() const {
    return this->candles[this->candles.size() - 1];
}

const float& Stock::getTotalProfits() const {
    return this->profits.getOverall();
}
