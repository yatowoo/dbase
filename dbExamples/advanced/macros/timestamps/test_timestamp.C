Bool_t CompareTimeStamps(const char *label,
                                      ValTimeStamp& test, ValTimeStamp& std)
{
   if (test == std) {
      cout << label << "    exact match " << endl;
      return kTRUE;
   }
   else {
      cout << label << " ** mismatch ** " << endl;
      cout << "            " << test.AsString() << endl;
      cout << "            " << std.AsString() << endl;
      return kFALSE;
   }
}


void test_timestamp(void)
{
   //Purpose: Test ValTimeStamp functionality

   cout << "-I- The tests for ValTimeStamp begins ..." << endl << endl;

   Int_t fail = 0;
   Int_t tests = 0;

   cout << "-I- Instanciate a object:    " << flush;

   ValTimeStamp anObjectT;
   anObjectT.Print("");

   // Get the time ( a la 'C')
   time_t nowtime;
   time(&nowtime);
   long int nsec = 12345;

   struct timespec nowts;
   nowts.tv_sec  = nowtime;
   nowts.tv_nsec = nsec;

   cout  << "-I- original time_spec:  [" << nowtime << "," << nsec << "] " << endl;

   ValTimeStamp val_now(nowts);
   // calls are done separately since the  AsString()
   // method is reusing a common buffer
   cout    << "-I- val_now default AsString: " << val_now.AsString("") << endl;
   cout    << "-I- val_now local   AsString: " << val_now.AsString("l") << endl;
   cout    << "-I- val_now as timespec: " << val_now.AsString("2") << endl;   

   struct timespec thents = val_now.GetTimeSpec();
   cout   << "-I- val_now forwarded timespec: [" << thents.tv_sec << "," << thents.tv_nsec << "] " << endl;
   tests++;
   if (nowts.tv_sec != thents.tv_sec || nowts.tv_nsec != thents.tv_nsec){
     fail++;
     cout << "-E- Problem ecountered in original-forwarded timespec failures: " << fail << endl;
   }

   // test various ctor's
   // Reference date: Jan 04 2017, 01:26:03 GMT
   //time_t   mytime = 978571563;
   time_t   mytime = 1485250410;
   Long_t   arbitrary = 123456; // 

   timespec mytimespec = {mytime,arbitrary};
   ValTimeStamp vatr(mytime,arbitrary);
   ValTimeStamp vatsr(mytimespec);

   /*
   Int_t    year  = 2001;
   Int_t    month = 1;
   Int_t    day   = 4;
   Int_t    hour  = 1;
   Int_t    min   = 26;
   Int_t    sec   = 3;
   Int_t    secOffset = vatr.GetZoneOffset();
   */

   Int_t    year  = 2017;
   Int_t    month = 1;
   Int_t    day   = 24;
   Int_t    hour  = 9;
   Int_t    min   = 33;
   Int_t    sec   = 30;
   Int_t    secOffset = vatr.GetZoneOffset();
   
   
   ValTimeStamp val1(year,month,day,hour,min,sec,arbitrary);
   ValTimeStamp val3(year,month,day,hour-8,min,sec,arbitrary,kTRUE,8*60*60);
   ValTimeStamp val4(year,month,day-1,hour+16,min,sec,arbitrary,kTRUE,8*60*60);
   ValTimeStamp valy1(year-1900,month,day,hour,min,sec,arbitrary);
   ValTimeStamp valy2(year-2000,month,day,hour,min,sec,arbitrary);
   
   // Compute date
   Int_t    date = year*10000 + month*100 + day;
   Int_t    time = hour*10000 + min*100 + sec;
   
   ValTimeStamp vas1(date,time,arbitrary);
   ValTimeStamp vas3(date-1,time+160000,arbitrary,kTRUE,8*60*60);
   ValTimeStamp vas4(date,time-80000,arbitrary,kTRUE,8*60*60);
   ValTimeStamp vasy1(date-19000000,time,arbitrary);
   ValTimeStamp vasy2(date-20000000,time,arbitrary);

   cout  << "-I- Current Time Offset:" << vatr.GetZoneOffset() << endl;
   cout  << endl;
   cout  << "-I- standard (vatr) " << vatr.AsString()  << endl;

   //TString byhand = "Thu, 04 Jan 2017 01:26:03 UTC +   123456 nsec";
   TString byhand = "Tue, 24 Jan 2017 09:33:30 UTC +   123456 nsec";
   TString byclass = vatr.AsString();

   tests++;
   
   if (byhand != byclass) {
     cout << "-E- Date Strings mismatch ! :" << byclass << " : " << byhand <<  endl;
      fail++;
   }
   else {
      cout << "-I- Date Strings matched ... continuing tests .. " << endl;
   }

   // Add various dates/time comparisons tests 
   tests++;   if (!CompareTimeStamps(" vatsr     ",vatsr,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" val1      ",val1,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" val3      ",val3,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" val4      ",val4,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" valy1     ",valy1,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" valy2     ",valy2,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" vas1      ",vas1,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" vas3      ",vas3,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" vas4      ",vas4,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" vasy1     ",vasy1,vatr)) fail++;
   tests++;   if (!CompareTimeStamps(" vasy2     ",vasy2,vatr)) fail++;


   cout << "-I- Now also test on an impossible match ? " << endl;
   CompareTimeStamps(" Indeed .... no probable match on arbitrary time! ",val_now,vatr);

   cout << " Alternative Time Formats:" << endl;
   cout << " \"\"       " << vatr.AsString("")   << endl;
   cout << " \"l\"      " << vatr.AsString("l")  << endl;
   cout << " \"c\"      " << vatr.AsString("c")  << endl;
   cout << " \"lc\"     " << vatr.AsString("lc") << endl;
   cout << " \"2\"      " << vatr.AsString("2")  << endl;

   tests++; if (vatr.GetDate() != date) fail++;
   tests++; if (vatr.GetTime() != time) fail++;

 
     
   cout   << "-I-  GMT    GetDate(): " << vatr.GetDate()
	  << "         GetTime(): " << (vatr.GetTime()) << endl;
   
   cout   << "-I- local   GetDate(): " << vatr.GetDate(kFALSE)
	  << "         GetTime(): " << (vatr.GetTime(kFALSE)) << endl;


   // Results Summary   
   cout   << "*************  Summary for TestTimeStamp tests ********* " << endl;
   cout   << " found  nFailures: " << fail  << " in  " << tests << " tests" << endl;
   cout   << endl;
}
