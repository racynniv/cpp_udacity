#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();

 private:
  long old_i_;
  long old_t_;
};



#endif
