/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef FAIRDBUTILTYPE
#define FAIRDBUTILTYPE

#include <string>
#include <algorithm>
#include <memory>

#ifndef __CINT__
#include <cstdlib>
#include <cxxabi.h>
#endif

namespace FairDbUtilType
{
  // Namespace definig
  // custom type manipulation
  
  std::string demangle(const char* name);
  
  template <class T>
    std::string GetTypeAsString(const T& t) {
    std::string str =  demangle(typeid(t).name());
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    return str;
  }
  
}

#endif // FairDbUtilType
