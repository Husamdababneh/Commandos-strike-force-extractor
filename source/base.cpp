/* ========================================================================
   $File: base.cpp
   $Date: 2025-03-03
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/


#include "base.h" 


function
f32 abs_f32(f32 x) {
    union{f32 f; u32 u;} r;
    r.f  = x;
    r.u &= SignFlipper32;
    return r.f;
}


function
f64 abs_f64(f64 x) {
    union{f64 f; u64 u;} r;
    r.f  = x;
    r.u &= SignFlipper64;
    return r.f;
}

//~ Strings
function
String8 operator""_s8(const char* str, size_t length) {
    String8 result;
    result.str  = (u8*)str;
    result.size = length;
    return result;
}

function
String
//~ Misc
function
String8 stringFromMonth(Month month) {
    switch (month) {
      case Month_January:   return "January"_s8;
      case Month_February:  return "February"_s8;
      case Month_March:     return "March"_s8;
      case Month_April:     return "April"_s8;
      case Month_May:       return "May"_s8;
      case Month_June:      return "June"_s8;
      case Month_July:      return "July"_s8;
      case Month_August:    return "August"_s8;
      case Month_September: return "September"_s8;
      case Month_October:   return "October"_s8;
      case Month_November:  return "November"_s8;
      case Month_December:  return "December"_s8;
    }    
    return "(null)"_s8;
}

function
String8 stringFromWeekDay(WeekDay weekday) {
    switch (weekday) {
      case WeekDay_Sunday:    return "Sunday"_s8;
      case WeekDay_Monday:    return "Monday"_s8;
      case WeekDay_Tuesday:   return "Tuesday"_s8;
      case WeekDay_Wednesday: return "Wednesday"_s8;
      case WeekDay_Thursday:  return "Thursday"_s8;
      case WeekDay_Friday:    return "Friday"_s8;  
      case WeekDay_Saturday:  return "Saturday"_s8;
    }    
    return "(null)"_s8;
}
