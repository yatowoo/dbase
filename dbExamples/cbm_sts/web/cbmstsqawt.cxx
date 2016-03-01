
#include <WApplication>
#include <WServer>
#include <WEnvironment>
#include <WHBoxLayout>
#include <WBootstrapTheme>
#include <WCssTheme>
#include <WIOService>


#include "CbmStsQaWApplication.h"
#include "CbmStsQaWMainWidget.h"
#include "Session.h"

#include <iostream>


using namespace Wt;
using namespace std;

WApplication *createApplication(const WEnvironment& env)
{

  CbmStsQaWApplication *my_app = new CbmStsQaWApplication(env);
  //Wt::WApplication * my_app = new Wt::WApplication(env);

 if (my_app->appRoot().empty()) {
    std::cerr << "!!!!!!!!!!" << std::endl
        << "!! -W- CbmStsQaWt:: ,"
        << " the Cbm Sts approot() looks suspect!" << std::endl
        << "!!!!!!!!!!" << std::endl;
  }

  // Choice of theme: defaults to bootstrap3 but can be overridden using
  // a theme parameter (for testing)


  const std::string *themePtr = env.getParameter("theme");
  std::string theme;
  if (!themePtr)
    theme = "bootstrap3";
  else
    theme = *themePtr;


  if (theme == "bootstrap3") {
    cout << "-I- CbmStsQaWApplication ----> Theme choosen: " << theme << endl;
    WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(my_app);
    bootstrapTheme->setVersion(WBootstrapTheme::Version3);
    bootstrapTheme->setResponsive(true);
    my_app->setTheme(bootstrapTheme);

   // <DB> Coment this Now CHECK ME 29/06/2015
   my_app->messageResourceBundle().use(my_app->appRoot() + "strings");
   my_app->messageResourceBundle().use(my_app->appRoot() + "templates");

   // load the default bootstrap3 (sub-)theme
   my_app->useStyleSheet("resources/themes/bootstrap/3/bootstrap.css");
   my_app->useStyleSheet("resources/themes/bootstrap/3/wt.css");
   my_app->useStyleSheet("resources/themes/bootstrap/3/wt.less");
   my_app->useStyleSheet("resources/webkit-transitions.css");
   my_app->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
   //my_app->useStyleSheet("resources/themes/bootstrap/3/custom.css");

   my_app->useStyleSheet("style/everywidget.css");
   my_app->useStyleSheet("style/dragdrop.css");
   my_app->useStyleSheet("style/combostyle.css");
   my_app->useStyleSheet("style/pygments.css");
   my_app->useStyleSheet("style/layout.css");
   my_app->useStyleSheet("style/auth.css");


  } else if (theme == "bootstrap2") {
  cout << "-I- CbmStsQaWApplication ----> Theme choosen: " << theme << endl;
    WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(my_app);
    bootstrapTheme->setResponsive(true);
    my_app->setTheme(bootstrapTheme);
  } else
    my_app->setTheme(new WCssTheme(theme));


  // load text bundles (for the tr() function)
  my_app->messageResourceBundle().use(my_app->appRoot() + "report");
  my_app->messageResourceBundle().use(my_app->appRoot() + "text");
  my_app->messageResourceBundle().use(my_app->appRoot() + "src");
  my_app->messageResourceBundle().use(my_app->appRoot() + "about");
  my_app->messageResourceBundle().use(my_app->appRoot() + "charts");
  // load Style
  my_app->useStyleSheet("style/CSSexample.css");
  my_app->useStyleSheet("style/charts.css");

  // Try Javascript
  my_app->require("scripts/JSRootCore.js");

  // Create Anchor for the Main Pane
  WHBoxLayout *layout = new WHBoxLayout(my_app->root());
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(new CbmStsQaWMainWidget(my_app->root()));

  my_app->setTitle("Cbm Sts Qa Web Interface");

  my_app->refresh();

 return my_app;
}


static bool terminating = false;

void
callRepeatedly(boost::function<void()> function, int seconds)
{

    if (!terminating) {
        Wt::WServer::instance()->ioService().schedule(
            seconds * 1000, boost::bind(callRepeatedly, function, seconds));
    }
    function();
}


string getWorkEnv(const char*  key){

  char const* env = getenv( key );
  std::string w_env( env );
  std::string s = "example";

  string::size_type i = w_env.find(s);
  if (i != string::npos)
   w_env.erase(i, s.length());

  cout << "-I- CbmStsQaWt::getWorkEnd()  Working environement is -----> " << w_env <<endl;

  return env == NULL ? string() : w_env;
}






int main(int argc, char **argv)
{

  string w_env = getWorkEnv("VMCWORKDIR" );

  if (w_env.empty()){
	cout << "-E- CbmStsQaWt quitting.... " << endl;
    cout << "    Please set the VMCWORKDIR environment using the config.sh in the fairroot build directory" << endl;
	return -1;
   }

  string str_doc = w_env + "dbase/dbWt/docroot";
  string str_app = w_env + "dbase/dbWt/approot";

   const char * const v[9] =
  {
    argv[0],
	  "--docroot", str_doc.c_str(),
	  "--approot", str_app.c_str(),
    "--http-port", "5564",
    "--http-address", "140.181.65.139"
  };

    //"--http-address", "127.0.0.1"

    try {
      WServer server(9, const_cast<char**>(v), WTHTTP_CONFIGURATION);

      server.addEntryPoint(Wt::Application, createApplication);

      // callRepeatedly(boost::bind(&Wt::WServer::expireSessions, &server), 60);
      // Wt::WServer::waitForShutdown();
      // terminating = true;
      // server.stop();

      // Authentication framework configured
      Session::configureAuth();

      server.run();

    } catch (WServer::Exception& e) {
      std::cerr << " -E- Server Exception: " <<  e.what() << std::endl;

    } catch (std::exception &e) {
      std::cerr << " -E- CbmStsQaWt:: exception: " << e.what() << std::endl;
    }

 }
