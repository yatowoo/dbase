#ifndef FAIRDBWTROOTCANVAS
#define FAIRDBWTROOTCANVAS

#include <Wt/WObject>
#include <Wt/WContainerWidget>
#include <Wt/WString>
#include <Wt/WJavaScript>

#include <TObject.h>

using namespace Wt;


class FairDbWtRootCanvas : public WContainerWidget
{
public:

  static FairDbWtRootCanvas* createCanvas(TObject *obj,
                                      int w=800, int h=800,
                                      WContainerWidget* parent = 0 );

  virtual void layoutSizeChanged(int w, int h);
  //virtual void dropEvent(WDropEvent event);

  JSlot fShow;
  JSignal<std::string>& okPressed() { return okPressed_; }
  JSignal<void>&        cancelPressed() { return cancelPressed_; }

private:
  enum Type { TH1, TCanvas, TGraph, TTree, TProfile, TGeoVolume};

  FairDbWtRootCanvas(Type t, TObject* obj, int w, int h,
                     WContainerWidget *parent);

  JSignal<std::string> okPressed_;
  JSignal<void>        cancelPressed_;

  Type fType;
  TObject* fObject;
  int fW;
  int fH;

  void setJavaScript();

};
#endif
