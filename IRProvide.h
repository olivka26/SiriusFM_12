//
//  IRProvide.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 06.02.2021.
//

#pragma once
#include <iostream>
#include <cstring>
#include <stdexcept>

namespace SiriusFM{
enum class CcyE{
    UNDEFINED=-1,
    USD=0,
    EUR=1,
    GBP=2,
    CHF=3,
    RUB=4,
    JPY=5,
};
inline char const* CcyE2Str(CcyE a_ccy){
    switch (a_ccy) {
        case CcyE::USD:
            return "USD";
        case CcyE::EUR:
            return "EUR";
        case CcyE::GBP:
            return "GBP";
        case CcyE::CHF:
            return "CHF";
        case CcyE::RUB:
            return "RUB";
        case CcyE::JPY:
            return "JPY";
        default:
            throw std::invalid_argument("Invalid Ccy");
    }
}
inline CcyE Str2CcyE(char const* a_str){
    if(strcmp(a_str,"USD")==0)
        return CcyE::USD;
    if(strcmp(a_str,"RUB")==0)
        return CcyE::RUB;
    if(strcmp(a_str,"EUR")==0)
        return CcyE::EUR;
    if(strcmp(a_str,"GBP")==0)
        return CcyE::GBP;
    if(strcmp(a_str,"JPY")==0)
        return CcyE::JPY;
    if(strcmp(a_str,"CHF")==0)
        return CcyE::CHF;
    throw std::invalid_argument("Invalid Ccy name");
}
enum class IRModeE{
    Const=0,
    FwdCurve=1,
    Stock=2
};
template <IRModeE IRM>
class IRProvider;
}
