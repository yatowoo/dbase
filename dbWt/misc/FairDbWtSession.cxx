#include "FairDbWtSession.h"

#include "Auth/AuthService"
#include "Auth/HashFunction"
#include "Auth/PasswordService"
#include "Auth/PasswordStrengthValidator"
#include "Auth/PasswordVerifier"
#include "Auth/GoogleService"
#include "Auth/Dbo/AuthInfo"
#include "Auth/Dbo/UserDatabase"

#include <WApplication>
#include <WLogger>

#include <unistd.h>


using namespace Wt;
namespace dbo = Wt::Dbo;

namespace 
{

 class UnixCryptHashFunction : public Auth::HashFunction
  {
  public:
    virtual std::string compute(const std::string& msg, 
				const std::string& salt) const
    {
      std::string md5Salt = "$1$" + salt;
      return crypt(msg.c_str(), md5Salt.c_str());
    }

    virtual bool verify(const std::string& msg,
			const std::string& salt,
			const std::string& hash) const
    {
      return crypt(msg.c_str(), hash.c_str()) == hash;
    }

    virtual std::string name () const {
       return "crypt";
    }
  };


 class MyOAuth : public std::vector<const Auth::OAuthService *>
  {
  public:
    ~MyOAuth()
    {
      for (unsigned i = 0; i < size(); ++i)
	         delete (*this)[i];
    }
  };

   Auth::AuthService myAuthService;
   Auth::PasswordService myPasswordService(myAuthService);
   MyOAuth myOAuthServices;

} // ! namespace

void FairDbWtSession::configureAuth()
{
  myAuthService.setAuthTokensEnabled(true, "fairdbcookie");
  myAuthService.setEmailVerificationEnabled(true);

  Auth::PasswordVerifier *verifier = new Auth::PasswordVerifier();
  verifier->addHashFunction(new Auth::BCryptHashFunction(7));
  verifier->addHashFunction(new UnixCryptHashFunction());


  myPasswordService.setVerifier(verifier);
  myPasswordService.setStrengthValidator(new Auth::PasswordStrengthValidator());
  myPasswordService.setAttemptThrottlingEnabled(true);

  if (Auth::GoogleService::configured())
    myOAuthServices.push_back(new Auth::GoogleService(myAuthService));
}

FairDbWtSession::FairDbWtSession()
  : fSqlite3(WApplication::instance()->appRoot() + "fairdbwt.db")
{
  fSession.setConnection(fSqlite3);
  fSqlite3.setProperty("show-queries", "true");

  fSession.mapClass<FairDbWtUser>("user");
  fSession.mapClass<AuthInfo>("auth_info");
  fSession.mapClass<AuthInfo::AuthIdentityType>("auth_identity");
  fSession.mapClass<AuthInfo::AuthTokenType>("auth_token");

  fUsers = new UserDatabase(fSession);

  dbo::Transaction transaction(fSession);
  try {
    fSession.createTables();

    /*
     * Add a default guest/guest account
     */
    Auth::User guestUser = fUsers->registerNew();
    guestUser.addIdentity(Auth::Identity::LoginName, "guest");
    myPasswordService.updatePassword(guestUser, "guest");

    Wt::log("info") << "Database created";
  } catch (...) {
    Wt::log("info") << "Using existing database";
  }

  transaction.commit();
}

FairDbWtSession::~FairDbWtSession()
{
  delete fUsers;
}

dbo::ptr<FairDbWtUser> FairDbWtSession::user() const
{
  if (fLogin.loggedIn()) {
    dbo::ptr<AuthInfo> authInfo = fUsers->find(fLogin.user());
    dbo::ptr<FairDbWtUser> user = authInfo->user();

    if (!user) {
      user = fSession.add(new FairDbWtUser());
      authInfo.modify()->setUser(user);
    }

    return user;
  } else
    return dbo::ptr<FairDbWtUser>();
}

std::string FairDbWtSession::userName() const
{
  if (fLogin.loggedIn())
    return fLogin.user().identity(Auth::Identity::LoginName).toUTF8();
  else
    return std::string();
}


Auth::AbstractUserDatabase& FairDbWtSession::users()
{
  return *fUsers;
}

const Auth::AuthService& FairDbWtSession::auth()
{
  return myAuthService;
}

const Auth::AbstractPasswordService& FairDbWtSession::passwordAuth()
{
  return myPasswordService;
}

const std::vector<const Auth::OAuthService *>& FairDbWtSession::oAuth()
{
  return myOAuthServices;
}
