#pragma once
#ifndef macros_hxx

#define blood_is_null(x) x == 0
#define blood_is_not_null(x) != 0
#define blood_return_if(x) if (x) {return;}

#define blood_folder_separtor (BLOOD_VS || BLOOD_32 ? '\\': '/' )
#define blood_filename (strrchr(__FILE__, blood_folder_separator) ? strrchr(__FILE__, blood_folder_separator) + 1 : __FILE__)


#endif // end macros_hxx