
#include <WApplication>
#include <WServer>
#include <WEnvironment>
#include <WHBoxLayout>
#include <WBootstrapTheme>
#include <WCssTheme>


#include "FairDbWtApplication.h"
#include "FairDbWtMainWidget.h"

#include "TObject.h"

using namespace Wt;

WApplication *createApplication(const WEnvironment& env)
{
  
  FairDbWtApplication *my_app = new FairDbWtApplication(env); 

 if (my_app->appRoot().empty()) {
    std::cerr << "!!!!!!!!!!" << std::endl
        << "!! -W-,"
        << " the my_approot looks suspect!" << std::endl
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
    WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(my_app);
    bootstrapTheme->setVersion(WBootstrapTheme::Version3);
    bootstrapTheme->setResponsive(true);
    my_app->setTheme(bootstrapTheme);

    // load the default bootstrap3 (sub-)theme
    my_app->useStyleSheet("resources/themes/bootstrap/3/bootstrap-theme.min.css");
  } else if (theme == "bootstrap2") {
    WBootstrapTheme *bootstrapTheme = new WBootstrapTheme(my_app);
    bootstrapTheme->setResponsive(true);
    my_app->setTheme(bootstrapTheme);
  } else
    my_app->setTheme(new WCssTheme(theme));


  // load text bundles (for the tr() function)
  my_app->messageResourceBundle().use(my_app->appRoot() + "report");
  my_app->messageResourceBundle().use(my_app->appRoot() + "text");
  my_app->messageResourceBundle().use(my_app->appRoot() + "src");
 
  WHBoxLayout *layout = new WHBoxLayout(my_app->root());
  layout->setContentsMargins(0, 0, 0, 0);


  layout->addWidget(new FairDbWtMainWidget());

  my_app->setTitle("FairDb Connections");

  my_app->useStyleSheet("style/everywidget.css");
  my_app->useStyleSheet("style/dragdrop.css");
  my_app->useStyleSheet("style/combostyle.css");
  my_app->useStyleSheet("style/pygments.css");
  my_app->useStyleSheet("style/layout.css");
  my_app->useStyleSheet("styles.css");
  my_app->messageResourceBundle().use(WApplication::appRoot() + "about");

 my_app->refresh();
  

 return my_app;
}







int main(int argc, char **argv)
{


   const char * const v[9] =
  {
    argv[0],
    "--docroot", "/Users/denis/fairroot/fairbase/dbase/dbWt/docroot",
    "--approot", "/Users/denis/fairroot/fairbase/dbase/dbWt/approot",
    "--http-port", "5564",
    "--http-address", "127.0.0.1"
  };

    try {
      WServer server(9, const_cast<char**>(v), WTHTTP_CONFIGURATION);

      server.addEntryPoint(Application, createApplication);

      server.run();

    } catch (WServer::Exception& e) {
      std::cerr << e.what() << std::endl;

    } catch (std::exception &e) {
      std::cerr << "-E- exception: " << e.what() << std::endl;
    } 

 }

