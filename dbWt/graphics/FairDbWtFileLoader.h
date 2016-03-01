#ifndef FAIRDBWTFILELOADER
#define FAIRDBWTFILELOADER

#include <Wt/WContainerWidget>
#include <iostream>
using namespace std;

namespace Wt {
  class WFileUpload;
  class WText;
  class WCheckBox;
}

using namespace Wt;

class FairDbWtFileLoader : public WContainerWidget
{
public:
  FairDbWtFileLoader(WContainerWidget *parent = 0);
  bool uploadNow();
  bool uploadFailed() const { return fUploadFailed; }
  Signal<void>& uploadDone() { return fUploadDone; }

private:
  Signal<void> fUploadDone;

  //! The WFileUpload control.
  WFileUpload *fUpload;

  class FairDbWtUploadInfo : public WContainerWidget
  {
  public:
    FairDbWtUploadInfo(const Http::UploadedFile& f, WContainerWidget *parent = 0);

    Http::UploadedFile fInfo;
    //! Anchor referencing the file.
    WAnchor   *fDownloadLink;
    //! The check box to keep or discard the uploaded file.
    WCheckBox *fKeep;
  };

  std::vector<FairDbWtUploadInfo *> fUploadInfo;
  WText *fError;
  bool fUploadFailed;
  void uploaded();
  void fileTooLarge(::int64_t size);
  void getFileName();

};
#endif // FAIRDBWTFILELOADER
