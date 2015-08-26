#ifndef FAIRDBWTAPPLICATION_H
#define FAIRDBWTAPPLICATION_H


#include <WApplication>
#include <WBreak>
#include <WContainerWidget>
#include <WLineEdit>
#include <WPushButton>
#include <WText>


using namespace Wt;


class FairDbWtApplication : public WApplication
{

public:
FairDbWtApplication(const WEnvironment& env);

protected:
virtual void  initialise();


};

#endif