//
//  Order.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include "Order.hpp"

Order::Order() {
    this->price = 0;
    this->quantity = 0;
}

Order::Order(const float& _price, const unsigned int& _quantity) {
    this->price = _price;
    this->quantity = _quantity;
}

bool Order::isValid() const {
    return this->price && this->quantity;
}
