#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() { GetLinuxParsesForJiffies(prev_jiffies_); }

// Return the aggregate CPU utilization
float Processor::Utilization() {
  GetLinuxParsesForJiffies(cur_jiffies_);
  CalculateDeltaTime(cur_jiffies_, prev_jiffies_);
  const float utilization = CalculateUtilization();
  cur_jiffies_.swap(prev_jiffies_);
  return utilization;
}

void Processor::CalculateDeltaTime(array<long, 3>& first_jiffies,
                                   array<long, 3>& second_jiffies) {
  for (int i = 0; i < 3; ++i) {
    delta_jiffies_[i] = first_jiffies[i] - second_jiffies[i];
  }
}

void Processor::GetLinuxParsesForJiffies(array<long, 3>& jiffies) {
  jiffies[0] = LinuxParser::IdleJiffies();
  jiffies[1] = LinuxParser::ActiveJiffies();
  jiffies[2] = LinuxParser::Jiffies();
}

float Processor::CalculateUtilization() {
  // small_number_buffer to prevent divide by zeros
  const float small_number_buffer = 0.0000000001;
  const float utilization =
      delta_jiffies_[1] /
      (delta_jiffies_[2] + small_number_buffer);
  return utilization;
}

long Processor::getTotalJiffies() {
  return cur_jiffies_[2];
}

void Processor::UpdatePrevJiffies() {
  for (size_t i = 0; i < prev_jiffies_.size(); ++i) {
    prev_jiffies_[i] = cur_jiffies_[i];
  }
}
