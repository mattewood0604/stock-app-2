//
//  Market.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/16/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Market_hpp
#define Market_hpp

#include "MarketInfo.hpp"

class Market {
public:
    Market();

    void waitForNextOpening() const;
    void waitForMinute() const;

    bool isOpen() const;

private:
    bool closed;
    bool open;

    bool justOpened;
    bool justClosed;

    void outputTime(struct tm* _now) const;
};

#endif /* Market_hpp */
