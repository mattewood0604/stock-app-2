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
    static bool logTicks;
    static bool logCandles;
    static bool logBuySell;
    static bool logProfits;

    static bool clearOrderAtDayEnd;

    static std::string parentDirectory;

private:
    Constants();
};

#endif /* Constants_hpp */
