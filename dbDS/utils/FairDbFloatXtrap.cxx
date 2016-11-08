#include "FairDbFloatXtrap.h"
#include <iostream>

#if defined(__APPLE__)
#include <fenv.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void macosx_fpe_handler( int a )
{
  std::cerr <<  "Floating point exception! (" 
       	    << strsignal(a)
	    << ") Aborting!" << std::endl;
  exit(1);
}

#define fegetenvd(x) asm volatile("mffs %0" : "=f" (x));
#define fesetenvd(x) asm volatile("mtfsf 255,%0" : : "f" (x));
enum {
 FE_ENABLE_INEXACT    = 0x00000008,
 FE_ENABLE_DIVBYZERO  = 0x00000010,
 FE_ENABLE_UNDERFLOW  = 0x00000020,
 FE_ENABLE_OVERFLOW   = 0x00000040,
 FE_ENABLE_INVALID    = 0x00000080,
 FE_ENABLE_ALL_EXCEPT = 0x000000F8
};
typedef union {
 struct {
  unsigned long hi;
  unsigned long lo;
 } i;
 double d;
} hexdouble;


#endif

FairDbFloatXtrap::FairDbFloatXtrap(bool a):activate_(a) 
{
  if(activate_==true)
  {
    // check for:
    //                 - invalid operand
    //                 - division by zero
    //                 - overflow exceptions

#if defined(linux)    
#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
    // fenv
    fenv_t newval;
    fegetenv(&old_setting);
    fegetenv(&newval);
    newval.__control_word &= ~(FE_INVALID|FE_DIVBYZERO|FE_OVERFLOW);
    fesetenv(&newval);
#else
    // fpu control 
    old_setting = __fpu_control;
    fpu_control_t newval =
      old_setting & ~(
		      _FPU_MASK_IM
		      |_FPU_MASK_ZM
		      |_FPU_MASK_OM
		      );
    
    _FPU_SETCW(newval);
#endif
#endif

#if defined(__APPLE__) 
#if defined(__i386__) 
  // Intel:
#elif defined(__x86_64__)
  // 64bit macosx 
#else 
  //
  hexdouble t;
  fegetenvd(t.d);                                
  t.i.lo |=  FE_ENABLE_DIVBYZERO | FE_ENABLE_OVERFLOW | FE_ENABLE_INVALID;;
  fesetenvd(t.d);
  signal(SIGFPE,macosx_fpe_handler);
#endif
#endif

  }
}


FairDbFloatXtrap::~FairDbFloatXtrap()
{
#if defined(linux)
if(activate_==true)
#if defined(__GLIBC__)&&(__GLIBC__>2 || __GLIBC__==2 && __GLIBC_MINOR__>=1)
  fesetenv(&old_setting);
#else
_FPU_SETCW(old_setting);
#endif
#endif
}

