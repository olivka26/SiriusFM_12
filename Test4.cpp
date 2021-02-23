//
//  Test4.cpp
//  
//
//  Created by Рамиль Алиев on 12.02.2021.
//

#include "DiffusionGBM.h"
#include "VanillaOptions.h"
#include "MCOptionHedger1D.hpp"
#include "IRProvideConst.h"
#include "Time.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cmath>
#include "GridNOP1D_S3_RKC1.hpp"
#include "BSM.hpp"

using namespace std;
using namespace SiriusFM;

int main(int argc, const char * argv[]){
    if (argc != 10){
        cerr << "Params: mu, sigma, s0, T_days, tau_min, P,Call/Put,K" << endl;
        return 1;
    }
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double S0 = atof(argv[3]);
    const char* OptType = argv[4];
    double K = atof(argv[5]);
    long T_days = atol(argv[6]);
    double deltaAcc = atof(argv[7]);
    int tau_min = atoi(argv[8]);
    long P = atol(argv[9]);
    printf("%s\n",OptType);
    assert(sigma > 0 &&
           S0 > 0 &&
           T_days > 0 &&
           tau_min > 0 &&
           P > 0 &&
           K > 0);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::USD;
    
    char const* ratesFileA="file.txt";
    char const* ratesFileB="file.txt";
    
    bool useTimerSeed = true;
    DiffusionGBM diff(mu,sigma,S0);
    MCOptionHedger1D<decltype(diff),IRPConst,IRPConst,CcyE,CcyE>hedger(&diff,ratesFileA, ratesFileB,useTimerSeed);
    time_t t0 = time(nullptr);
    time_t T = t0 + SEC_IN_DAY * T_days;
    double TTE = YearFracInt(T - t0);
    double Ty=EPOCH_BEGIN + double(T) / AVG_SEC_IN_YEAR;
    OptionFX const * option = nullptr;
    decltype(hedger)::DeltaFunc const* deltaFunc = nullptr;
    double C0=0.0;
    double rateA=hedger.GetRateA(ccyA,0.0);
    double rateB=hedger.GetRateB(ccyB,0.0);
    cout<<"Rates"<<endl;
    function<double (double,double)> deltaCall(
[K, Ty, rateA, rateB, sigma](double a_St,double a_t)->double{ //a_t - current time not expiration
        double currTTE = Ty - a_t;
        return BSMDeltaCall(a_St,K,currTTE,rateA,rateB,sigma);
    }
    );
    function<double(double,double)> deltaPut
        (
          [K, Ty, rateA, rateB, sigma]
          (double a_St, double a_t) -> double
          {
            double currTTE = Ty - a_t;
            return BSMDeltaPut (a_St, K, currTTE, rateA, rateB, sigma);
          }
        );
    if(strcmp(OptType,"Call")==0){
        option = new CallOptionFX(ccyA, ccyB, K, T,true);
        C0=BSMPxCall(S0,K,TTE,rateA,rateB,sigma);
        deltaFunc=&deltaCall;
    }
    else if(strcmp(OptType,"Put")==0){
        option = new PutOptionFX(ccyA, ccyB, K, T,true);
        C0=BSMPxPut(S0,K,TTE,rateA,rateB,sigma);
        deltaFunc=&deltaPut;
    }
    else
        throw invalid_argument("Invalid option type");
    auto res=hedger.SimulateHedging(option,t0,C0,deltaFunc,deltaAcc,tau_min,P);
  //  OPPathEval *pathEval(option);
    
    double EPnL   = get<0>(res);
    double StDPnL = get<1>(res);
    double MinPnL = get<2>(res);
    double MaxPnL = get<3>(res);
    cout<<EPnL<<" "<<StDPnL<<" "<<MinPnL<<" "<<MaxPnL;
    delete option;
    return 0;
}

//check for rub<false>
