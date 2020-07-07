#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
  const int hours = seconds/3600;
  const int mins = (seconds % 3600)/60;
  const int secs = seconds % 60;
  return AddZeros(hours) + ":" + AddZeros(mins) + ":" + AddZeros(secs);
}

std::string Format::AddZeros(const int n) {
  std::string str = std::to_string(n);
  while(str.length() < 2) {
    str = "0"+str;
  }
  return str;
}
