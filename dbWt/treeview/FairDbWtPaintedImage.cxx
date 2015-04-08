#include "FairDbWtPaintedImage.h"




void FairDbWtPaintedImage::paintEvent(Wt::WPaintDevice *paintDevice) {
    
    // Init Painter
    Wt::WPainter painter(paintDevice);

    // Create an Image
    Wt::WPainter::Image image("test_graph.png",1000,1000);
    painter.drawImage(0.0, 0.0, image);

}
