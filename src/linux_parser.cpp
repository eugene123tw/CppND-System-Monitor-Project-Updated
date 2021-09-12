#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <algorithm>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

template <typename T>
T findValueByKey(std::string const& keyFilter, std::string const& filename) {
  std::string line, key;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const& filename) {
  std::string line;
  T value;

  std::ifstream stream(LinuxParser::kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  float free_mem = findValueByKey<float>(filterMemFreeString, kMeminfoFilename);
  float total_mem =
      findValueByKey<float>(filterMemTotalString, kMeminfoFilename);
  return (total_mem - free_mem) / total_mem;
}

long LinuxParser::UpTime() {
  long uptime = getValueOfFile<long>(kUptimeFilename);
  return uptime;
}

long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::stringstream linestream;
  string line;

  int pid_int;
  string comm;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  unsigned int flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> pid_int >> comm >> state >> ppid >> pgrp >> session >>
        tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
        utime >> stime;
  }
  return (utime + stime) / sysconf(_SC_CLK_TCK);
}

vector<string> LinuxParser::CpuUtilization() {
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  string key;
  vector<string> line_vec;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> key) {
      line_vec.emplace_back(key);
    }
  }
  return line_vec;
}

int LinuxParser::TotalProcesses() {
  int value = findValueByKey<int>(filterProcesses, kStatFilename);
  return value;
}

int LinuxParser::RunningProcesses() {
  int value = findValueByKey<int>(filterRunningProcesses, kStatFilename);
  return value;
}

string LinuxParser::Command(int pid) {
  string value = getValueOfFile<string>(std::to_string(pid) + kCmdlineFilename);
  return value;
}

string LinuxParser::Ram(int pid) {
  string value = findValueByKey<string>(filterProcMem,
                                        std::to_string(pid) + kStatusFilename);
  return value.size() > 3 ? value.substr(0, value.size() - 3) : 0;
}

string LinuxParser::Uid(int pid) {
  string value =
      findValueByKey<string>(filterUID, std::to_string(pid) + kStatusFilename);
  return value;
}

string LinuxParser::User(int pid) {
  string key;
  string value;
  string line;
  string uid = Uid(pid);
  string username{""};

  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);

      std::getline(linestream, username, ':');
      std::getline(linestream, key, ':');
      std::getline(linestream, key, ':');
      if (key == uid) return username;
    }
  }
  return username;
}

long LinuxParser::UpTime(int pid) {
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  string line;

  int pid_int;
  string comm;
  char state;
  int ppid;
  int pgrp;
  int session;
  int tty_nr;
  int tpgid;
  unsigned int flags;
  unsigned long minflt;
  unsigned long cminflt;
  unsigned long majflt;
  unsigned long cmajflt;
  unsigned long utime;
  unsigned long stime;
  long int cutime;
  long int cstime;
  long int priority;
  long int nice;
  long int num_threads;
  long int itrealvalue;
  long long unsigned int starttime;

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> pid_int >> comm >> state >> ppid >> pgrp >> session >>
        tty_nr >> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt >>
        utime >> stime >> cutime >> cstime >> priority >> nice >> num_threads >>
        itrealvalue >> starttime;
  }
  return LinuxParser::UpTime() - (starttime / sysconf(_SC_CLK_TCK));
}
