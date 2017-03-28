#ifndef CBMSTSWTQAIVPANELLIST_H
#define CBMSTSWTQAIVPANELLIST_H

#include <Wt/WContainerWidget>

namespace Wt {
  class WPanel;
}

class CbmStsWtQaIvPanelList : public Wt::WContainerWidget
{
public:
  CbmStsWtQaIvPanelList(Wt::WContainerWidget *parent);

  Wt::WPanel *addWidget(const Wt::WString& text, Wt::WWidget *w);
  void addPanel(Wt::WPanel *panel);
  void removePanel(Wt::WPanel *panel);

  using WContainerWidget::addWidget;

private:
  void onExpand(bool notUndo);

  int wasExpanded_;
};

#endif //CBMSTSWTQAIVPANELLIST_H
