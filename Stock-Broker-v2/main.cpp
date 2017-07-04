//
//  main.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Dates.hpp"
#include "FileManager.hpp"
#include "Stock.hpp"

#include <iostream>

void mockRestCall(Stock& _stock, unsigned int& _marketTime) {
    if (_stock.totalTicks() == 0) {
        return;
    }

    const Tick& tick = _stock.getTick(_marketTime);
    _stock.addTickToCandle(tick);

    _stock.indicators();
    _stock.order();
}

int main(int argc, const char * argv[]) {
    Dates dates;
    Stock stock("JNUG", Stock::ZigZag(20, 10));

    for (unsigned int i = 0; i < dates.dates.size(); i++) {
        stock.clearData();

        std::cout << dates.dates[i] << std::endl;
        std::cout << "------------" << std::endl;
        FileManager::readTicks(stock, dates.dates[i]);

        for (unsigned int j = 0; j < stock.totalTicks(); j++) {
            mockRestCall(stock, j);
        }

        stock.logProfits();
        stock.resetProfits();
    }

    std::cout << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << "Days: " << dates.dates.size() << std::endl;
    std::cout << "Profits: " << stock.getTotalProfits() << std::endl;
    std::cout << "-+-+-+-+-+-+-+-+-+" << std::endl;
    std::cout << std::endl;

    return 0;
}
