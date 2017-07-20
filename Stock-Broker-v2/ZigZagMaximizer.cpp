//
//  ZigZagMaximizer.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/13/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "ZigZagMaximizer.hpp"

#include <string>
#include <iostream>

ZigZagMaximize::ZigZagMaximize() {
    this->percentage = 0;
    this->candleOffset = 0;
    this->movingAverageLength = 0;
    this->output = "";
}

void ZigZagMaximize::addOutput() {
    this->output.append("-=-=-=-=-=-=-=-=-=-=-=-=-");
    this->output.append("\n");
    this->output.append("Percentage: ");
    this->output.append(std::to_string(this->percentage));
    this->output.append("\n");
    this->output.append("Candle Offset: ");
    this->output.append(std::to_string(this->candleOffset));
    this->output.append("\n");
    this->output.append("Moving Average Length: ");
    this->output.append(std::to_string(this->movingAverageLength));
    this->output.append("\n");
    this->output.append("Profit: ");
    this->output.append(std::to_string(this->profit));
    this->output.append("\n");
    this->output.append("Total Trades: ");
    this->output.append(std::to_string(this->totalTrades));
    this->output.append("\n");
    this->output.append("Positive Trades: ");
    this->output.append(std::to_string(this->positiveTrades));
    this->output.append("\n");
    this->output.append("Negative Trades: ");
    this->output.append(std::to_string(this->negativeTrades));
    this->output.append("\n");
    this->output.append("-=-=-=-=-=-=-=-=-=-=-=-=-");
    this->output.append("\n");
}

void ZigZagMaximize::clearOutput() {
    this->output = "";
}

void ZigZagMaximize::log() const {
    std::cout << this->output;
}
