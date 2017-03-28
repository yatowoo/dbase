#include "CbmStsWtQaMainWidget.h"
#include "CbmStsWtQaBrowser.h"
#include "FairDbWtRootBrowser.h"

#include <WImage>
#include <iostream>

using namespace std;
using namespace Views;

namespace Layout {
  
  CbmStsWtQaMainWidget::CbmStsWtQaMainWidget(WContainerWidget *parent):
    FairDbWtMainWidget(parent)
  {
    
    Wt::log("debug") << "CbmStsWtQaMainWidget::ctor() ";
    
    fSession.login().changed().connect(this, &CbmStsWtQaMainWidget::onAuthEvent);
    
    // Auth Model 
    Auth::AuthModel* authModel = new Auth::AuthModel(Session::auth(), fSession.users(), this);
    authModel->addPasswordAuth(&Session::passwordAuth());
    authModel->addOAuth(Session::oAuth());
    
    // Auth Widget
    authWidget = new Auth::AuthWidget(fSession.login());
    authWidget->setModel(authModel);
    // Auth: with no registration
    authWidget->setRegistrationEnabled(false);
    
    // Layout Auth Widget
    WVBoxLayout *layout = new WVBoxLayout(this);
    layout->setDirection(Wt::WBoxLayout::TopToBottom);
    layout->setSpacing(0);
    
    WContainerWidget *envelop = new WContainerWidget();
    
    // Add Footer Widget
    WContainerWidget* footer = new WContainerWidget();
    
    footer->setContentAlignment(Wt::AlignRight);
    footer->addStyleClass("authstyle");
    footer->addWidget(new WText("<font size =\"1\" color=\"grey\"> powered by FairDB  &copy; 2015 ></font>"));
    
    envelop->addWidget(authWidget);
    layout->addWidget(envelop,1);
    layout->addWidget(footer);
    
    // Now run the ID environment
    authWidget->processEnvironment();
  }
  
  
  
  CbmStsWtQaMainWidget::~CbmStsWtQaMainWidget()
  {
    Wt::log("debug") << "CbmStsWtQaMainWidget::dtor() ";
  }
  
  
  
  void CbmStsWtQaMainWidget::onAuthEvent()
  {
    if (fSession.login().loggedIn()) {
      
      const Wt::Auth::User& u = fSession.login().user();
      Wt::log("notice")
        << "User " << u.id()
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
        << " logged in.";
      
      // Auth succeeded ...  now show the main widget
      showMainWidget();
      
    } else {
      Wt::log("notice") << "User logged out.";
      Wt::WApplication::instance()->root()->clear();
      new Wt::WText("You are logged out", Wt::WApplication::instance()->root());
      Wt::WApplication::instance()->quit();
    }
  }
  
  
  void CbmStsWtQaMainWidget::showMainWidget()
  {
    // Create the Sts Qa main panel
    fNavigation = new WNavigationBar(this);
    fContentstack = new WStackedWidget(this);
    
    // Define the Main Navigation bar
    fNavigation->setTitle("FairDb",
                          "https://github.com/denisbertini/dbase/tree/dev");
    fNavigation->setResponsive(true);
    fNavigation->addStyleClass("main-nav");
    
    WAnimation animation(WAnimation::Fade,
                         WAnimation::Linear,
                         200);
    fContentstack->setTransitionAnimation(animation, true);
    
    // Main nav. Menu
    fMenu = new WMenu(fContentstack, 0);
    fMenu->setInternalPathEnabled();
    fMenu->setInternalBasePath("/");
    
    // Create and Add the nav. menus
    addToMenu(fMenu, "Sts Qa",  new CbmStsWtQaBrowser());
    // Test for Root Browser class
    addToMenu(fMenu, "Root Browser", new FairDbWtRootBrowser());
    
    fNavigation->addMenu(fMenu);
    
    // Release the constraint
    WContainerWidget *envelop = new WContainerWidget();
    envelop->setPadding(10);
    envelop->addWidget(authWidget);
    
    fNavigation->addWidget(envelop,Wt::AlignRight);
    
    // Add it inside Layout
    WVBoxLayout *layout = new WVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(fNavigation);
    layout->addWidget(fContentstack, 1);
    // <DB> remove the default margin
    layout->setContentsMargins(0, 0, 0, 0);
  }
  
}
