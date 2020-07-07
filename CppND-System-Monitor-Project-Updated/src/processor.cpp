#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization

float Processor::Utilization() {
  long new_t, new_i;
  
  new_t = LinuxParser::Jiffies();
  new_i = LinuxParser::IdleJiffies();
  
  float delta_t = float(new_t) - float(old_t);
  float delta_i = float(new_i) - float(old_i);
  
  old_t = new_t;
  old_i = new_i;
  
  float utilization = (delta_t - delta_i) / delta_t;
  return utilization;
}
