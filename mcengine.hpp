//
//  mcengine.hpp
//  SiriusFM
//
//  Created by Рамиль Алиев on 08.02.2021.
//

#pragma once
#include <stdio.h>
#include <cmath>
#include <stdexcept>
#include <ctime>
#include "IRProvideConst.h"
#include "diffusion.h"

namespace SiriusFM {
template<typename Diffusion1D, typename AssetClassA, typename AssetClassB,typename A_Provider, typename B_Provider>
class MCEngine1D{
    long const m_MaxL;
    long const m_MaxP;
    double* m_paths;
    long m_L; // m_L<=m_MaxL
    long m_P; // m_P<=m_maxP;
    double m_t; //timestep or yearfraction
    double m_t0;
    Diffusion1D const* m_diff;
    A_Provider const* m_rateA;
    B_Provider const* m_rateB;
    //IRProvider const* m_irp; //interstraight parameter
    //DRProvider const* m_drp;
    AssetClassA m_a;
    AssetClassB m_b;
    bool m_RN;
public:
    MCEngine1D(long a_MaxL, long a_MaxP)
    :m_MaxL(a_MaxL),
    m_MaxP(a_MaxP),
    m_paths(new double[m_MaxL * m_MaxP]),
    m_L(0),
    m_P(0),
    m_t(NAN),
    m_t0(NAN),
    m_diff(nullptr),
    m_rateA(nullptr),
    m_rateB(nullptr),
    m_a(AssetClassA::UNDEFINED),
    m_b(AssetClassB::UNDEFINED),
    m_RN(false){
        if(m_MaxL <=0 || m_MaxP <=0)
            throw std::invalid_argument("One of maxima is less than 0");
        /*
        const double* x1;
        double const* x2; Object constant, pointer no
        double* const x3; Pointer constant, object no
        const double* const x4; Object and pointer are constant
        double const* const x5; Object and pointer are constant
        */
        
    }
    //current and expiration time respectively
    void Simulate(time_t a_t0, time_t a_T, int a_cur_min, Diffusion1D const* a_diff, A_Provider a_rateA, B_Provider a_rateB, AssetClassA a_A,  AssetClassB a_B, bool a_isRN);
};
}
