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

#include "Candle.hpp"
#include "Tick.hpp"

class Stock {
private:
    class BuySell {
    public:
        BuySell() {
            this->isBuy = false;
            this->isSell = false;
        };

        void merge(const BuySell& _buySell) {
            this->isBuy = this->isBuy || _buySell.isBuy;
            this->isSell = this->isSell || _buySell.isSell;
        };

        void reset() {
            this->isBuy = false;
            this->isSell = false;
        };

        bool isBuy;
        bool isSell;
    };

    class Order {
    public:
        Order() {
            this->price = 0;
            this->quantity = 0;
        };

        Order(const float& _price, const unsigned int& _quantity) {
            this->price = _price;
            this->quantity = _quantity;
        };

        bool isValid() const {
            return this->price && this->quantity;
        }

        float price;
        float quantity;
    };
    
public:
    class ZigZag {
    public:
        ZigZag(const unsigned int& _candleOffset, const unsigned int& _movingAverageLength);

        BuySell indicator(const Stock& _stock);

        int lastPoint;
        unsigned int candleOffset;
        unsigned int movingAverageLength;
    };

    Stock(const std::string& _symbol, const ZigZag& _zigZag);

    void clearData();

    void addTick(const Tick& _tick);
    void addTickToCandle(const Tick& _tick);

    void indicators();
    void order();

    void logProfits() const;
    void resetProfits();

    const std::string& getSymbol() const;

    const Tick& getTick(const unsigned int& _time) const;
    const unsigned int totalTicks() const;

    const Candle& getLastCandle() const;

    const float& getTotalProfits() const;
//    Stock();
//    Stock(const std::string& _symbol);
//
//    void addTimeToCandles(TimeQuote& _timeQuote);
//    void addQuoteToTestData(TimeQuote& _timeQuote);
//    TimeQuote& getTestQuote(const unsigned int& _marketTime);
//
//    Candle& getLastCandle();
//    unsigned int getNumberOfCandles() const;
//    const Candle& getCandleAtIndex(const unsigned int& _index) const;
//
//    StockModel& getStockModel();
//
//    //void buyOrSell();
//
//    float ema(const float& _price, const float& _previousEMA, const float& _multiplier) const;
//
//    void setWaveTrendComplete(const bool& _complete);
//
//    void logMoneyMade() const;
//    float getPercentageMade() const;
//
//    const float& getShortMultiplier() const;
//    const float& getLongMultiplier() const;
//
//    std::string instrumentUrl;
//
//    TimeQuote currentQuote;
//
//    float averagePriceEMA;
//    float apESA;
//    unsigned int apESACalculated;
//    float ci;
//    unsigned int ciCalculated;
//    std::vector<float> previousW1;
//
//    float w1;
//    float w2;
//
//    std::vector<float> previousKST;
//    std::vector<float> previousMACDSignals;
//
//    float kst;
//    float kstSignalLine;
//
//    unsigned int numberOfTrades;
//    unsigned int negativeTrades;
//
//    unsigned int numberOfQuotes;
//
//    bool canStartBuying;
//
//    unsigned int buyCandleNumber;
//
//    bool macdComplete;
//    bool kstComplete;
//
//    float moneyMade;
//
//    bool isReady;
//    bool isSet;
//    bool isGo;

private:
    Stock();
    
    ZigZag zigZag;
    void enhancedInvestor();

    std::string symbol;
    Tick currentTick;

    std::vector<Candle> candles;
    std::vector<Tick> ticks;

    unsigned int maxCandleTime;
    float perTradeLoss;

    BuySell buySell;
    Order buyOrder;

    float dailyProfits;
    float totalProfits;

//    friend class BuySell;
//    friend class IndicatorAlgorithms;
//
//    void supertrendAlgorithm();
//    float averageTrueRange() const;
//
//    bool waveTrendComplete;
//    bool williamsVixValid;
//    bool isBought;
//    bool isBuy;
//    bool isSell;
//
//    float buyPrice;
//    float shortMultiplier;
//    float longMultiplier;
//
//    float maxMade;
//    float percentageMade;
//    bool maxLossTaken;
//
//    StockModel stockModel;
//
};


#endif /* Stock_hpp */
