//
//  MCEngine1D.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 10.02.2021.
//

#pragma once
#include <cmath>
#include <stdexcept>
#include <ctime>
#include <tuple>

namespace SiriusFM {
template<typename Diffusion1D, typename A_Provider, typename B_Provider, typename AssetClassA, typename AssetClassB>
class MCEngine1D{
    long const m_MaxL;
    long const m_MaxP;
    double* m_paths;
    long m_L; // m_L<=m_MaxL
    long m_P; // m_P<=m_maxP;
    /*
    double m_t; //timestep or yearfraction
    double m_t0;
    long a_P;
    Diffusion1D const* m_diff;
    A_Provider const* m_rateA;
    B_Provider const* m_rateB;
    //IRProvider const* m_irp; //interstraight parameter
    //DRProvider const* m_drp;
    AssetClassA m_a;
    AssetClassB m_b;
    bool m_RN;
     */
public:
    MCEngine1D(long a_MaxL, long a_MaxP)
    :m_MaxL(a_MaxL),
    m_MaxP(a_MaxP),
    m_paths(new double[m_MaxL * m_MaxP]),
    m_L(0),
    m_P(0)
    /*
    m_t(NAN),
    m_t0(NAN),
    m_diff(nullptr),
    m_rateA(nullptr),
    m_rateB(nullptr),
    m_a(AssetClassA::UNDEFINED),
    m_b(AssetClassB::UNDEFINED),
    m_RN(false)
     */{
         if(m_MaxL <=0 || m_MaxP <=0)
            throw std::invalid_argument("One of maxima is less than 0");
         for (long p = 0; p < m_MaxP; ++p){
             for(long l = 0; l < m_MaxL; ++l)
             m_paths[p*m_MaxL + l] = 0;
         }
        /*
        const double* x1;
        double const* x2; Object constant, pointer no
        double* const x3; Pointer constant, object no
        const double* const x4; Object and pointer are constant
        double const* const x5; Object and pointer are constant
        */
    }
    std::tuple <long, long, double const *> Get_paths () const
         {
             return (m_L <= 0 || m_P <= 0) ? std::make_tuple(0, 0, nullptr)
                                                   : std::make_tuple(m_L, m_P, m_paths);
         }
    ~MCEngine1D(){
        delete[] m_paths;
    }
    template <bool IsRN>
    void Simulate
    (time_t a_t0, time_t a_T, int a_tau_min, double a_S0, long a_P,Diffusion1D const* a_diff, A_Provider const *a_rateA, B_Provider const* a_rateB, AssetClassA a_A,  AssetClassB a_B);
    MCEngine1D (const MCEngine1D &) = delete;
    void operator = (const MCEngine1D &) = delete;
};
}
