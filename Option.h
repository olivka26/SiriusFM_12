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
protected:
    AssetClassA const m_assetA;
    AssetClassB const m_assetB;
    time_t const m_expirTime;
    bool const m_isAmerican;
    bool const m_isAsian;
public:
    Option(AssetClassA a_assetA,
           AssetClassB a_assetB,
           time_t a_expirTime,
           bool a_isAmerican,
           bool a_isAsian):
    m_assetA (a_assetA),
    m_assetB (a_assetB),
    m_expirTime(a_expirTime),
    m_isAmerican(a_isAmerican),
    m_isAsian(a_isAsian){}
    AssetClassA assetA() const{
        return m_assetA;
    }
    AssetClassB assetB() const{
        return m_assetB;
    }
    time_t expirTime() const{
        return m_expirTime;
    }
    bool ifAmerican() const{
        return m_isAmerican;
    }
    bool ifAsian() const{
        return m_isAsian;
    }
    virtual double payoff(long a_L,
                          double const * a_path,
                          double const *a_ts)const=0;
    virtual ~Option(){};
};
using OptionFX = Option<CcyE, CcyE>;
}
