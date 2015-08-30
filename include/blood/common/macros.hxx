#pragma once
#ifndef macros_hxx
#include "encoding.hxx"

#define blood_is_null(x) x == 0
#define blood_is_not_null(x) != 0
#define blood_return_if(x) if (x) {return;}

#if defined(BLOOD_VS) || defined(BLOOD_32)
#define blood_folder_separator '\\'       
#else
#define blood_folder_separtor '/'       
#endif

#define blood_filename (strrchr(__FILE__, blood_folder_separator) ? strrchr(__FILE__, blood_folder_separator) + 1 : __FILE__)
#define blood_code_line blood::fn::text::string_format("%s[%d]", blood_filename, __LINE__) 


#endif // end macros_hxx