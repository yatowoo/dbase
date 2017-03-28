#ifndef CBMSTSWTQAIVCHARTCONFIG_H_
#define CBMSTSWTQAIVCHARTCONFIG_H_

#include <Wt/WContainerWidget>
#include <Wt/Chart/WDataSeries>

namespace Wt {
  class WCheckBox;
  class WComboBox;
  class WFormWidget;
  class WLineEdit;
  class WTable;

  namespace Chart {
    class WCartesianChart;
  }
}

class CbmStsWtQaIvChartConfig : public Wt::WContainerWidget
{
 public:
  
  CbmStsWtQaIvChartConfig(Wt::Chart::WCartesianChart *chart, Wt::WContainerWidget *parent);
  
  void setValueFill(Wt::Chart::FillRangeType fill);
  
 private:
  Wt::Chart::WCartesianChart  *chart_;
  Wt::Chart::FillRangeType fill_;
  
  //! Struct that holds the controls for one series
  struct SeriesControl {
    Wt::WCheckBox *enabledEdit;
    Wt::WComboBox *typeEdit;
    Wt::WComboBox *markerEdit;
    Wt::WComboBox *axisEdit;
    Wt::WCheckBox *legendEdit;
    Wt::WCheckBox *shadowEdit;
    Wt::WComboBox *labelsEdit;
  };
  
  //! Controls for series
  std::vector<SeriesControl> seriesControls_;
  
  //! Struct that holds the controls for one axis
  struct AxisControl {
    Wt::WCheckBox *visibleEdit;
    Wt::WComboBox *scaleEdit;
    Wt::WCheckBox *autoEdit;
    Wt::WLineEdit *minimumEdit;
    Wt::WLineEdit *maximumEdit;
    Wt::WCheckBox *gridLinesEdit;
    Wt::WLineEdit *labelAngleEdit;
  };
  
  //! Controls for axes
  std::vector<AxisControl> axisControls_;
  
  Wt::WLineEdit *titleEdit_;
  Wt::WLineEdit *chartWidthEdit_;
  Wt::WLineEdit *chartHeightEdit_;
  Wt::WComboBox *chartOrientationEdit_;
  Wt::WComboBox *legendLocationEdit_;
  Wt::WComboBox *legendSideEdit_;
  Wt::WComboBox *legendAlignmentEdit_;

  void connectSignals(Wt::WFormWidget *w);
  void update();

  static bool validate(Wt::WFormWidget *w);
};

#endif // CBMSTSWTQAIVCHARTCONFIG_H
