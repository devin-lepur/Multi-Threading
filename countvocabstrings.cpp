/* 
 * Author: Devin Lepur
 * RedID: 825148526
 * 9/28/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * countvocabstrings
 */

#include <iostream>
#include <sstream>
#include <fstream>
#include "shareddata.h"
#include "countvocabstrings.h"
#include "tree.h"

void* countvocabstringsThread(void* shared_data) {
    SHARED_DATA* data = static_cast<SHARED_DATA*>(shared_data);

    data->numOfProcessedLines = 0;

    //Check if output file is valid
    std::ofstream outputFile("countNumOfContainedVocab.txt");
        if(!outputFile.is_open()) {
            std::cerr << "Error: Unable to open output file" << std::endl;
            exit(EXIT_FAILURE);
        }

    std::string line;

    while(true) {
        if(data->taskCompleted[VOCABFILEINDEX]) {
            break;
        }
    }

    //Process lines from queue
    while(true) {
        pthread_mutex_lock(&data->queue_mutex);
        if(!data->lineQueue->empty()) {
            data->numOfProcessedLines++;
            line = data->lineQueue->front();
            data->lineQueue->pop();
            pthread_mutex_unlock(&data->queue_mutex);
        } else if (data->taskCompleted[TESTFILEINDEX]) {
            //All lines have been processed and readlines is done
            pthread_mutex_unlock(&data->queue_mutex);
            break;
        } else {
            //All lines have been processed but readlines is not done

            pthread_mutex_unlock(&data->queue_mutex);
            continue;
        }

        //Tree for searching words
        tree myTree;
        
        //Iterate through the words in the line and store substrings in tree
        std::istringstream iss(line);
        std::string word;
        //split line into words and insert all substrings of each word into tree
        while(iss >> word) {
            int length = 0;
            while(word[length] != '\0') {
                length++;
            }

            //Process line into all substrings and insert into tree
            //loop for the starting position of the substring
            for (int start = 0; start < length; start++) {
                //loop for the length of the substring
                for (int len = 1; len <= length - start; len++) {
                    //Extract the substring
                    char substring[len + 1];
                    for (int i = 0; i < len; i++) {
                        substring[i] = word[start + i];
                    }
                    substring[len] = '\0';
                    myTree.insert(substring);
                }
            }
        }

        //Count vocab words in each line
        unsigned int count = 0;
        for(const std::string& vocabWord : *data->vocabVect) {
            const char* vocabChar = vocabWord.c_str();
            if(myTree.search(vocabChar)) {
                count++;
            }
        }

        //output to file if count is greater than defined min count
        if(count > data->minNumOfVocabStringsContainedForPrinting) {
            outputFile << count << std::endl;
        }
        
        //deallocate tree
        myTree.deallocateTree(myTree.root);
    }
    return NULL;
}