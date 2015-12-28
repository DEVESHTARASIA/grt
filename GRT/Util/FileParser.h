/*
GRT MIT License
Copyright (c) <2012> <Nicholas Gillian, Media Lab, MIT>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef GRT_FILE_PARSER_HEADER
#define GRT_FILE_PARSER_HEADER

#include <iostream>     // cout, endl
#include <fstream>      // fstream
#include <vector>
#include <string>
#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <sstream>
#include "../DataStructures/Vector.h"
#include "InfoLog.h"

GRT_BEGIN_NAMESPACE

class FileParser{
public:
    FileParser():infoLog("[FileParser]"),warningLog("[WARNING FileParser]"){
	    clear();
    }
    ~FileParser(){
    }
    
    Vector< std::string >& operator[](const unsigned int &index){
        return fileContents[index];
    }
    
    bool parseCSVFile(std::string filename,bool removeNewLineCharacter=true){
        return parseFile(filename,removeNewLineCharacter,',');
    }
    
    bool parseTSVFile(std::string filename,bool removeNewLineCharacter=true){
        return parseFile(filename,removeNewLineCharacter,'\t');
    }
  
    bool getFileParsed(){
	  return fileParsed;
    }
  
    bool getConsistentColumnSize(){
	  return consistentColumnSize;
    }
  
    unsigned int getRowSize(){
	  return (unsigned int)fileContents.size();
    }
  
    unsigned int getColumnSize(){
	  return columnSize;
    }
  
    Vector< Vector< std::string > >& getFileContents(){
        return fileContents;
    }
    
    bool clear(){
        fileParsed = false;
        consistentColumnSize = false;
        columnSize = 0;
        fileContents.clear();
        return true;
    }
    
    static bool parseColumn( const std::string &row, Vector< std::string > &cols, const char seperator = ',' ){

        std::cout << "parse column!" << std::endl;

        const unsigned int N = (unsigned int)row.length();
        if( N == 0 ) return false;
        
        cols.clear();
        std::string columnString = "";
        const int sepValue = seperator;
        for(unsigned int i=0; i<N; i++){
            std::cout << "i: " << i << std::endl;
            if( int(row[i]) == sepValue ){
                cols.push_back( columnString );
                columnString = "";
            }else columnString += row[i];
        }
        
        //Add the last column
        cols.push_back( columnString );
        
        //Remove the new line char from the string in the last column
        if( cols.size() >= 1 ){
            size_t K = cols.size()-1;
            size_t foundA = cols[ K ].find('\n');
            size_t foundB = cols[ K ].find('\r');
            if( foundA != std::string::npos || foundB != std::string::npos ){
                cols[ K ]  = cols[ K ].substr(0,cols[K].length()-1);
            }
        }
        
        return true;
    }
  
protected:
    
    bool parseFile(const std::string &filename,const bool removeNewLineCharacter,const char seperator){

        infoLog << "parsefile!" << std::endl;
        
        //Clear any previous data
        clear();
        
        std::ifstream file( filename.c_str(), std::ifstream::in );
        if ( !file.is_open() ){
            warningLog << "parseFile(...) - Failed to open file: " << filename << std::endl;
            return false;
        }

        infoLog << "got here!" << std::endl;
        
        Vector< std::string > vec;
        infoLog << "got here!" << std::endl;
        std::string line;
        infoLog << "got here!" << std::endl;
        unsigned int lineCounter = 0;

        infoLog << "parsing file " << std::endl;
        
        //Loop over each line of data and parse the contents
        while ( getline( file, line ) )
        {
            infoLog << "top of while loop!" << std::endl;

            if( !parseColumn(line, vec,seperator) ){
                clear();
                warningLog << "parseFile(...) - Failed to parse column!" << std::endl;
                file.close();
                return false;
            }
            
            //Check to make sure all the columns are consistent
            if( columnSize == 0 ){
                consistentColumnSize = true;
                columnSize = (unsigned int)vec.size();
            }else if( columnSize != vec.size() ) consistentColumnSize = false;

            fileContents.push_back( vec );

            if( ++lineCounter % 1000 == 0 ){
                infoLog << "* ";
            }
        }

        infoLog << std::endl;
        infoLog << "File parsed, num rows: " << lineCounter << std::endl;
        
        //Close the file
        file.close();
        
        //Flag that we have parsed the file
        fileParsed = true;
        
        return true;
    }
    
    bool fileParsed;
    bool consistentColumnSize;
    unsigned int columnSize;
    InfoLog infoLog;
    WarningLog warningLog;
    Vector< Vector< std::string > > fileContents;

};
    
GRT_END_NAMESPACE

#endif //GRT_FILE_PARSER_HEADER
