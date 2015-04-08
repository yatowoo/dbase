#ifndef FAIRDBWTAPP_H
#define FAIRDBWTAPP_H


#include <WApplication>
#include <WBreak>
#include <WContainerWidget>
#include <WLineEdit>
#include <WPushButton>
#include <WText>


using namespace Wt;

class FairDbPopup;

class FairDbWtApplication : public WApplication
{
public:
  FairDbWtApplication(const WEnvironment& env);
  
  void InitJS();


  void textChanged(std::string incoming);
  void TestJS1();
  void f(const std::string &msg); 
  void greet();
  
private:
  void confirmed();
  void setAmount(std::string amount);
  FairDbPopup *promptAmount_;
  FairDbPopup *confirmPay_;



  WText *currentAmount_;
  WLineEdit *nameEdit_;
  WText *greeting_;
  
};

#endif