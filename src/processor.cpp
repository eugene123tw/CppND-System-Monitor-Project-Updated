#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::vector;

float Processor::Utilization() {
  vector<string> line_vec = LinuxParser::CpuUtilization();
  vector<float> numbers;

  for (size_t i = 0; i < line_vec.size(); i++) {
    if (i > 0) numbers.emplace_back(std::stof(line_vec[i]));
  }

  idle_ = numbers[LinuxParser::CPUStates::kIdle_] +
          numbers[LinuxParser::CPUStates::kIOwait_];
  non_idle_ = numbers[LinuxParser::CPUStates::kUser_] +
              numbers[LinuxParser::CPUStates::kNice_] +
              numbers[LinuxParser::CPUStates::kSystem_] +
              numbers[LinuxParser::CPUStates::kIRQ_] +
              numbers[LinuxParser::CPUStates::kSoftIRQ_] +
              numbers[LinuxParser::CPUStates::kSteal_];
  total_ = idle_ + non_idle_;
  total_delta_ = total_ - prev_total_;
  idle_delta_ = idle_ - prev_idle_;
  util_ = (total_delta_ - idle_delta_) / total_delta_;
  prev_idle_ = idle_;
  prev_non_idle_ = non_idle_;
  prev_total_ = total_;

  return util_;
}
