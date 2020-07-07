#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
Processor::Processor() : old_t_(LinuxParser::Jiffies()), 
                         old_i_(LinuxParser::IdleJiffies()) {}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  long new_t, new_i;
  new_t = LinuxParser::Jiffies();
  new_i = LinuxParser::IdleJiffies();

  float delta_t = float(new_t) - float(old_t_);
  float delta_i = float(new_i) - float(old_i_);
  
  old_t_ = new_t;
  old_i_ = new_i;

  float utilization = (delta_t - delta_i) / delta_t;
  return utilization;
}
