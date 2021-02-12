//
//  IRProvideConst.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 06.02.2021.
//

#pragma once
#include <iostream>
#include "Time.h"
#include <cmath>
#include "IRProvide.h"

namespace SiriusFM{
template<>
class IRProvider <IRModeE::Const>{
    double m_IRS[int(CcyE::JPY)];
public:
    IRProvider (char const* a_file);
    double r(CcyE a_ccy, double a_t)const{
        return m_IRS[int(a_ccy)];
    }
    double DF(CcyE a_ccy, time_t a_t0, time_t a_t1)const{ //discount factor
        double y =YearFracInt(a_t1-a_t0);
        return exp(-m_IRS[int(a_ccy)]*y);
    }
};
using IRPConst=IRProvider <IRModeE::Const>; //alias
}
