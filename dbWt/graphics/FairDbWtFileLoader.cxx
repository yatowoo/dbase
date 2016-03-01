#include <fstream>
#ifndef WIN32
#include <unistd.h>
#endif
#include <boost/lexical_cast.hpp>
#include <iostream>

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WCheckBox>
#include <Wt/WCssDecorationStyle>
#include <Wt/WFileResource>
#include <Wt/WFileUpload>
#include <Wt/WProgressBar>
#include <Wt/WText>

#include "FairDbWtFileLoader.h"

using namespace std;


FairDbWtFileLoader::FairDbWtUploadInfo::FairDbWtUploadInfo(
	const Http::UploadedFile& f, WContainerWidget *parent)
  : WContainerWidget(parent),
    fInfo(f)
{
  fKeep = new WCheckBox(this);
  fKeep->setChecked();

  std::streamsize fsize = 0;
  {
    std::ifstream theFile(fInfo.spoolFileName().c_str());
    theFile.seekg(0, std::ios_base::end);
    fsize = theFile.tellg();
    theFile.seekg(0);
  }
  std::wstring size;
  if (fsize < 1024)
    size = boost::lexical_cast<std::wstring>(fsize) + L" bytes";
  else
    size = boost::lexical_cast<std::wstring>((int)(fsize / 1024))
      + L"kb";

  std::wstring fn = static_cast<std::wstring>
    (escapeText(WString::fromUTF8(fInfo.clientFileName())));

  fDownloadLink
    = new WAnchor("", fn + L" (<i>" + WString::fromUTF8(fInfo.contentType())
		  + L"</i>) " + size, this);

  WFileResource *res = new WFileResource(fInfo.contentType(),
					 fInfo.spoolFileName(),
					 this);
  res->suggestFileName(fInfo.clientFileName());
  fDownloadLink->setLink(res);
}

FairDbWtFileLoader::FairDbWtFileLoader(WContainerWidget *parent)
  : WContainerWidget(parent),
    fUploadDone(this),
    fUploadFailed(false)
{
  fUpload = new WFileUpload(this);
  fUpload->setMultiple(true);
  fUpload->setFileTextSize(1000);// 1GB

  WProgressBar *progress = new WProgressBar();
  progress->setFormat(WString::Empty);
  progress->setVerticalAlignment(AlignMiddle);
  fUpload->setProgressBar(progress);

  fUpload->decorationStyle().font().setSize(WFont::Smaller);
  fUpload->setVerticalAlignment(AlignMiddle);

	// The error message.
  fError= new WText("", this);
  fError->setStyleClass("error");
  fError->setMargin(WLength(5), Left);

  //fUpload->changed().connect(fUpload,  &WFileUpLoad::upload);
  fUpload->changed().connect(this,  &FairDbWtFileLoader::getFileName);
  fUpload->uploaded().connect(this, &FairDbWtFileLoader::uploaded);
  // React to a fileupload problem.
  fUpload->fileTooLarge().connect(this, &FairDbWtFileLoader::fileTooLarge);
}


void FairDbWtFileLoader::getFileName(){
    uploadNow();
}



bool FairDbWtFileLoader::uploadNow()
{
  if (fUpload) {
    if (fUpload->canUpload()) {
        cout << "-I- File to upload name#" <<  fUpload->spoolFileName() << " : " <<
				 fUpload->clientFileName() << endl;
			 //fUpload->upload();
       return true;
    } else
      return false;
  } else
    return false;
}

void  FairDbWtFileLoader::uploaded()
{
  std::vector<Http::UploadedFile> files = fUpload->uploadedFiles();

  if (!files.empty()) {
    delete fUpload;
    fUpload = 0;
    delete fError;
    fError = 0;

    for (unsigned i = 0; i < files.size(); ++i)
      fUploadInfo.push_back(new FairDbWtUploadInfo(files[i], this));
  } else {
    fError->setText(tr("msg.file-empty"));
    fUploadFailed = true;
  }
  fUploadDone.emit();
}

void FairDbWtFileLoader::fileTooLarge(::int64_t size)
{
  fError->setText(tr("msg.file-too-large")
		  .arg(size / 1024)
		  .arg(WApplication::instance()->maximumRequestSize() / 1024));
  fUploadFailed = true;
  fUploadDone.emit();
}
