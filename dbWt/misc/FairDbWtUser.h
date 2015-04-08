#ifndef FAIRDBWTUSER_H
#define FAIRDBWTUSER_H

#include <WDateTime>
#include <Dbo/Types>
#include <Dbo/WtSqlTraits>
#include <Auth/Dbo/AuthInfo>

#include <string>

class FairDbWtUser;
typedef Wt::Auth::Dbo::AuthInfo<FairDbWtUser> AuthInfo;
typedef Wt::Dbo::collection< Wt::Dbo::ptr<FairDbWtUser> > Users;

class FairDbWtUser : public Wt::Dbo::Dbo<FairDbWtUser>
{
public:
  FairDbWtUser();

  /* a copy of auth info's user name */
  std::string name; 
  int fNLogins;
  Wt::WDateTime fLastLogin;
  Wt::Dbo::collection< Wt::Dbo::ptr<AuthInfo> > authInfos;

  template<class Action>
  void persist(Action& a)
  {
    Wt::Dbo::field(a, fNLogins, "nboflogins");
    Wt::Dbo::field(a, fLastLogin, "lastGame");

    Wt::Dbo::hasMany(a, authInfos, Wt::Dbo::ManyToOne, "user");
  }
};

DBO_EXTERN_TEMPLATES(FairDbWtUser);

#endif // FAIRDBWTUSER_H
