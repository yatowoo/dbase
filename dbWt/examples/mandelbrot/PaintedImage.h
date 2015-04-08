#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>

class PaintedImage : public Wt::WPaintedWidget
{
public:
    PaintedImage(Wt::WContainerWidget *parent = 0)
    : Wt::WPaintedWidget(parent)
    {
    setLayoutSizeAware(true);
    resize(1000, 1000);  // Provide a default size.
    }

protected:
    void paintEvent(Wt::WPaintDevice *paintDevice);
    
};