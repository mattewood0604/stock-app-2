//
//  RestCall.cpp
//  Stock-Broker-v2
//
//  Created by Matt Wood on 7/14/17.
//  Copyright © 2017 MattWood. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "Constants.hpp"
#include "RestCall.hpp"

const std::string RestCall::accountUrl = "https://api.robinhood.com/accounts/5QY77902/";

RestCall::RestCall() {
    this->initializeQuotesHandle();
    this->initializeAuthenticationHandle();

    this->authenticate();
    this->initializeAvailableCashHandle();
}

void RestCall::waitForNextCall() const {
    sleep(2);
}

void RestCall::initializeQuotesHandle() {
    quotesHandle = curl_easy_init();

    std::string quotesURL = "https://api.robinhood.com/quotes/?symbols=";
    quotesURL.append(Constants::stocksForQuotesAsCsv);

    curl_easy_setopt(quotesHandle, CURLOPT_URL, quotesURL.c_str());

    curl_easy_setopt(quotesHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(quotesHandle, CURLOPT_WRITEDATA, (void*)&response);
}

void RestCall::initializeAuthenticationHandle() {
    authenticationHandle = curl_easy_init();

    curl_easy_setopt(authenticationHandle, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(authenticationHandle, CURLOPT_URL, "https://api.robinhood.com/api-token-auth/");

    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Accept: application/json");
    curl_easy_setopt(authenticationHandle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(authenticationHandle, CURLOPT_POSTFIELDS, "username=mattewood&password=1OrangeHippoWithAn!");
    curl_easy_setopt(authenticationHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(authenticationHandle, CURLOPT_WRITEDATA, (void*)&response);
}

void RestCall::initializeAvailableCashHandle() {
    availableCashHandle = curl_easy_init();

    curl_easy_setopt(availableCashHandle, CURLOPT_URL, accountUrl.c_str());

    struct curl_slist* headers = NULL;
    std::string authorization = "Authorization: Token ";
    if (!authenticationToken.size()) {
        std::cout << "Error: Authentication Token NULL in RestCall::initializeAvailableCashHandle" << std::endl;
        return;
    }

    authorization.append(authenticationToken);
    headers = curl_slist_append(headers, authorization.c_str());
    curl_easy_setopt(availableCashHandle, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(availableCashHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(availableCashHandle, CURLOPT_WRITEDATA, (void*)&response);
}

void RestCall::authenticate() {
    CURLcode returnCode = curl_easy_perform(authenticationHandle);
    if (returnCode != CURLE_OK) {
        fprintf(stderr, "RestCall::authenticate: curl_easy_perform() failed: %s\n", curl_easy_strerror(returnCode));
        this->logFailureData();
    }
    else {
        response.log();
    }

    this->authenticationToken = response.parseAuthentication();
    this->response.clear();
}

std::vector<Tick> RestCall::quotes() {
    CURLcode ret = curl_easy_perform(quotesHandle);
    if(ret != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(ret));
        this->logFailureData();
    }
    else {
        response.log();
    }

    std::vector<Tick> ticks = this->response.parseQuotes();
    this->response.clear();
    return ticks;
}


void RestCall::instruments() {
    CURL* instrumentsHandle = curl_easy_init();

    std::string instrumentsURL = "https://api.robinhood.com/instruments/";

    curl_easy_setopt(instrumentsHandle, CURLOPT_URL, instrumentsURL.c_str());

    curl_easy_setopt(instrumentsHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(instrumentsHandle, CURLOPT_WRITEDATA, (void*)&response);

    CURLcode returnCode = curl_easy_perform(instrumentsHandle);
    if (returnCode != CURLE_OK) {
        fprintf(stderr, "instruments: curl_easy_perform() failed: %s\n", curl_easy_strerror(returnCode));
        this->logFailureData();
    }

    unsigned int total = 0;
    unsigned int i = 1;
    std::string nextUrl = response.nextUrlForInstruments();
    bool foundSymbol = false;
    while(nextUrl.size() > 0) {
        nextUrl.erase(std::remove(nextUrl.begin(), nextUrl.end(), '\\'), nextUrl.end());
        //std::cout << "\n" << nextUrl << "\n" << std::endl;

        std::vector<std::string> foundSymbols = response.symbolsFromInstruments();
        for (unsigned int i = 0; i < foundSymbols.size(); i++) {
            //if (foundSymbols[i].compare("JBL") == 0) {
                foundSymbol = true;
            //}

            if (foundSymbol) {
                unsigned int volume = RestCall::getVolumeForStockSymbol(foundSymbols[i]);
                if (volume >= 1000000) {
                    std::cout << foundSymbols[i] << ": " << volume << std::endl;
                }
            }
        }
        //total += allSymbols.size();

        response.clear();

        //instrumentsHande = curl_easy_init();

        curl_easy_reset(instrumentsHandle);
        curl_easy_setopt(instrumentsHandle, CURLOPT_URL, nextUrl.c_str());
        curl_easy_setopt(instrumentsHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
        curl_easy_setopt(instrumentsHandle, CURLOPT_WRITEDATA, (void*)&response);

        //curl_easy_setopt(instrumentsHande, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
        //curl_easy_setopt(instrumentsHande, CURLOPT_WRITEDATA, (void*)&response);

        returnCode = curl_easy_perform(instrumentsHandle);
        if (returnCode != CURLE_OK) {
            fprintf(stderr, "instruments: curl_easy_perform() failed: %s\n", curl_easy_strerror(returnCode));
            this->logFailureData();
        }
        
        nextUrl = response.nextUrlForInstruments();
        i++;
    }
    
    std::cout << i << "  " << total << std::endl;
}

unsigned int RestCall::getVolumeForStockSymbol(const std::string &_symbol) {
    CURL* stockFundamentalsHandle = curl_easy_init();

    std::string stockFundamentalsUrl = "https://api.robinhood.com/fundamentals/";
    stockFundamentalsUrl.append(_symbol);
    stockFundamentalsUrl.append("/");

    curl_easy_setopt(stockFundamentalsHandle, CURLOPT_URL, stockFundamentalsUrl.c_str());

    curl_easy_setopt(stockFundamentalsHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(stockFundamentalsHandle, CURLOPT_WRITEDATA, (void*)&response);

    CURLcode returnCode = curl_easy_perform(stockFundamentalsHandle);
    if (returnCode != CURLE_OK) {
        fprintf(stderr, "getVolumeForStockSymbol %s: %s\n curl_easy_perform() failed: %s\n", _symbol.c_str(), stockFundamentalsUrl.c_str(), curl_easy_strerror(returnCode));
        this->logFailureData();
        return 0;
    }

    unsigned int volume = response.parseAverageVolume();
    response.clear();
    return volume;
}

MarketInfo RestCall::getInfoForToday() {
    std::string datesUrl = "https://api.robinhood.com/markets/XNYS/hours/";

    time_t currentTime = time(0);
    struct tm* now = localtime(&currentTime);

    int year = now->tm_year + 1900;
    datesUrl.append(std::to_string(year));
    datesUrl.append("-");

    int month = now->tm_mon + 1;
    if (month < 10) {
        datesUrl.append("0");
        datesUrl.append(std::to_string(month));
    }
    else {
        datesUrl.append(std::to_string(month));
    }
    datesUrl.append("-");

    int day = now->tm_mday;
    if (day < 10) {
        datesUrl.append("0");
        datesUrl.append(std::to_string(day));
    }
    else {
        datesUrl.append(std::to_string(day));
    }
    datesUrl.append("/");

    std::cout << datesUrl << std::endl;

    CURL* allDatesHandle = curl_easy_init();
    curl_easy_setopt(allDatesHandle, CURLOPT_WRITEFUNCTION, RestCall::WriteMemoryCallback);
    curl_easy_setopt(allDatesHandle, CURLOPT_WRITEDATA, (void*)&response);
    curl_easy_setopt(allDatesHandle, CURLOPT_URL, datesUrl.c_str());
    CURLcode returnCode = curl_easy_perform(allDatesHandle);
    if (returnCode != CURLE_OK) {
        fprintf(stderr, "RestCall::getInfoForToday: curl_easy_perform() failed: %s\n", curl_easy_strerror(returnCode));
        this->logFailureData();
    }

    MarketInfo marketInfo = response.parseMarketInfo();
    std::cout << marketInfo.nextOpenDay << std::endl;
    std::cout << marketInfo.opensToday << std::endl;
    response.clear();

    return marketInfo;
}

size_t RestCall::WriteMemoryCallback(void* _contents, size_t _size, size_t _nmemb, void* _userp) {
    size_t realsize = _size * _nmemb;
    Response* response = (Response*)_userp;
    response->recreate(_contents, realsize);
    return realsize;
}

void RestCall::logFailureData() const {
    char* data;
    curl_easy_getinfo(authenticationHandle, CURLINFO_EFFECTIVE_URL, &data);
    std::cout << "Last Url: " << data << std::endl;
    curl_easy_getinfo(authenticationHandle, CURLINFO_RESPONSE_CODE, &data);
    std::cout << "Response Code: " << data << std::endl;
}