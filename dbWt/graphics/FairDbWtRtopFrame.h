#ifndef FAIRDBWTRTOPFRAME
#define FAIRDBWTRTOPFRAME

#include <Wt/WContainerWidget>
#include "FairDbWtRootBrowser.h"
using namespace Wt;


class FairDbWtRtopFrame : public WContainerWidget
{
public:
  FairDbWtRtopFrame(FairDbWtRootBrowser* browser, WContainerWidget *parent);

  // Drops
  virtual void dropEvent(WDropEvent event);

private:
  FairDbWtRootBrowser* fBrowser;
};

#endif // FAIRDBWTRTOPFRAME
