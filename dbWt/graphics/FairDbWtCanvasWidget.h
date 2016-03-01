#ifndef FAIRDBWTCANVASWIDGET
#define FAIRDBWTCANVASWIDGET

#include <Wt/WContainerWidget>
#include <Wt/WGridLayout>
#include <vector>

using namespace Wt;


class FairDbWtCanvasWidget : public WContainerWidget
{
public:
  FairDbWtCanvasWidget(WContainerWidget *root=nullptr,
    int w=800, int h=800, int row=1, int col=1);
  int getRows(){return fRows;}
  int getColumns(){return fColumns;}
  int getCapacity() {return (fRows*fColumns);}
  int getIndex() { return fIndex;}
  void getFreeSlot(int& a_row, int& a_col){
    // Method to allocate a empty slot ( row, col)
     int idx = fIndex%getCapacity();
     a_row =  (int)  (idx / getColumns());
     a_col =  (int)  (idx % getColumns());
     fIndex++;
  }

 WContainerWidget* getContainer(int row, int col){
  int idx = (row*fColumns) + col;
  return fContWidgets[idx];
 }

private:
  int fWidth;
  int fHeight;
  int fRows;
  int fColumns;
  int fIndex;

  std::vector<WContainerWidget*> fContWidgets;
};

#endif // FAIRDBWTCANVASEWIDGET
