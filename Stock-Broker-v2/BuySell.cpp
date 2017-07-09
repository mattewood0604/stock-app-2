//
//  BuySell.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "BuySell.hpp"

BuySell::BuySell() {
    this->isBuy = false;
    this->isSell = false;
};

void BuySell::merge(const BuySell& _buySell) {
    this->isBuy = this->isBuy || _buySell.isBuy;
    this->isSell = this->isSell || _buySell.isSell;
};

void BuySell::reset() {
    this->isBuy = false;
    this->isSell = false;
};
