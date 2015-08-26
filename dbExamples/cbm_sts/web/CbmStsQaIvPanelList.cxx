#include <Wt/WPanel>

#include "CbmStsQaIvPanelList.h"

using namespace Wt;

CbmStsQaIvPanelList::CbmStsQaIvPanelList(WContainerWidget *parent)
  : WContainerWidget(parent)
{ }

WPanel *CbmStsQaIvPanelList::addWidget(const WString& text, WWidget *w)
{
  WPanel *p = new WPanel();
  p->setTitle(text);
  p->setCentralWidget(w);

  addPanel(p);

  return p;
}

void CbmStsQaIvPanelList::addPanel(WPanel *panel)
{
  panel->setCollapsible(true);
  panel->collapse();

  panel->expandedSS().connect(this, &CbmStsQaIvPanelList::onExpand);

  WContainerWidget::addWidget(panel);
}

void CbmStsQaIvPanelList::onExpand(bool notUndo)
{
  WPanel *panel = dynamic_cast<WPanel *>(sender());

  if (notUndo) {
    wasExpanded_ = -1;

    for (unsigned i = 0; i < children().size(); ++i) {
      WPanel *p = dynamic_cast<WPanel *>(children()[i]);
      if (p != panel) {
	if (!p->isCollapsed())
	  wasExpanded_ = i;
	p->collapse();
      }
    }
  } else {
    if (wasExpanded_ != -1) {
      WPanel *p = dynamic_cast<WPanel *>(children()[wasExpanded_]);
      p->expand();
    }
  }
}
