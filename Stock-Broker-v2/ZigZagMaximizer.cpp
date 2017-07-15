//
//  ZigZagMaximizer.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/13/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "ZigZagMaximizer.hpp"

#include <iostream>

ZigZagMaximize::ZigZagMaximize() {
    this->percentage = 0;
    this->candleOffset = 0;
    this->movingAverageLength = 0;
};

void ZigZagMaximize::log() const {
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
    std::cout << "Percentage: " << this->percentage << std::endl;
    std::cout << "Candle Offset: " << this->candleOffset << std::endl;
    std::cout << "Moving Average Length: " << this->movingAverageLength << std::endl;
    std::cout << "Profit: " << this->profit << std::endl;
    std::cout << "Total Trades: " << this->totalTrades << std::endl;
    std::cout << "Positive Trades: " << this->positiveTrades << std::endl;
    std::cout << "Negative Trades: " << this->negativeTrades << std::endl;
    std::cout << "-=-=-=-=-=-=-=-=-=-=-=-=-" << std::endl;
}
