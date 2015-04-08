#ifndef FAIRDBDEFERREDWIDGET_H
#define FAIRDBDEFERREDWIDGET_H

#include <Wt/WContainerWidget>


template <typename aFunction>
class FairDbDeferredWidget : public Wt::WContainerWidget
{
public:
  FairDbDeferredWidget(aFunction func)
    : fFunc(func) { }

private:
  void load() {
    addWidget(fFunc());
    WContainerWidget::load();
  }

  aFunction fFunc;
};


// Declare here global Widget deferred instance function

template <typename aFunction>
FairDbDeferredWidget<aFunction> *deferCreate(aFunction func)
{
  return new FairDbDeferredWidget<aFunction>(func);
}


#endif // FAIRDBDEFERREDWIDGET_H
