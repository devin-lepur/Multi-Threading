# Multi-Threading
Author: Devin Lepur
Multi threaded file processor

This project is similar to my Trie Tree project in that it
counts the number of substrings of a line of a given testfile
in a vocabulary file. This differs in that it uses POSIX
threading to complete tasks simultaneously as well as
provide a dynamically updated progress bar as the program
runs.

The "main" file calls the three worker thread files
"readlines", "readvocab", and "countvocabstrings". Readlines
and readvocab process words from the input files inserting
them into a dynamic vector and queue respectively. Both lock
the mutex as they manipulate the data to ensure data integrity.
Countvocabstring busy waits until the completion of readlines
after which, it begins creating a trie tree with all possible
substrings of each element of the queue, also locking the mutex,
and then searching each word in the vector and keeping count.
This count is then output to an output file if the count is
higher than a default value or an optional argument value.

sample files are provided for inputs; "vocabulary.txt" and
"testfile.txt" as well as a samlple output of the program.

Usage:
./countvocabstrings vocabulary.txt testfile.txt -p 40 -m 5 -v 3
Note:
-p controls the number of progress marks on the progress bar.
-m places a "#" every N character of the progress bar.
-v sets the threshold for mininum count required to output.
