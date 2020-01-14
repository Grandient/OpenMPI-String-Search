// Sequential search example in C++
#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>

using namespace std;

// Searches for all the indexes where the substrings in a vector occur in a file
int search(vector<string> substring_vec, string file) {
  for (int i = 0; i < substring_vec.size(); i++) {
    bool found = false;
    string search_str = substring_vec.at(i);

    cout << "\"" << search_str << "\" found at index(es):" << endl;
    for (int j = 0; j < file.length(); j++) {
      if (file.substr(j, search_str.length()) == search_str) {
        cout << j << " ";
        found = true;
      }
    }
    cout << endl;

    // string not found in file
    if (!found) {
      cout << search_str << " does not occur in file " << endl;
    }
  }
}

int main() {
  // vector of multiple strings being searched for
  vector<string> substring_vec;
  substring_vec.push_back("Lorem ipsum");
  substring_vec.push_back("dolor sit");
  substring_vec.push_back("sad");
  // file being searched
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

  double start_time = omp_get_wtime();

  // call search to search for strings in file
  search(substring_vec, entire_file);

  double end_time = omp_get_wtime();

  // Print timing results
  printf("Search executed in %lf seconds\n", end_time - start_time);
  return 0;
}
