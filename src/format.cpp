#include "format.h"

#include <string>

using std::string;

string Format::ElapsedTime(long seconds) {
  int total_minute = seconds / 60;
  int total_hour = total_minute / 60;

  string hour = (total_hour % 60) > 9 ? std::to_string(total_hour % 60)
                                      : "0" + std::to_string(total_hour % 60);
  string minute = (total_minute % 60) > 9
                      ? std::to_string(total_minute % 60)
                      : "0" + std::to_string(total_minute % 60);
  string second = (seconds % 60) > 9 ? std::to_string(seconds % 60)
                                     : "0" + std::to_string(seconds % 60);

  string time = hour + ":" + minute + ":" + second;
  return time;
}