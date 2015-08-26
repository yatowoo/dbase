Int_t  read_table()
{
  CbmStsDbQaPar *stsPar = new CbmStsDbQaPar();

  // First import Csv Model file
  stsPar->ImportFromCsvFile("../data/sensor-table.csv");

  // Read the I:V tables in data dir.
  stsPar->ImportIvFilesFromDir();
 
  stsPar->Print();

return 0;
}
