#include "FairDbWtJsRootProxy.h"
#include "FairDbWtRootCanvas.h"

#include <WCssDecorationStyle>
#include <TClass.h>
#include <TH1F.h>
#include <TString.h>
#include <TBufferJSON.h>


using namespace Wt;

FairDbWtJsRootProxy* FairDbWtJsRootProxy::fgInstance  = 0;
JSlot FairDbWtJsRootProxy::fSlot;

FairDbWtJsRootProxy::FairDbWtJsRootProxy()
{
	cout << " -I- create FairDbWtJsRootProxy" << endl;
}

FairDbWtJsRootProxy::~FairDbWtJsRootProxy()
{
  cout  << " -I- ~ Destroying ~ FairDbWtJsRootProxy" << endl;
  FairDbWtJsRootProxy::fgInstance = 0;
}


FairDbWtJsRootProxy& FairDbWtJsRootProxy::instance()
{
  if ( ! fgInstance ) {
         fgInstance = new FairDbWtJsRootProxy();
  }
  return *fgInstance;
}


WContainerWidget* FairDbWtJsRootProxy::createContainerWidget(
																				WContainerWidget* parent
                                        , TObject* obj
																				, int w, int h)
{
  if (obj == nullptr) return new WContainerWidget(parent);
	else
	return FairDbWtRootCanvas::createCanvas(obj, w, h, parent);
}
