Int_t  readn_table()
{
  CbmStsDbQaNewPar *stsPar = new CbmStsDbQaNewPar();

  // Import  Summary
   stsPar->ImportFromCsvFile("../data/new/summary.csv");
  // Import  ChannelMap
   stsPar->ImportFromCsvFile("../data/new/channel_map.csv");
   stsPar->ImportFromCsvFile("../data/new/geometry.csv");
   stsPar->ImportFromCsvFile("../data/new/ownership.csv");
   stsPar->ImportFromCsvFile("../data/new/qa_results.csv");
   

  // Read the I:V tables in data dir.
   stsPar->ImportIvFilesFromDir();
 
  stsPar->Print();

return 0;
}
