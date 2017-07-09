//
//  Order.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Order_hpp
#define Order_hpp

class Order {
public:
    Order();
    Order(const float& _price, const unsigned int& _quantity);

    bool isValid() const;

    float price;
    float quantity;
};

#endif /* Order_hpp */
