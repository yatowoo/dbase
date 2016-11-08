#ifndef FAIRDBFLOATXTRAP_H
#define FAIRDBFLOATXTRAP_H
#ifndef __CINT__

#if defined(linux)
#include <fpu_control.h>
#include <fenv.h>
#endif

struct FairDbFloatXtrap
{
  FairDbFloatXtrap(bool activate);
  ~FairDbFloatXtrap();

#if defined(linux)
#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
  fenv_t old_setting;
#else
  fpu_control_t old_setting;
#endif
#endif
  bool activate_;
  
  static bool validate();
};

#endif // __CINT__
#endif // FAIRDBFLOATXTRAP_H
