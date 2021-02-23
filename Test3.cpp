//
//  Test3.cpp
//  
//
//  Created by Рамиль Алиев on 11.02.2021.
//

#include "DiffusionGBM.h"
#include "VanillaOptions.h"
#include "MCOptionPricer1D.hpp"
#include "IRProvide.h"
#include "IRProvideConst.h"
#include "Time.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <cmath>
using namespace std;
using namespace SiriusFM;

int main(int argc, const char * argv[]){
    if (argc != 9){
        cerr << "Params: mu, sigma, s0, T_days, tau_min, P,Call/Put,K" << endl;
        return 1;
    }
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double S0 = atof(argv[3]);
    const char* OptType = argv[4];
    double K = atof(argv[5]);
    long expirTime = atol(argv[6]);
    int tau_min = atoi(argv[7]);
    long P = atol(argv[8]);
    printf("%s\n",OptType);
    assert(sigma > 0 &&
           S0 > 0 &&
           expirTime > 0 &&
           tau_min > 0 &&
           P > 0 &&
           K > 0);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::USD;
    
    bool useTimerSeed = true;
    DiffusionGBM diff(mu, sigma,S0);
    MCOptionPricer1D<decltype(diff),IRPConst,IRPConst,CcyE,CcyE>Pricer(&diff,"file.txt","file.txt",useTimerSeed);
    time_t t0 = time(nullptr);
    time_t T = t0 + SEC_IN_DAY * expirTime;
    OptionFX const * option = nullptr;
    if(strcmp(OptType,"Call")==0)
        option = new CallOptionFX(ccyA, ccyB, K,T,true);
    else if(strcmp(OptType,"Put")==0)
        option = new PutOptionFX(ccyA, ccyB, K, T,true);
    else
        throw invalid_argument("Invalid option type");
  //  OPPathEval *pathEval(option);
    double px=Pricer.Px(option,t0,tau_min,P);
    delete option;
    cout<<px;
    return 0;
}
