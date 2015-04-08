#include <Wt/WContainerWidget>
#include <Wt/WGridLayout>
#include <Wt/WSlider>
#include <Wt/WText>

#include "FairDbWtTCanvas.h"
#include "FairDbWtTCanvasWidget.h"

using namespace Wt;

FairDbWtTCanvas::FairDbWtTCanvas(WContainerWidget *root, TCanvas* gCanvas, bool showTitle)
  : WContainerWidget(root)
{
  std::string text;
  if (showTitle)
    text += "<h2>TGraph example</h2>";
    text += "<p>A simple example demonstrating TGraph in vector graphics. </p>";
  

  new WText(text, this);

  WContainerWidget *emweb = new WContainerWidget(this);
  emweb->setMargin(WLength::Auto, Left | Right);

  WGridLayout *layout = new WGridLayout();
  emweb->setLayout(layout);

  WSlider *scaleSlider = new WSlider(Horizontal);
  scaleSlider->setMinimum(0);
  scaleSlider->setMaximum(20);
  scaleSlider->setValue(10);
  scaleSlider->setTickInterval(5);
  scaleSlider->setTickPosition(WSlider::TicksBothSides);
  scaleSlider->resize(300, 50);
  scaleSlider->sliderMoved().connect(this, &FairDbWtTCanvas::scaleShape);

  layout->addWidget(scaleSlider, 0, 1, AlignCenter | AlignMiddle);

/*
  WSlider *rotateSlider = new WSlider(Vertical);
  rotateSlider->setMinimum(-30);
  rotateSlider->setMaximum(30);
  rotateSlider->setValue(0);
  rotateSlider->setTickInterval(10);
  rotateSlider->setTickPosition(WSlider::TicksBothSides);
  rotateSlider->resize(50, 400);
  rotateSlider->sliderMoved().connect(this, &FairDbWtTCanvas::rotateShape);

  layout->addWidget(rotateSlider, 1, 0, AlignCenter | AlignMiddle);
  */

  shapes_ = new FairDbWtTCanvasWidget(this,gCanvas);
  shapes_->setAngle(0.0);
  shapes_->setRelativeSize(0.5);
  shapes_->setPreferredMethod(WPaintedWidget::HtmlCanvas);

  layout->addWidget(shapes_, 1, 1, AlignCenter | AlignMiddle);


}

void FairDbWtTCanvas::rotateShape(int v)
{
  shapes_->setAngle(v / 2.0);

  // Being silly: test alternate rendering method
  // shapes_->setPreferredMethod(v < 0 ? WPaintedWidget::InlineSvgVml
	//		      : WPaintedWidget::HtmlCanvas);
}

void FairDbWtTCanvas::scaleShape(int v)
{
  shapes_->setRelativeSize(0.1 + 0.9 * (v/20.0));
}
