#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using namespace std;

int Process::Pid() { return pid_; }

double Process::CpuUtilization() {
  cpu_util_ =
      (LinuxParser::ActiveJiffies(pid_) / (LinuxParser::UpTime(pid_) + 1e-9));
  return cpu_util_;
}

string Process::Command() {
  string cmd = LinuxParser::Command(pid_);
  return cmd.size() > 40 ? cmd.substr(0, 40) + "..." : cmd;
}

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

bool Process::operator<(Process const& a) const {
  return this->cpu_util_ < a.cpu_util_;
}