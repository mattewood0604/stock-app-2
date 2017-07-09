//
//  OnYourMark.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/4/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef OnYourMark_hpp
#define OnYourMark_hpp

#include "Stock.hpp"

class Stock;

class OnYourMark {
public:
    OnYourMark(int _crossPeriods, int _kijunPeriods, int _overKijunPeriods, float _readySignalPercent);
    OnYourMark(const OnYourMark& _onYourMark);

    BuySell indicator(Stock& _stock);

    unsigned int crossPeriods;
    unsigned int kijunPeriods;
    unsigned int overKijunPeriods;
    float readySignalPercent;

private:
    OnYourMark();

    bool isValid() const;

    bool determineSignal(Stock& _stock, unsigned int offset = 0);
    bool determineKijunCross(Stock& _stock);
    bool determineOverKijun(Stock& _stock);

    unsigned int readyCandle;
    unsigned int setCandle;

    bool isReady;
    bool isSet;
};

#endif /* OnYourMark_hpp */
