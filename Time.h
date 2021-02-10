//
//  Time.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 08.02.2021.
//

#pragma once
#include <stdio.h>
#include <ctime>

namespace  SiriusFM{
}
inline double YearFrac(time_t a_t){
    constexpr double SecY = 365.25*86400.0;
    return 1970.0+double(a_t)/SecY;
}
