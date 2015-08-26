#ifndef CBMSTSQAIVPANELLIST_H
#define CBMSTSQAIVPANELLIST_H

#include <Wt/WContainerWidget>

namespace Wt {
  class WPanel;
}

class CbmStsQaIvPanelList : public Wt::WContainerWidget
{
public:
  CbmStsQaIvPanelList(Wt::WContainerWidget *parent);

  Wt::WPanel *addWidget(const Wt::WString& text, Wt::WWidget *w);
  void addPanel(Wt::WPanel *panel);
  void removePanel(Wt::WPanel *panel);

  using WContainerWidget::addWidget;

private:
  void onExpand(bool notUndo);

  int wasExpanded_;
};

#endif //CBMSTSQAIVPANELLIST_H
