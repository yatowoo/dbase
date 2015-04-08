#include "FairDbWtUser.h"

#include <Auth/Dbo/AuthInfo>
#include <Dbo/Impl>

DBO_INSTANTIATE_TEMPLATES(FairDbWtUser);

using namespace Wt;
using namespace Wt::Dbo;

FairDbWtUser::FairDbWtUser()
  : fNLogins(0)
{ 

}
