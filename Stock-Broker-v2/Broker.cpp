//
//  Broker.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/20/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include <iostream>

#include "Broker.hpp"
#include "Dates.hpp"
#include "FileManager.hpp"
#include "Market.hpp"
#include "RestCall.hpp"

Broker::Broker() {

}

Broker::~Broker() {
    for (unsigned int i = 0; i < this->stocks.size(); i++) {
        if (this->stocks[i]) {
            delete this->stocks[i];
        }
    }
}

void Broker::run() {
    Market market;
    RestCall restCall;

    this->initializeForRun();

    std::cout << "~-~-~Application Start Up~-~-~" << std::endl;

    MarketInfo marketInfo = restCall.getInfoForToday();
    market.waitForNextOpening(marketInfo);

    while(true) {
        FileManager fileManager;
        fileManager.initForWriting();
        std::vector<Tick> ticks;

        bool justOpened = true;
        while (market.isOpen()) {
            if (justOpened) {
                std::cout << "Opened: ";
                Dates::outputCurrentTime();
                justOpened = false;
            }

            ticks = restCall.quotes();
            fileManager.writeTicks(ticks);
            // Add ticks to the stocks we want to buy or sell
            // Each time a new candle is added to the end make sure to pop one off the front
            this->addTicksToStocks(ticks);

            restCall.waitForNextCall();
        }

        std::cout << "Closed: ";
        Dates::outputCurrentTime();

        marketInfo = restCall.getInfoForToday();
        market.waitForNextOpening(marketInfo);
    }
}

void Broker::initializeForRun() {
    Stock* jnug = new Stock("JNUG", 0.01, ZigZag(72, 34), OnYourMark(0, 0, 0, 0));
    Stock* jdst = new Stock("JDST", 0.01, ZigZag(26, 47), OnYourMark(0, 0, 0, 0));
    Stock* dust = new Stock("DUST", 0.01, ZigZag(21, 51), OnYourMark(0, 0, 0, 0));

    this->stocks = std::vector<Stock*>();
    stocks.push_back(jnug);
    stocks.push_back(jdst);
    stocks.push_back(dust);

    Dates dates;
    FileManager fileManager;
    for (unsigned int i = 0; i < this->stocks.size(); i++) {
        Stock& stock = *this->stocks[i];
        for (unsigned int i = 0; i < dates.numberOfDates(); i++) {
            stock.clearData();

            fileManager.readTicks(stock, dates.getDate(i));

            for (unsigned int j = 0; j < stock.getTotalTicks(); j++) {
                stock.mockRestCall(j);
            }

            stock.resetDailyProfits();
        }
    }
}

void Broker::addTicksToStocks(const std::vector<Tick>& _ticks) {
    for (unsigned int i = 0; i < this->stocks.size(); i++) {
        for (unsigned int j = 0; j < _ticks.size(); j++) {
            if (_ticks[j].getSymbol().compare(this->stocks[i]->getSymbol()) == 0) {
                //std::cout << _ticks[j].getSymbol() << " == " << this->stocks[i]->getSymbol() << std::endl;
            }
        }
    }
}

void Broker::test() {
    Stock stock("JNUG", 0.01, ZigZag(72, 34), OnYourMark(0, 0, 0, 0));
    // Stock* jdst = new Stock("JDST", 0.01, ZigZag(26, 47), OnYourMark(0, 0, 0, 0));
    // Stock* dust = new Stock("DUST", 0.01, ZigZag(21, 51), OnYourMark(0, 0, 0, 0));

    FileManager fileManager;
    Dates dates;

    for (unsigned int i = 0; i < dates.numberOfDates(); i++) {
        stock.clearData();

        std::cout << dates.getDate(i) << std::endl;
        std::cout << "------------" << std::endl;
        fileManager.readTicks(stock, dates.getDate(i));

        for (unsigned int j = 0; j < stock.getTotalTicks(); j++) {
            stock.mockRestCall(j);
        }

        stock.logProfits();
        stock.resetDailyProfits();
    }

    this->logProfits(stock);
}

void Broker::logProfits(const Stock& _stock) {
    Dates dates;

    std::cout << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << "Days: " << dates.numberOfDates() << std::endl;
    std::cout << "Profits: " << _stock.getTotalProfits() << std::endl;
    std::cout << "Average Price: " << _stock.getAveragePrice() << std::endl;
    std::cout << "Total Trades: " << _stock.getTotalTrades() << std::endl;
    std::cout << "Positive Trades: " << _stock.getPositiveTrades() << std::endl;
    std::cout << "Negative Trades: " << _stock.getNegativeTrades() << std::endl;
    std::cout << "Trades / Day: " << (float)_stock.getTotalTrades() / dates.numberOfDates() << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << std::endl;
}
