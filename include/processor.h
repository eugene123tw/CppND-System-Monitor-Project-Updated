#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  double Utilization();

 private:
  double prev_idle_ = 0.0;
  double prev_non_idle_ = 0.0;
  double prev_total_ = 0.0;
  double idle_ = 0.0;
  double non_idle_ = 0.0;
  double total_ = 0.0;
  double total_delta_ = 0.0;
  double idle_delta_ = 0.0;
  double util_ = 0.0;
};

#endif