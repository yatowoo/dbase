#ifndef FAIRDBWTTCANVAS_H
#define FAIRDBWTTCANVAS_H

#include <Wt/WContainerWidget>
#include "TCanvas.h"

using namespace Wt;

class FairDbWtTCanvasWidget;

class FairDbWtTCanvas : public WContainerWidget
{
public:
  FairDbWtTCanvas(WContainerWidget *root, TCanvas *gCanvas, bool showTitle=true);

private:
  FairDbWtTCanvasWidget *shapes_;

  void rotateShape(int v);
  void scaleShape(int v);

};

#endif // FAIRDBWTTCANVAS_H
