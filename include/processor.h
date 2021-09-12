#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  float prev_idle_ = 0.0;
  float prev_non_idle_ = 0.0;
  float prev_total_ = 0.0;
  float idle_ = 0.0;
  float non_idle_ = 0.0;
  float total_ = 0.0;
  float total_delta_ = 0.0;
  float idle_delta_ = 0.0;
  float util_ = 0.0;
};

#endif