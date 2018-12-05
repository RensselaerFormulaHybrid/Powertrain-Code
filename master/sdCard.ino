void createNewFile() {
  int i=1;
  currentFileName = "Run1.txt";
  String j = "";
  while(SD.exists(currentFileName)) {   //Increase Run# until unused filename is found
    i++;
    j=i;
    currentFileName = "Run"+j+".txt";
  }
  activeFile = SD.open(currentFileName, FILE_WRITE);    //Create file, immediately close
  activeFile.close();
}

void writeToSD() {
  String i = ",";
  sdString = "nul";
  activeFile = SD.open(currentFileName, FILE_WRITE);   //Open currentFile, write sdString, close currentFile
  activeFile.println(sdString);
  activeFile.close();
}
