#ifndef FAIRDBWTSESSION_H
#define FAIRDBWTSESSION_H

#include <vector>

#include <Auth/Login>

#include <Dbo/Session>
#include <Dbo/ptr>
#include <Dbo/backend/Sqlite3>

#include "FairDbWtUser.h"

typedef Wt::Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

class FairDbWtSession
{
public:
  static void configureAuth();

  FairDbWtSession();
  ~FairDbWtSession();

  Wt::Auth::AbstractUserDatabase& users();
  Wt::Auth::Login& login() { return fLogin; }

  std::string userName() const;
  
  static const Wt::Auth::AuthService& auth();
  static const Wt::Auth::AbstractPasswordService& passwordAuth();
  static const std::vector<const Wt::Auth::OAuthService *>& oAuth();

private:
  Wt::Dbo::backend::Sqlite3 fSqlite3;
  mutable Wt::Dbo::Session fSession;
  UserDatabase *fUsers;
  Wt::Auth::Login fLogin;

  Wt::Dbo::ptr<FairDbWtUser> user() const;
};

#endif //FAIRDBWTSESSION_H
