void composite_write(){
  //
  // Purpose: Write composite data model

  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);
  
  
  struct Test_t {
    Int_t    dateResStart;
    Int_t    timeResStart;
    Int_t    dateResEnd;
    Int_t    timeResEnd;
    Int_t    comp_id;
    Int_t    s_id1;
    Int_t    s_id2;
    Int_t    s_id3;
    Double_t p_data1;
    Double_t p_data2;
    Double_t p_data3;
  };
  
  Test_t tests[] = {
     {20140101, 000000, 20160131,  000000, 195, 20, 30, 110, 102., 103., 111.}
     {20140101, 000000, 20160131,  000000, 1098, 50, 80, 130, 105., 108., 113.}
  };

  Int_t db_entry=0;

  // Create FairDbExPar2 Parameter
  FairDbExPar2 aPar2(FairDbDetector::kSts, DataType::kData,"ExPar2","ExPar2","Example");
  
  // Create corresponding table @ db_entry=0 
  aPar2.CreateDbTable(db_entry);
  
  
  for (int itest=0; itest<2; ++itest ) {
    Test_t& test = tests[itest];
    Int_t date_start = test.dateResStart;
    Int_t time_start = test.timeResStart;
    Int_t date_end   = test.dateResEnd;
    Int_t time_end   = test.timeResEnd;
    Int_t compId   = test.comp_id;
    Int_t s_id[3] = {test.s_id1, test.s_id2, test.s_id3};
    Int_t p_data[3] = {test.p_data1, test.p_data2, test.p_data3};
    
    
    
    ValTimeStamp t_start(date_start/10000, date_start/100%100, date_start%100,
                         time_start/10000, time_start/100%100, time_start%100);
    
    ValTimeStamp t_end(date_end/10000, date_end/100%100, date_end%100,
                       time_end/10000, time_end/100%100, time_end%100);
    
    
    cout << " -I- created Validity Time Range " << t_start.AsString()
         << " : " 
         << t_end.AsString() 
	 << endl;

    // Set composition id
    aPar2.SetCompId(compId);
   
    // Create a IoV and corresponding writer for FairDbExPar2 
    ValInterval range(FairDbDetector::kSts,DataType::kData,t_start, t_end, "test_range");

    // Get Writer from parameter
    FairDbWriter<FairDbExPar2>& w_par2 = aPar2.GetWriter();
    
    // Writer activation:  val_range   comp.  version   db_entry   logtitle
    w_par2.Activate(range, aPar2.GetComboNo(), aPar2.GetVersion(),
		           aPar2.GetDbEntry(), aPar2.GetLogTitle());
    
    
    for (Int_t idata=0;idata<3;idata++){
      
      aPar2.SetIdSystem(s_id[idata]);
      aPar2.SetData(p_data[idata]);
      
      w_par2 << aPar2;
      
    }
    
    if(!w_par2.Close()){
      cout << "-E- IO error writing par2 " << endl;
    }
    
  }//!for(itest)
  

 // Close DB connection
 sql_io->close();
  
}  
