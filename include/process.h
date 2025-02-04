#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) { this->CpuUtilization(); }
  int Pid();
  std::string User();
  std::string Command();
  double CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_{0};
  float cpu_util_{0};
};

#endif