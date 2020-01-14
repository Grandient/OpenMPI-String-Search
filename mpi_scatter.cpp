#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 4 // the number of processors passed with flag -np

using namespace std;

// Searches for all the indexes where the substrings in a vector occur in a file
int search(vector<string> substring_vec, string file) {
    // Initialize MPI variables
    int numtasks, rank, sendcount, recvcount, source;
    // Divide the file by the number of communicators
    int step_div = file.length() / (SIZE);
    int KEYWORDS = substring_vec.size();
    char sendbuf[file.length() + 1];
    char recvbuf[step_div + 1];

    sendcount = step_div;
    recvcount = step_div + 1;
    source = 0;

    // Initialize World
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    strcpy(sendbuf, file.c_str());
    for (int i = 0; i < substring_vec.size(); i++){
        string search_str = substring_vec.at(i);
        MPI_Scatter(sendbuf, sendcount, MPI_CHAR, recvbuf, recvcount, MPI_CHAR, 0, MPI_COMM_WORLD);

        cout << "rank: " << rank << " \"" << search_str << "\" found at index(es):" << endl;
        string data(recvbuf);
        bool found = false;
        for (int i = 0; i < step_div; i++){
            if (data.substr(i, search_str.length()) == search_str){
                printf("%d ", i + (step_div * rank));
                found = true;
            }
        }
        cout << endl;
        if (!found){
            cout << search_str << " does not occur in rank " <<  rank << " part of the file" << endl;
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}

int main(int argc, char **argv)
{
    // Vector of strings
    vector<string> substring_vec;
    substring_vec.push_back("Lorem ipsum");
    substring_vec.push_back("dolor sit");
    substring_vec.push_back("sad");

    // File being searched
    string filename = "100_words.txt";
    // Set input file stream up
    ifstream file;
    file.open(filename);
    if (!file)
    {
        cout << "Unable to open file " << filename << endl;
        exit(1);
    }
    // Take in the entire file as a string
    string entire_file = {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
    file.close();

    // Timing starts:
    double start_time = MPI_Wtime();

    // call search to search for strings in file
    search(substring_vec, entire_file);

    double end_time = MPI_Wtime();

    // Print timing results
    printf("Search executed in %lf seconds\n", end_time - start_time);
    return 0;
}
