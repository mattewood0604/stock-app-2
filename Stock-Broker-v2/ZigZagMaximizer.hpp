//
//  ZigZagMaximizer.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/13/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef ZigZagMaximizer_hpp
#define ZigZagMaximizer_hpp

class ZigZagMaximize {
public:
    ZigZagMaximize();
    void log() const;

    float percentage;
    unsigned int candleOffset;
    unsigned int movingAverageLength;
    float profit;
    float duration;
    unsigned int totalTrades;
    unsigned int positiveTrades;
    unsigned int negativeTrades;
};


#endif /* ZigZagMaximizer_hpp */
