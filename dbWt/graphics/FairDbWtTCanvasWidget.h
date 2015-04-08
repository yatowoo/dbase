#ifndef FAIRDBWTTCANVASWIDGET_H
#define FAIRDBWTTCANVASWIDGET_H



#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

#include <Wt/WPaintedWidget>
#include <Wt/Chart/WCartesianChart>   

#include <Chart/WCartesianChart>
#include <Wt/WAbstractItemModel>

#include <Wt/WStandardItemModel>
#include <Wt/WStandardItem>
#include <Wt/WString>
#include <Wt/WText>


#include "TCanvas.h"
#include "TGraph.h"


using namespace Wt;
using namespace Wt::Chart;


namespace Wt {
  class WPainter;
}



class NumericItem : public WStandardItem {
      public:

        virtual NumericItem *clone() const {
          return new NumericItem();
        }
    
        virtual void setData(const boost::any &data, int role = UserRole) {
          boost::any dt;
    
          if (role == EditRole) {
           std::string s = Wt::asString(data).toUTF8();
            char *endptr;
            double d = strtod(s.c_str(), &endptr);
            if (*endptr == 0)
              dt = boost::any(d);
            else
              dt = data;
          }
    
          WStandardItem::setData(data, role);
        
        }
};






class FairDbWtTCanvasWidget : public WPaintedWidget
{
public:
  FairDbWtTCanvasWidget(WContainerWidget *parent = 0, TCanvas* gcanvas = 0);

  void setAngle(double angle);
  double angle() const { return angle_; }

  void setRelativeSize(double size);
  double relativeSize() const { return size_; }

protected:
  virtual void paintEvent(WPaintDevice *paintDevice);

private:
  double angle_;
  double size_;
  TCanvas* fCanvas;


  void drawTCanvas(WPainter& p);
  void drawTGraph(WPainter& p, TGraph* aGraph);

  void drawEmwebLogo(WPainter& p);
  void drawEmwebE(WPainter& p);
  void drawEmwebMW(WPainter& p);
};

#endif // 
