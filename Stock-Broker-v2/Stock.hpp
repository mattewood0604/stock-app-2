//
//  Stock.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Stock_hpp
#define Stock_hpp

#include <string>
#include <vector>

#include "BuySell.hpp"
#include "Candle.hpp"
#include "OnYourMark.hpp"
#include "Order.hpp"
#include "Profits.hpp"
#include "Tick.hpp"
#include "ZigZag.hpp"

class OnYourMark;
class ZigZag;

class Stock {
public:
    Stock(const std::string& _symbol, const float& _perTradeLoss, const ZigZag& _zigZag, const OnYourMark& _onYourMark);
    ~Stock();

    void clearData();

    void mockRestCall(unsigned int& _marketTime);

    void addTick(const Tick& _tick);

    void indicators();
    void order();

    void logProfits() const;
    void resetDailyProfits();

    const std::string& getSymbol() const;

    const Candle& getLastCandle() const;
    unsigned int getTotalTicks() const;

    const unsigned int& getTotalTrades() const;
    const unsigned int& getPositiveTrades() const;
    unsigned int getNegativeTrades() const;
    const float& getTotalProfits() const;
    float getAveragePrice() const;

private:
    friend class OnYourMark;
    friend class ZigZag;

    Stock();
    
    ZigZag* zigZag;
    OnYourMark* onYourMark;

    void addTickToCandle(const Tick& _tick);

    std::string symbol;
    Tick currentTick;

    std::vector<Candle> candles;
    std::vector<Tick> ticks;

    unsigned int maxCandleTime;
    float perTradeLoss;

    unsigned int totalTrades;
    unsigned int positiveTrades;
    unsigned int totalCandles;
    float candleSum;

    BuySell buySell;
    Order buyOrder;
    Profits profits;
};


#endif /* Stock_hpp */
