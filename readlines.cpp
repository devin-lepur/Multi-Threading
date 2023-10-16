/* 
 * Author: Devin Lepur
 * RedID: 825148526
 * 9/28/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * readlines.cpp
 */

#include <fstream>
#include <iostream>
#include <string>
#include "shareddata.h"
#include "readlines.h"

void* readlinesThread(void* shared_data) {
    SHARED_DATA* data = static_cast<SHARED_DATA*>(shared_data);

    data->lineCountInFile[TESTFILEINDEX] = 0;

    //Open file
    std::ifstream testFile(data->fileName[TESTFILEINDEX]);

    //Check if file is valid
    if(!testFile.is_open()) {
        std::cerr << "Unable to open <<" << data->fileName[TESTFILEINDEX]  << ">>"  << std::endl;
        exit(EXIT_FAILURE);
        return NULL;
    }

    std::string line;
    

    //Read lines from test file and add them to queue
    while(std::getline(testFile, line)) {
        data->lineCountInFile[TESTFILEINDEX]++;
        //Lock mutex
        pthread_mutex_lock(&data->queue_mutex);

        data->lineQueue->push(line);

        //Unlock mutex
        pthread_mutex_unlock(&data->queue_mutex);
    }

    testFile.close();

    data->taskCompleted[TESTFILEINDEX] = true;
    return NULL;
}