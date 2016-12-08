void suid(bool update=false){
//
  //
  // Purpose: - Write mechanism of non-composite data
  //            from database table
  //

  TStopwatch t;
  t.Start();
  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(0);

  //Write skipping 0 entry 
  Int_t db_entry = 0;
  
  ValTimeStamp start;
  ValTimeStamp   end = ValTimeStamp::GetEOT();
    
  ValInterval  range(FairDbDetector::kSts,
                     DataType::kData,start,end,"test");
  
  CbmStsDbQaSUID sUID;
  sUID.SetVersion(0);
  //aPar1.SetComboNo(-1);
  sUID.SetDbEntry(db_entry);
  sUID.SetLogTitle("suid");
  sUID.CreateDbTable(0);


  ValTimeStamp now;
  // Create the Writer 
  FairDbWriter<CbmStsDbQaSUID> w_suid;
  // Writer activation:  val_range   comp.  version   db_entry   logtitle

  
  if (!update) {
    int wafer = 10;
    for (int i=0; i<200; i++){
      w_suid.Activate(range, i, sUID.GetVersion(), sUID.GetDbEntry(), sUID.GetLogTitle());
      w_suid.SetComboNo(i);
      sUID.SetVendor("CiS");
      sUID.SetBatchId("12345");
      sUID.SetBatchTime("date");
      sUID.SetWaferId(wafer);
      sUID.SetSensorId(i);
      sUID.SetCompId(i);
      w_suid << sUID;
    }
    
  } else {
    // update the last component
    int i = 199;  
    w_suid.Activate(range, i, sUID.GetVersion(), sUID.GetDbEntry(), sUID.GetLogTitle());
    w_suid.SetComboNo(i);
    sUID.SetVendor("HAMAMATSU");
    sUID.SetBatchId("3444");
    sUID.SetBatchTime("date");
    sUID.SetWaferId(10);
    sUID.SetSensorId(i);
    sUID.SetCompId(i);
    w_suid << sUID;
    
  }
  
  if(!w_suid.Close()){
      cout << "-E- IO error writing par1 " << endl;
   }
  
  t.Stop();
  t.Print();
  
  // Close DB Connection 
  //if (sql_io) sql_io->Close();
}
