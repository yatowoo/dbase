void simple_write(){
  //
  //
  // Purpose: - Write mechanism of non-composite data
  //            from database table
  //
  
  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);


  ValTimeStamp start(2017,1,1,12,00,00);
  ValTimeStamp   end(2017,9,27,12,00,00);
  ValInterval  range(FairDbDetector::kSts,
		      DataType::kData,start,end,"test");
  

  FairDbExPar1 aPar1;
  aPar1.SetVersion(0);
  aPar1.SetComboNo(-1);
  aPar1.SetDbEntry(0);
  aPar1.SetLogTitle("test");
  
  Int_t db_entry=0;
  
  aPar1.CreateDbTable(db_entry);
  
  FairDbWriter<FairDbExPar1> w_par1;

  // Writer activation:  val_range   comp.  version   db_entry   logtitle
  w_par1.Activate(range, aPar1.GetComboNo(), aPar1.GetVersion(), aPar1.GetDbEntry(), aPar1.GetLogTitle());

  // Write to SQL table the parameter values
  aPar1.SetIdSystem(9);
  aPar1.SetPedestal(9.);
  aPar1.SetGain1(9.);
  aPar1.SetGain2(9.);
  // IO
  w_par1 << aPar1;

  // Write to the same table other values
  // of the same parameter 
  aPar1.SetIdSystem(2);
  aPar1.SetPedestal(2.);
  aPar1.SetGain1(2.);
  aPar1.SetGain2(3.);

  w_par1 << aPar1;

  if(!w_par1.Close()){
      cout << "-E- IO error writing par1 " << endl;
  }
    
}
