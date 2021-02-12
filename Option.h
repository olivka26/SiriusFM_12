//
//  Option.h
//  
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <stdexcept>
#include "IRProvide.h"

namespace SiriusFM {
template<typename AssetClassA, typename AssetClassB>
class Option{
public:
    AssetClassA const m_assetA;
    AssetClassB const m_assetB;
    bool const m_isAmerican;
    time_t const m_expirTime;
    Option(AssetClassA a_assetA,AssetClassB a_assetB,time_t a_expirTime, bool a_isAmerican):
    m_assetA (a_assetA),
    m_assetB (a_assetB),
    m_isAmerican(a_isAmerican),
    m_expirTime(a_expirTime){}
    virtual double payoff(long a_L,
                          double const * a_path,
                          double const *a_ts)const=0;
    bool IsAmerican()const{return m_isAmerican;}
    virtual ~Option(){};
};
using OptionFX = Option<CcyE, CcyE>;
}
