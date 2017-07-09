//
//  Profits.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Constants.hpp"
#include "Profits.hpp"

#include <iostream>

Profits::Profits() {
    this->daily = 0;
    this->overall = 0;
}

void Profits::reset() {
    this->daily = 0;
    this->overall = 0;
}

void Profits::resetDaily() {
    this->daily = 0;
}

void Profits::resetOverall() {
    this->overall = 0;
}

void Profits::add(const float& _profits) {
    this->daily += _profits;
    this->overall += _profits;
}

void Profits::log() const {
    if (Constants::logProfits) {
        std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
        std::cout << "Profit: " << std::endl;
        std::cout << "  Daily: " << this->daily << std::endl;
        std::cout << "  Overall: " << this->overall << std::endl;
        std::cout << "-=-=-=-=-=-=-=-=-=-=-" << std::endl;
        std::cout << std::endl;
    }
}

const float& Profits::getDaily() const {
    return this->daily;
}

const float& Profits::getOverall() const {
    return this->overall;
}

