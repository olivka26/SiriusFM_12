//
//  MCOptionHedger1D.hpp
//  
//
//  Created by Рамиль Алиев on 12.02.2021.
//

#pragma once
#include "MCOptionHedger1D.h"
#include "MCEngine1D.hpp"

namespace SiriusFM{
template<typename Diffusion1D,
typename A_Provider,
typename B_Provider,
typename AssetClassA,
typename AssetClassB>
std::tuple<double, double, double, double> MCOptionHedger1D<Diffusion1D,
A_Provider,
B_Provider,
AssetClassA,
AssetClassB>::SimulateHedging(Option <AssetClassA, AssetClassB> const* a_option,
          time_t a_t0,double a_C0,DeltaFunc const* a_deltaFunc,double a_DeltaAcc,
          int a_tau_mins,
          long a_P){
    assert(a_option !=nullptr && a_tau_mins >0 && a_P>0 && a_deltaFunc!=nullptr && a_DeltaAcc>0);
    OPPathEval pathEval(a_option,&m_irpA, &m_irpB, a_C0, a_deltaFunc, a_DeltaAcc);
    m_mce.template Simulate<true>(a_t0, a_option->m_expirTime, a_tau_mins,a_P,m_useTimerSeed,m_diff,&m_irpA, &m_irpB,a_option->m_assetA, a_option->m_assetB,&pathEval);
    return athEval.GetStats();
}
}
