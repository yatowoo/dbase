void read_suid(Int_t compid=0){

  // Query date
  ValTimeStamp t_query(2016, 11, 18, 8, 25, 0);
  ValCondition cond(FairDbDetector::kSts, DataType::kData,t_query);
  cout << "-I- Query time : " << t_query <<  " condition: " << cond << endl;    

  // Create a Reader template
  FairDbReader<CbmStsDbSUID> r_suid;
  r_suid.Activate(cond, 0);

  Int_t nRows = r_suid.GetNumRows();
  cout << "-I- Reading SUID table, found nb of rows: " << nRows << endl;
  CbmStsDbSUID* cgd=NULL;

  if ( compid == 0 )  {    
    for (int i = 0; i < nRows; ++i) {
      cgd = (CbmStsDbSUID*) r_suid.GetRow(i);
      if (!cgd) { continue; }
      Int_t comp_id =  cgd->GetCompId();
      Int_t batch_id =  cgd->GetBatchId();
      string vendor =  cgd->GetVendor();
      cout << " @ row = " << i << " found comp_id: " << comp_id <<
        " batch_id: " <<  batch_id <<  " vendor: " << vendor << endl;
    }   
  }else {
    cout << "-I- Random access SUID table via indexes ( comp_id ) " << endl; 
    UInt_t index = compid;
      const CbmStsDbSUID* suid = r_suid.GetRowByIndex(index);
      if (suid){
        suid->Print(); 
      }
    }
    
}
