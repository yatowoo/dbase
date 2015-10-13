#include "CbmStsQaWMainWidget.h"
#include "CbmStsQaWTreeView.h"
#include "FairDbWtTreeView.h"

#include "CbmStsQaIvChartView.h"

#include "CsvUtil.h"

#include <WImage>
#include <iostream>

using namespace std;

CbmStsQaWMainWidget::CbmStsQaWMainWidget(WContainerWidget *parent):
  FairDbWtMainWidget(parent)
{

  fSession.login().changed().connect(this, &CbmStsQaWMainWidget::onAuthEvent);
  
  Auth::AuthModel* authModel = new Auth::AuthModel(Session::auth(),
                                                  fSession.users(), this);
  authModel->addPasswordAuth(&Session::passwordAuth());
  authModel->addOAuth(Session::oAuth());

  authWidget = new Auth::AuthWidget(fSession.login());
  
  authWidget->setModel(authModel);

  // No registration
  authWidget->setRegistrationEnabled(false);
  
  
  WVBoxLayout *layout = new WVBoxLayout(this);
  layout->setDirection(Wt::WBoxLayout::TopToBottom); 
  layout->setSpacing(0);  
  
  WContainerWidget *envelop = new WContainerWidget();
  Wt::WColor c("rgba(153 , 192  ,  243 , 50)");

  // Add  Simple footer
  WContainerWidget* footer = new WContainerWidget(); 
  footer->setContentAlignment(Wt::AlignRight); 
  footer->addStyleClass("authstyle"); 
  footer->addWidget(new WText("<font size =\"1\" color=\"grey\"> powered by FairDB  &copy; 2015  </font>")); 
  
  /* Image example
  Wt::WImage *image = new WImage(Wt::WLink("pics/fairdb2.jpg"), footer);
  image->setAlternateText("FairDB logo");
  image->resize(40,18); 
  WText *out = new Wt::WText(footer);
  out->setMargin(10, Wt::Left);
  */

  //envelop->decorationStyle().setBackgroundColor(c);
  //envelop->setOverflow(OverflowAuto);
  envelop->setPadding(400);
  envelop->addStyleClass("authstyle");
  authWidget->setMinimumSize(500,500);
  authWidget->setMaximumSize(500,500);
  envelop->addWidget(authWidget);  
  layout->addWidget(envelop,1); 
  layout->addWidget(footer); 

  // Create a Main Stack
  fMainStack = new WStackedWidget();
  addWidget(fMainStack);

  // Now run the ID environment
  authWidget->processEnvironment(); 
}


 
CbmStsQaWMainWidget::~CbmStsQaWMainWidget()
{  
 cout << "~CbmStsQaWMainWidget() " << endl; 
}
 


void CbmStsQaWMainWidget::onAuthEvent()
{
  if (fSession.login().loggedIn()) {  

      const Wt::Auth::User& u = fSession.login().user();
      Wt::log("notice") 
       << "User " << u.id()
       << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
       << " (" << u.identity(Wt::Auth::Identity::LoginName) << ")"
       << " logged in.";
       
      // Show Main Widget  
     showMainWidget();
  } else {
     Wt::log("notice") << "User logged out.";
     Wt::log("notice") << "User logged out.";
     Wt::WApplication::instance()->root()->clear();
     new Wt::WText("You are logged out", Wt::WApplication::instance()->root());
     Wt::WApplication::instance()->quit();
  }
}


void CbmStsQaWMainWidget::showMainWidget()
{
 
 // Create the Sts Qa main panel
 fNavigation = new WNavigationBar(this);
 fContentstack = new WStackedWidget(this);
 

 // Define the Main Navigation bar
 fNavigation->setTitle("FairDb Instances","");
 fNavigation->setResponsive(false);

 
 WAnimation animation(WAnimation::Fade,
                       WAnimation::Linear,
                       200);
 fContentstack->setTransitionAnimation(animation, true);

 
 fMenu = new WMenu(fContentstack, NULL);
 fMenu->setInternalPathEnabled();
 fMenu->setInternalBasePath("/");
  
 // Create and Add the menu 
 addToMenu(fMenu, "Sts Qa",  new CbmStsQaWTreeView());
 //WMenuItem *item = new WMenuItem("test", new CbmStsQaWTreeView());
 //fMenu->addItem(item);
 fNavigation->addMenu(fMenu);
 
 // Release the constraint
 WContainerWidget *envelop = new WContainerWidget();
 envelop->setPadding(10);
 authWidget->setMinimumSize(WLength::Auto,WLength::Auto);
 authWidget->setMaximumSize(WLength::Auto,WLength::Auto);
 envelop->addWidget(authWidget);
 
 fNavigation->addWidget(envelop,Wt::AlignRight);
 
 // Add it inside Layout
 WVBoxLayout *layout = new WVBoxLayout(this);
 layout->setContentsMargins(0, 0, 0, 0);
 layout->addWidget(fNavigation);
 layout->addWidget(fContentstack, 1);
 
}



