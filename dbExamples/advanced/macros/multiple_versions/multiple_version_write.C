void multiple_version_write(){
  //
  //
  // Purpose: - Write multiple version of non-composite data
  //            to database table
  //           


  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->SetVerbosity(1);
  sql_io->open();
 
 
  
  struct Test_t {
    Int_t    dateResStart;
    Int_t    timeResStart;
    Int_t    dateResEnd;
    Int_t    timeResEnd;
    Double_t  firstGain1;
    Double_t  lastGain1;
  };
  
  Test_t tests[] = {
    {20140101,110000, 20140131,  10203, 10., 20.},
    {20140131, 10204, 20150101,  10203,  2.,  8.},
    {20150101, 10204, 20150104,  10203, 30., 33.},
    {20150105,110000, 20150105, 130000, 1.0, 1.3},
    {20150120, 40506, 20380119,  31407,  8.,  9.},
  };

  Int_t db_entry=0;
   
  // Create par1 Parameter
  FairDbExPar1 aPar1;
  aPar1.SetVersion(0);
  aPar1.SetComboNo(-1);
  aPar1.SetDbEntry(db_entry);
  aPar1.SetLogTitle("test");

  // Create corresponding table @ db_entry
  aPar1.CreateDbTable(db_entry);
  

 for (int itest=0; itest<5; ++itest ) {
    Test_t& test = tests[itest];
    Int_t date_start = test.dateResStart;
    Int_t time_start = test.timeResStart;
    Int_t date_end   = test.dateResEnd;
    Int_t time_end   = test.timeResEnd;
    Double_t gain1   = test.firstGain1;
    Double_t gain2   = test.lastGain1;
    
    ValTimeStamp t_start(date_start/10000, date_start/100%100, date_start%100,
                         time_start/10000, time_start/100%100, time_start%100);

    ValTimeStamp t_end(date_end/10000, date_end/100%100, date_end%100,
                       time_end/10000, time_end/100%100, time_end%100);
    
    
    cout << "-I- created Validity Time Range " << t_start.AsString()
         << " : " 
         << t_end.AsString() 
	 << endl;
    
    aPar1.SetIdSystem(2);
    aPar1.SetPedestal(2.);
    aPar1.SetGain1(gain1);
    aPar1.SetGain2(gain2);
    
    // Create a IoV and corresponding writer for FairDbExPar1 
    ValInterval range(FairDbDetector::kSts,DataType::kData,t_start, t_end, "test");
    FairDbWriter<FairDbExPar1> w_par1;
    // Writer activation:  val_range   comp.  version   db_entry   logtitle
    w_par1.Activate(range, aPar1.GetComboNo(), aPar1.GetVersion(),db_entry, aPar1.GetLogTitle());
    w_par1 << aPar1;
    
    if(!w_par1.Close()){
      cout << "-E- IO error writing par1 " << endl;
    }
      
 }//!for(itest)


 // Close DB connection
  sql_io->close();
  
}  
