//
//  BuySell.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef BuySell_hpp
#define BuySell_hpp

#include <stdio.h>

class BuySell {
public:
    BuySell();
    
    void merge(const BuySell& _buySell);
    void reset();

    bool isBuy;
    bool isSell;
};

#endif /* BuySell_hpp */
