//
//  RestCall.hpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/14/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#ifndef RestCall_hpp
#define RestCall_hpp

#include <curl/curl.h>

#include "Response.hpp"

class RestCall {
public:
    RestCall();

    void waitForNextCall() const;

    MarketInfo getInfoForToday();

    std::vector<Tick> quotes();

    void instruments();

private:
    static const std::string accountUrl;

    CURL* quotesHandle;
    CURL* authenticationHandle;
    CURL* availableCashHandle;

    Response response;

    std::string authenticationToken;

    void initializeQuotesHandle();
    void initializeAuthenticationHandle();
    void initializeAvailableCashHandle();

    void logFailureData() const;

    void authenticate();

    unsigned int getVolumeForStockSymbol(const std::string &_symbol);

    static size_t WriteMemoryCallback(void* _contents, size_t _size, size_t _nmemb, void* _userp);
};

#endif /* RestCall_hpp */
