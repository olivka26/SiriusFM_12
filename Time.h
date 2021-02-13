//
//  Time.h
//  SiriusFM
//
//  Created by Рамиль Алиев on 08.02.2021.
//

#pragma once

namespace  SiriusFM{
constexpr int SEC_IN_MIN = 60;
constexpr int SEC_IN_DAY = 86400;
constexpr double AVG_DAYS_IN_YEAR = 365.25;
constexpr double AVG_SEC_IN_YEAR  = AVG_DAYS_IN_YEAR * SEC_IN_DAY;
constexpr double EPOCH_BEGIN = 1970.0;
inline double YearFrac(time_t a_t){
    constexpr double SecY = 365.25*86400.0;
    return 1970.0+double(a_t)/SecY;
}
inline double YearFracInt(time_t a_t){
    constexpr double SecY = 365.25*86400.0;
    return double(a_t)/SecY;
}
}
