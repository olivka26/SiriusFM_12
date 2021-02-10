//
//  IRProvideConst.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 06.02.2021.
//

#pragma once
#include <cstdio>
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
};
}
