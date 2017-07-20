//
//  MarketInfo.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/16/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef MarketInfo_hpp
#define MarketInfo_hpp

#include <string>

class MarketInfo {
public:
    MarketInfo();

	time_t todayAsTime() const;
    time_t nextOpenDayAsTime() const;

    bool opensToday;
    std::string nextOpenDay;

private:
    int getDay() const;
    int getMonth() const;
    int getYear() const;
};

#endif /* MarketInfo_hpp */
