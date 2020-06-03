#include <Arduino.h>
#include <SD.h>

int dataLength = 10;

File myFile;
File root;

String message = "0";
String textdata;
String datanummer;


#define lineSize 23;
unsigned long fileSize;
unsigned long numLines;




void printDirectory(File dir) { //dir skal være root directory - dvs. den mappe .txt filerne gemmes i
  uint8_t fileCount = 0;

                      
  while (true) {

    File entry =  dir.openNextFile(); //Går ind i directoriet, returnerer næste fil i mappen
    if (!entry) { //Hvis ikke flere filer, stop while-loop
      // no more files
        Serial.println(fileCount); //Muligvis løsningen på Matlab - sender én int frem for x antal filnavne
           break;
    }
    //Serial.println(entry.name()); //printer næste filnavn. 
    fileCount++;
    entry.close(); //lukker filen
  }
}

void setup() {
Serial.begin(115200);


pinMode(5,OUTPUT);
 SD.begin(5); //SS pin

 




}

void loop() {

if (Serial.available() > 0) {

  message = Serial.readStringUntil('\n');
    
  if (message.indexOf ("datafile")>= 0)
    { 
      root = SD.open("/"); //åbner nuværende directory, bestemt ved "/"
      printDirectory(root); //Læser filnavne i nuværende directory, printer til seriel porten
      root.close();
      
    } 

  if (message.indexOf ("loaddata")>= 0){ //Matlab sender "loaddata"
  datanummer = message.substring(8,message.length());    
      
      myFile = SD.open("/data" + datanummer + ".txt",FILE_READ); //Åbner filen som en readfil
        if (!myFile)
        {
           Serial.println("Filen kunne ikke findes");            
        }
        else 
        {
             fileSize = myFile.size(); //finder størrelsen af hver fil
          numLines = fileSize/lineSize; //filstørrelsen deles med linjestørrelse. Sådan findes antal linjer.  
           
          Serial.println(numLines); //Sender antal linjer i filen til Matlab -> Evt. til brug i For-loop
          delay(1000); //Delay således at den kan nå at sende numLines til MatLab før MatLab begynder at læse data
          
          for(uint16_t i = 0;i<=numLines;i++)
          {
          textdata = myFile.readStringUntil('\n'); // alle dele af string'en, indtil \n (newline). Så kører den for-loopet igennem igen, indtil den har gjort det for alle linjer af .txt fil (numLines)
          Serial.println(textdata);
          delay(1); //Delay for at undgå synkroniseringsissues
          }
        
        }         
             
      //læs linje for linje
      myFile.close();
    }
   
  } 
}