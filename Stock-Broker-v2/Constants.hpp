//
//  Constants.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Constants_hpp
#define Constants_hpp

#include <string>

class Constants {
public:
    static const bool logTicks;
    static const bool logCandles;
    static const bool logBuySell;
    static const bool logProfits;
    static const bool logResponse;

    static const bool clearOrderAtDayEnd;

    static const std::string readDirectory;
    static const std::string writeDirectory;

private:
    Constants();
};

#endif /* Constants_hpp */
