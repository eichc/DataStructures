// A simple "caller ID" program

#include <iostream>
#include <vector>
#include<map>
#include <string>
using namespace std;

// add a number, name pair to the phonebook
void add(map<int, string> &phonebook, int number, string const& name) {
  phonebook[number] = name;
}

// given a phone number, determine who is calling
void identify(const map<int, string> & phonebook, int number) {
  map<int, string>::const_iterator itr = phonebook.find(number);
  if (itr == phonebook.end()) {
    cout << "unknown caller!" << endl;
  }
  else {
    cout << (itr->second) << " is calling!" << endl;
  }
}


int main() {
  // create the phonebook; initially all numbers are unassigned
  map<int, string> phonebook;

  // add several names to the phonebook
  add(phonebook, 1111, "fred");
  add(phonebook, 2222, "sally");
  add(phonebook, 3333, "george");
  add(phonebook, 3333, "michael");

  // test the phonebook
  identify(phonebook, 2222);
  identify(phonebook, 4444);
  identify(phonebook, 3333);
}