//
//  ZigZag.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef ZigZag_hpp
#define ZigZag_hpp

#include "BuySell.hpp"
#include "Stock.hpp"

class Stock;

class ZigZag {
public:
    ZigZag(const unsigned int& _candleOffset, const unsigned int& _movingAverageLength);
    ZigZag(const ZigZag& _zigZag);

    BuySell indicator(const Stock& _stock);

    int lastPoint;
    unsigned int candleOffset;
    unsigned int movingAverageLength;

private:
    ZigZag();
    
    bool isValid() const;
};

#endif /* ZigZag_hpp */
