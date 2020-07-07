#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <array>
#include <vector>

using std::vector;
using std::array;

class Processor {
 public:
  Processor();
  float Utilization();
  long getTotalJiffies();

 private:
  void CalculateDeltaTime(array<long, 3>&, array<long, 3>&);
  virtual void GetLinuxParsesForJiffies(array<long, 3>&);
  float CalculateUtilization();
  void UpdatePrevJiffies();

  // cpu_jiffies arrays are organised with [0] = ActiveJiffies [1] =
  // NonIdleJiffies [2] = TotalJiffies
  array<long, 3> cur_jiffies_;
  array<long, 3> prev_jiffies_;
  array<float, 3> delta_jiffies_;
};


#endif
