//
//  main.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "FileManager.hpp"
#include "Market.hpp"
#include "Maximizer.hpp"
#include "RestCall.hpp"
#include "Stock.hpp"

#include <iostream>
#include <future>

void runProfits(Stock& _stock) {
    FileManager fileManager;
    Dates dates;

    for (unsigned int i = 0; i < dates.numberOfDates(); i++) {
        _stock.clearData();

        std::cout << dates.getDate(i) << std::endl;
        std::cout << "------------" << std::endl;
        fileManager.readTicks(_stock, dates.getDate(i));

        for (unsigned int j = 0; j < _stock.getTotalTicks(); j++) {
            _stock.mockRestCall(j);
        }

        _stock.logProfits();
        _stock.resetDailyProfits();
    }
}

void logProfits(const Stock& _stock) {
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

void runStocks() {
    Market market;
    RestCall restCall;

    std::cout << "~-~-~Application Start Up~-~-~" << std::endl;

    // RunProfits
    // Load up all of the stocks that I want to buy
    // Run all of the profits of them to set up what should happen
    // Make sure the previous days candles are still saved

    market.waitForNextOpening();

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
            restCall.waitForNextCall();
        }

        std::cout << "Closed: ";
        Dates::outputCurrentTime();

        market.waitForNextOpening();
    }
}

// int argc, const char * argv[]
int main() {
    Stock jnug("JNUG", 0.01, ZigZag(72, 34), OnYourMark(0, 0, 0, 0));
    Stock jdst("JDST", 0.01, ZigZag(26, 47), OnYourMark(0, 0, 0, 0));
    Stock dust("DUST", 0.01, ZigZag(21, 51), OnYourMark(0, 0, 0, 0));

    

    // runStocks();
    // FileManager::initForWriting();

    // RestCall restCall;
    // restCall.quotes();
    // restCall.instruments();

    // Maximizer maximizer("JNUG");
    // maximizer.maximizeZigZag();
    // maximizer.maximizeOnYourMark();

    runProfits(jnug);
    logProfits(jnug);

    return 0;
}
