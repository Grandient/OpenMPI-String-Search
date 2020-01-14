// C++ search parallelized using OpenMP
// Splitting up the file based on how many threads we have
#include <stdio.h>
#include <omp.h>
#include <iostream>
#include <fstream>
#include <vector>

#define NUM_THREADS 4

using namespace std;
// Searches for all the indexes where the substrings in a vector occur in a file
int search(vector<string> substring_vec, string file) {
  // Divide the file by the number of threads
  int step_div = file.length() / (NUM_THREADS + 1);

  for (int i = 0; i < substring_vec.size(); i++) {
    bool found = false;
    string search_str = substring_vec.at(i);
    // Need to keep print "ordered" so output indexes make sense
    cout << "\"" << search_str << "\" found at index(es):" << endl;

    #pragma omp parallel
    {
      // Get thread ID
      int threadID = omp_get_thread_num();
      // Get indexes of the chunks of text
      long thread_start = step_div * threadID;
      long thread_end = step_div * (threadID + 1);
      int j = thread_start;
      for (j; j < thread_end; j++) {
        if (file.substr(j, search_str.length()) == search_str) {
          // printf used instead of cout because threads work in "random" order
          printf("%d ", j);
          found = true;
        }
      }
    }
    cout << endl;

    // String not found in file
    if (!found) {
      cout << search_str << " does not occur in file " << endl;
    }
  }

}

int main() {
  // Vector of multiple strings being searched for
  vector<string> substring_vec;
  substring_vec.push_back("Lorem ipsum");
  substring_vec.push_back("dolor sit");
  substring_vec.push_back("sad");

  // Set the number of threads
  omp_set_num_threads(NUM_THREADS);

  // File being searched
  string filename = "100_words.txt";
  // Set input file stream up
  ifstream file;
  file.open(filename);

  if (!file) {
    cout << "Unable to open file " << filename << endl;
    exit(1);
  }

  // Take in the entire file as a string
  string entire_file = {istreambuf_iterator<char>(file), istreambuf_iterator<char>()};
  file.close();

  // Timing starts:
  double start_time = omp_get_wtime();

  // call search to search for strings in file
  search(substring_vec, entire_file);

  double end_time = omp_get_wtime();

  // Print timing results
  printf("Search executed in %lf seconds\n", end_time - start_time);
  return 0;
}
