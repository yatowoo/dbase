void simple_read(){
  //
  // Purpose: - Retrieval mechanism of non-composite data
  //

  FairParTSQLIo *sql_io = new FairParTSQLIo();
  sql_io->open();
  sql_io->SetVerbosity(1);

 
  Int_t p_version=0;
  ValTimeStamp tStamp(2017,2,1,12,00,00);
  ValCondition cond(FairDbDetector::kSts, DataType::kData,tStamp);


  FairDbReader<FairDbExPar1> r_par1;
  
   r_par1.Activate(cond, p_version);

   Int_t nRows = r_par1.GetNumRows();
  
   cout << endl;   
   cout << " nRows: " << nRows << endl;

   Int_t fIdSystem;
   Int_t fPedestal;
   Int_t fGain1;
   Int_t fGain2;

   for (int i = 0; i < nRows; ++i) {
     FairDbExPar1* cgd = (FairDbExPar1*) r_par1.GetRow(i);
     if (!cgd) { continue; }
     fIdSystem =  cgd->GetIdSystem();
     fPedestal =  cgd->GetPedestal();
     fGain1    =  cgd->GetGain1();
     fGain2    =  cgd->GetGain2();
     
     cout << " row i= " << i << " : " <<  fIdSystem << " : "
          << fPedestal << " : " << fGain1
          << " : " << fGain2 << endl;
   }
   
   
   
   // Check validity range of results.
   ValTimeStamp t_start = r_par1.GetValidityRec()->GetValInterval().GetTimeStart();
   ValTimeStamp t_end   = r_par1.GetValidityRec()->GetValInterval().GetTimeEnd();
   
   cout << endl;    
   cout << " Validity range: " << r_par1.GetValidityRec()->GetValInterval().AsString() << endl;
   cout << " Time Start: " << t_start.AsString() << " Time end: " << t_end.AsString() <<  endl;
   cout << endl;    
   
   
   sql_io->close();
}
