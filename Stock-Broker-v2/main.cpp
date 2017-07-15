//
//  main.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "FileManager.hpp"
#include "Maximizer.hpp"
#include "RestCall.hpp"
#include "Stock.hpp"
#include "ZigZag.hpp"

#include <iostream>
#include <future>

void runProfits(Stock& _stock) {
    Dates dates;

    for (unsigned int i = 0; i < dates.dates.size(); i++) {
        _stock.clearData();

        std::cout << dates.dates[i] << std::endl;
        std::cout << "------------" << std::endl;
        FileManager::readTicks(_stock, dates.dates[i]);

        for (unsigned int j = 0; j < _stock.getTotalTicks(); j++) {
            _stock.mockRestCall(j);
        }

        _stock.logProfits();
        _stock.resetDailyProfits();
    }

    std::cout << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << "Days: " << dates.dates.size() << std::endl;
    std::cout << "Profits: " << _stock.getTotalProfits() << std::endl;
    std::cout << "Average Price: " << _stock.getAveragePrice() << std::endl;
    std::cout << "Total Trades: " << _stock.getTotalTrades() << std::endl;
    std::cout << "Positive Trades: " << _stock.getPositiveTrades() << std::endl;
    std::cout << "Negative Trades: " << _stock.getNegativeTrades() << std::endl;
    std::cout << "Trades / Day: " << (float)_stock.getTotalTrades() / dates.dates.size() << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << std::endl;
}

// int argc, const char * argv[]
int main() {
    Stock jnug("JNUG", 0.01, ZigZag(72, 34), OnYourMark(0, 0, 0, 0));
    Stock jdst("JDST", 0.01, ZigZag(26, 47), OnYourMark(0, 0, 0, 0));
    Stock dust("DUST", 0.01, ZigZag(21, 51), OnYourMark(0, 0, 0, 0));

    FileManager::initForWriting();

    RestCall restCall;
    restCall.quotes();
    // restCall.instruments();

    // Maximizer maximizer("JNUG");
    // maximizer.maximizeZigZag();
    // maximizer.maximizeOnYourMark();

    //runProfits(jdst);

    return 0;
}
