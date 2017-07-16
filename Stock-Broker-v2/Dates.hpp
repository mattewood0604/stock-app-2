//
//  Dates.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Dates_hpp
#define Dates_hpp

#include <string>
#include <vector>

class Dates {
public:
    Dates();

    unsigned int numberOfDates() const;
    const std::string& getDate(const unsigned int& _index) const;

    static void outputCurrentTime();

private:
    std::vector<std::string> dates;

    static bool sortDates(std::string _date1, std::string _date2);

    static time_t createTime(std::string _date);
    static int getDay(std::string _date);
    static int getMonth(std::string _date);
    static int getYear(std::string _date);
};

#endif /* Dates_hpp */
