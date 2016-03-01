#ifndef FAIRDBWTJSROOTPROXY
#define FAIRDBWTJSROOTPROXY

#include <WObject>
#include <WString>
#include <WJavaScript>
#include <WContainerWidget>

#include <TObject.h>

#include <iostream>

using namespace std;
using namespace Wt;

class FairDbWtJsRootProxy
{
public:
    static  FairDbWtJsRootProxy& instance();
    static  bool isActive() {return fgInstance ? true: false;}

    static WContainerWidget* createContainerWidget(
                                  WContainerWidget* parent=nullptr,
                                  TObject* obj=nullptr
                                  ,int w=800
                                  ,int h=800);

    static WContainerWidget* refreshContainerWidget(
                                  WContainerWidget* cont,
                                  TObject* obj=nullptr);




private:
   FairDbWtJsRootProxy();
   virtual ~FairDbWtJsRootProxy();

   FairDbWtJsRootProxy(const FairDbWtJsRootProxy&);
   FairDbWtJsRootProxy& operator=(const FairDbWtJsRootProxy&);
   static FairDbWtJsRootProxy* fgInstance;

   static JSlot fSlot;

};
#endif
