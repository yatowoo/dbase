#ifndef FAIRDBPOPUP_H_
#define FAIRDBPOPUP_H_

#include <Wt/WObject>
#include <Wt/WString>
#include <Wt/WJavaScript>

using namespace Wt;

class FairDbPopup : public WObject
{
public:
  static FairDbPopup *createConfirm(const WString& message, WObject *parent = 0);

  static FairDbPopup *createPrompt(const WString& message,
			     const std::string defaultValue,
			     WObject *parent = 0);

  static FairDbPopup *createAlert(const WString& message, WObject *parent = 0);

  void setMessage(const WString& message);
  void setDefaultValue(const std::string defaultValue);
  const WString& message() const { return message_; }
  const std::string& defaultValue() const { return defaultValue_; }


  JSlot show;
  JSignal<std::string>& okPressed() { return okPressed_; }
  JSignal<void>&        cancelPressed() { return cancelPressed_; }

private:
  enum Type { Confirm, Alert, Prompt };

  FairDbPopup(Type t, const WString& message, const std::string defaultValue,
	WObject *parent);

  JSignal<std::string> okPressed_;
  JSignal<void>        cancelPressed_;

  Type t_;
  WString message_;
  std::string defaultValue_;

  void setJavaScript();
};


#endif // FAIRDBPOPUP_H_
