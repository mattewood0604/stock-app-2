//
//  Tick.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/2/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Tick_hpp
#define Tick_hpp

#include <string>

class Tick {
public:
    enum FROM { JSON, CSV };

    Tick();
    Tick(const std::string& _json, const std::string& _symbol = "", FROM _from = JSON);

    void log() const;
    std::string toCSV() const;

    const std::string& getSymbol() const;

    std::string json;

    uint64_t dayTime;
    float price;

private:
    std::string symbol;
    uint16_t bidQty;
    float bid;
    uint16_t offerQty;
    float offer;

    static std::string bidKey;
    static std::string bidQtyKey;
    static std::string offerKey;
    static std::string offerQtyKey;
    static std::string timeKey;
    static std::string priceKey;
    static std::string symbolKey;

    void fromCSV(const std::string& _symbol, const std::string& _quote);

    int parseInt(const std::string& _key) const;
    float parseFloat(const std::string& _key) const;
    std::string parseString(const std::string& _key) const;
    uint64_t parseTime() const;
};

#endif /* Tick_hpp */
