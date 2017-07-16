//
//  Response.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/14/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef Response_hpp
#define Response_hpp

#include <string>
#include <vector>

#include "MarketInfo.hpp"
#include "Tick.hpp"

class Response {
public:
    Response();

    void clear();
    void log() const;

    void recreate(void* _contents, size_t& _size);

    std::string parseAuthentication() const;
    std::vector<Tick> parseQuotes() const;

    std::string nextUrlForInstruments() const;
    std::vector<std::string> symbolsFromInstruments() const;
    unsigned int parseAverageVolume() const;

    MarketInfo parseMarketInfo() const;

private:
    char* memory;
    size_t size;
};

#endif /* Response_hpp */
