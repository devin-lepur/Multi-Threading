/* 
 * Author: Devin Lepur
 * RedID: 825148526
 * 9/28/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * readvocab.cpp
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "shareddata.h"
#include "readvocab.h"

void* readvocabThread(void* shared_data) {
    SHARED_DATA* data = static_cast<SHARED_DATA*>(shared_data);

    data->lineCountInFile[VOCABFILEINDEX] = 0;
    data->numOfCharsReadFromVocabFile = 0;
    
    //Open vocab file
    std::ifstream vocabFile(data->fileName[VOCABFILEINDEX]);

    //Check if file is valid
    if(!vocabFile.is_open()) {
        std::cerr << "Unable to open <<" << data->fileName[VOCABFILEINDEX]  << ">>"  << std::endl;
        exit(EXIT_FAILURE);
        return NULL;
    }
    
    std::string line;

    //Read words and stores them in a vector
    while(std::getline(vocabFile, line)) {
        data->lineCountInFile[VOCABFILEINDEX]++;
        data->numOfCharsReadFromVocabFile += line.length();
        data->numOfCharsReadFromVocabFile ++;
        data->vocabVect->push_back(line);
    }

    //Close file
    vocabFile.close();

    //Set flag for readvocabThread completion
    data->taskCompleted[VOCABFILEINDEX] = true;
    return NULL;
}