#include "CbmStsWtQaViewsUtils.h"

namespace Views{
  
  WPushButton *createColorButton(const char *className,
                                 const WString& text)
  {
    WPushButton *button = new WPushButton();
    button->setTextFormat(Wt::XHTMLText);
    button->setText(text);
    button->addStyleClass(className);
    return button;
  }
  
}
