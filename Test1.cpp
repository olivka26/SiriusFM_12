//
//  main.cpp
//  SiriusFM
//
//  Created by Рамиль Алиев on 05.02.2021.
//

#include <iostream>
#include "DiffusionGBM.h"
#include "MCEngine1D.hpp"
#include "IRProvideConst.h"
using namespace  SiriusFM;
using namespace std;

int main(int argc, const char * argv[]) {
    if (argc != 7){
        cerr << "Params: mu, sigma, s0, T_days, tau_min, P" << endl;
        return 1;
    }
    double mu = atof(argv[1]);
    double sigma = atof(argv[2]);
    double s0 = atof(argv[3]);
    long T_days = atol(argv[4]);
    int tau_min = atoi(argv[5]);
    long P = atol(argv[6]);
    CcyE ccyA = CcyE::USD;
    CcyE ccyB = CcyE::USD;
    IRPConst irp ("file.txt");
    DiffusionGBM diff(mu, sigma,s0);
    MCEngine1D <DiffusionGBM, decltype(irp), decltype(irp),CcyE,CcyE,OPPathEval>mce(20'000, 20'000);
        time_t t0 = time(NULL);
        time_t T  = t0 + T_days * 86400;
        double Ty = double(T_days) / 365.25;
        cout << Ty << endl;
        mce.Simulate <false>(t0, T, tau_min, P, true, &diff, &irp, &irp,ccyA, ccyA);
        auto res = mce.Get_paths();
        long L1  = get<0>(res);
        long P1  = get<1>(res);
        double const * paths = get<2>(res);
        // E of log(ST)
        double EST  = 0.0;
        double EST2 = 0.0;
        int    NVP  = 0;   // Valid paths
        for (long p = 0; p < P1; ++p){
            double const * path = paths + p * L1;
            double ST = path[L1 - 1];
            // In practice may be ST <= 0
            if (ST <= 0.0)
                continue;
            ++NVP;
            double RT = log(ST/s0);
            EST  += RT;
            EST2 += RT * RT;
        }
    	cout<<NVP<<endl;
        assert(NVP > 1);
        EST /= double(NVP); // (mu - sigma^2 / 2) * T
        // Now find Variance of ST
        double VarST = (EST2 - double(NVP) * EST * EST) / double(NVP - 1);
                                                            // sigma^2 * T
        double sigma2E = VarST / Ty;
        double muE = (EST + VarST / 2.0) / Ty;
        cout << "mu = " << mu << ", muE = " << muE << endl;
        cout << "sigma2 = " << sigma * sigma;
        cout << ", sigma2E = " << sigma2E << endl;
        return 0;
    return 0;
}
