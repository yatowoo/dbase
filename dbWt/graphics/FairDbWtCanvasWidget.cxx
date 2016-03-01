#include "FairDbWtCanvasWidget.h"
#include <Wt/WContainerWidget>
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WGridLayout>

#include "FairDbWtJsRootProxy.h"

#include <iostream>

using namespace Wt;
using namespace std;

FairDbWtCanvasWidget::FairDbWtCanvasWidget(WContainerWidget *root
                                           ,int w  , int h
                                           ,int row, int col)
  : WContainerWidget(root)
  , fWidth(w)
  , fHeight(h)
  , fRows(row)
  , fColumns(col)
  , fIndex(0)
{
  // Create the grid layout
  WGridLayout *grid = new WGridLayout();

  // Disable Grid spacing (default is used)
  grid->setHorizontalSpacing(9);
  grid->setVerticalSpacing(9);

  // Divide the grid into equally-sized cells with stretch 0
  for (int c = 0; c < fColumns; ++c)
  grid->setColumnStretch(c, 1);
  for (int r = 0; r < fRows; ++r)
  grid->setRowStretch(r, 1);

  //Add dummy ContainerWidget in the main container

  for (int i = 0; i < fRows; i++){
    for (int j = 0; j < fColumns; j++){
      // set resizeable columns
      grid->setColumnResizable(j);
      grid->setRowResizable(i);

      // add a placeholder widget for styling
      WContainerWidget *cw =
      FairDbWtJsRootProxy::createContainerWidget(nullptr,nullptr);
      cw->resize(fWidth,fHeight);
      cw->setContentAlignment(Wt::AlignCenter);
      int idx = (i*fColumns)+ j;
      fContWidgets.insert(fContWidgets.begin()+idx, cw);
      grid->addWidget(cw, i, j, Wt::AlignCenter);
    }
  }
   setLayout(grid);
}
