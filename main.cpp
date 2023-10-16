/* 
 * Author: Devin Lepur
 * RedID: 825148526
 * 9/28/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * main.cpp
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cmath>
#include "shareddata.h"
#include "readvocab.h"
#include "readlines.h"
#include "countvocabstrings.h"


int main(int argc, char* argv[]) {
    //set argv[] argument indexes
    const int vocabArgIndex = 1;
    const int testArgIndex = 2;

    SHARED_DATA sharedData;

    //Set default shared data values
    sharedData.numOfProgressMarks = DEFAULT_NUMOF_MARKS;
    sharedData.hashmarkInterval = DEFAULT_HASHMARKINTERVAL;
    sharedData.minNumOfVocabStringsContainedForPrinting = DEFAULT_MINNUM_OFVOCABSTRINGS_CONTAINED;
    sharedData.taskCompleted[TESTFILEINDEX] = false;
    sharedData.taskCompleted[VOCABFILEINDEX] = false;


    //optarg proved problematic so manual checking of op args is used
    //iterate through argv and check for optional args starting with '-'
    //check which op arg it is and update values accordingly
    for(int i = 0; i < argc; i++) {
        if(argv[i][0] == '-') {
            
            if(argv[i][1] == 'p') {
                sharedData.numOfProgressMarks = atoi(argv[i + 1]);
                if(sharedData.numOfProgressMarks < MIN_NUMOF_MARKS) {
                    std::cerr << "Number of progress marks must be a number and at least 10" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            if(argv[i][1] == 'm') {
                sharedData.hashmarkInterval = atoi(argv[i + 1]);
                if(sharedData.hashmarkInterval <= 0 || sharedData.hashmarkInterval > 10) {
                    std::cerr << "Hash mark interval for progress must be a number, greater than 0, and less than or equal to 10" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
            if(argv[i][1] == 'v') {
                sharedData.minNumOfVocabStringsContainedForPrinting = atoi(argv[i + 1]);
            }
        }
    }

    //store files names in sharedData
    sharedData.fileName[VOCABFILEINDEX] = argv[vocabArgIndex];
    sharedData.fileName[TESTFILEINDEX] = argv[testArgIndex];

    //create a stat structure to find num of chars in each file
    struct stat vocabBuffer;
    stat(sharedData.fileName[VOCABFILEINDEX], &vocabBuffer);

    sharedData.totalNumOfCharsInVocabFile = vocabBuffer.st_size;

    //initialize data structures for thread usage
    sharedData.lineQueue = new std::queue<std::string>();
    sharedData.vocabVect = new std::vector<std::string>();


    //initialize mutex
    pthread_mutex_init(&sharedData.queue_mutex, NULL);

    //create threads
    pthread_t readvocabThreadID, readlinesThreadID, countvocabstringsThreadID;
    pthread_create(&readvocabThreadID, NULL, readvocabThread, &sharedData);
    pthread_create(&readlinesThreadID, NULL, readlinesThread, &sharedData);
    pthread_create(&countvocabstringsThreadID, NULL, countvocabstringsThread, &sharedData);


    unsigned int displayCount = 0;
    //display for readvocabThread
    while(true) {
        double percent = (double) sharedData.numOfCharsReadFromVocabFile / (double)sharedData.totalNumOfCharsInVocabFile;
        double markInterval = 1.0 / sharedData.numOfProgressMarks;
        unsigned int numOfMarksNeeded = std::floor(percent / markInterval);
        while(displayCount < numOfMarksNeeded) {
            displayCount++;
            if(displayCount % sharedData.hashmarkInterval == 0) {
                std::cout << '#';
            } else {
                std::cout << '-';
            }
        }
        if(sharedData.numOfProgressMarks == displayCount) {
            std::cout << "\nThere are " << sharedData.lineCountInFile[VOCABFILEINDEX] << " lines in vocabfile.txt" << std::endl;
            break;
        }
    }


    displayCount = 0;
    //Display for countvocabstringsThread
    while(true) {
        double percent = (double) sharedData.numOfProcessedLines / (double) sharedData.lineCountInFile[TESTFILEINDEX];
        double markInterval = 1.0 / sharedData.numOfProgressMarks;
        unsigned int numOfMarksNeeded = std::floor(percent / markInterval);
        while(displayCount < numOfMarksNeeded) {
            displayCount++;
            if(displayCount % sharedData.hashmarkInterval == 0) {
                std::cout << '#';
            } else {
                std::cout << '-';
            }
        }
        if(sharedData.numOfProgressMarks == displayCount) {
            std::cout << "\nThere are " << sharedData.lineCountInFile[TESTFILEINDEX] << " lines in testfile.txt." << std::endl;
            break;
        }
    }
    
    

    //wait for completion
    pthread_join(readvocabThreadID, NULL);
    pthread_join(readlinesThreadID, NULL);
    pthread_join(countvocabstringsThreadID, NULL);

    pthread_mutex_destroy(&sharedData.queue_mutex);

    delete sharedData.lineQueue;
    delete sharedData.vocabVect;
}