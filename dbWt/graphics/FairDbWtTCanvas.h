#ifndef FAIRDBWTTCANVAS
#define FAIRDBWTTCANVAS

#include <Wt/WContainerWidget>
#include "TCanvas.h"

using namespace Wt;

class FairDbWtTCanvasWidget;

class FairDbWtTCanvas : public WContainerWidget
{
public:
  FairDbWtTCanvas(WContainerWidget *root, TCanvas *aCanvas=nullptr, bool showTitle=true);

private:
  void confirmed();
  void setAmount(std::string amount);

};

#endif // FAIRDBWTTCANVAS
