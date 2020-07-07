#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key;
  float value, mem_t, mem_f, cached, buff;
  
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {  //only looking at specific values
          mem_t = value;
        }
        else if (key == "MemFree:") {
          mem_f = value;
        }
        else if (key == "Buffers:") {
          buff = value;
        }
        else if (key == "Cached:") {
          cached = value;
          break;
        }
      }
    }
  }
  float utilization = (mem_t - mem_f - buff - cached) / mem_t; 
  return utilization;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  long value{0};
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  return value;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return ActiveJiffies() + IdleJiffies();
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { 
  string line, key;
  long int value, a_jifs{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::stringstream linestream(line);
    for (int i=0; i<13; ++i) {
      linestream >> key;
    }
    for (int i=0; i < 4; ++i) { //only interested in 13:17 values
      linestream >> value;
      a_jifs += value;
    }
    return a_jifs;
  }
  return a_jifs;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  string line, key;
  long state, a_jifs;
  vector<int> cpu_states{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::stringstream linestream(line);
    linestream >> key;  //pop leading string
    while (linestream >> state) {
      cpu_states.push_back(state);
    }
  }
  a_jifs = cpu_states.at(CPUStates::kUser_) + cpu_states.at(CPUStates::kNice_) + 
           cpu_states.at(CPUStates::kSystem_) + cpu_states.at(CPUStates::kIRQ_) + 
           cpu_states.at(CPUStates::kSoftIRQ_) + cpu_states.at(CPUStates::kSteal_);
  return a_jifs;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, key;
  long state, i_jifs{0};
  vector<int> cpu_states{};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    linestream >> key;  //pop leading
    while (linestream >> state) {
      cpu_states.push_back(state);
    }
  }
  i_jifs = cpu_states.at(CPUStates::kIdle_) + cpu_states.at(CPUStates::kIOwait_);
  return i_jifs;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, key, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while (linestream >> value) {
      jiffies.push_back(value);
    }
  }
  return jiffies;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream,line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key;
  int value{0};
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(std::getline(stream,line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        return value;
      }
    }
  }
  return value;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  string line{""};
  if (stream.is_open()) {
    std::getline(stream,line);
    return line;
  } 
  return line;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key;
  float value{0.0};
  if (stream.is_open()) {
    while (std::getline(stream,line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        const long mb_ram = value / 1000;
        return to_string(mb_ram);
      }
    }
  }
  return string("0");
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line, key, uid{""};
  if (stream.is_open()) {
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> uid;
      if (key == "Uid:") {
        return uid;
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  std::ifstream stream(LinuxParser::kPasswordPath);
  string line, username{""}, usr_id;
  if (stream.is_open()) {
    while(std::getline(stream,line)) {
      std::replace(line.begin(),line.end(),':', ' ');
      std::replace(line.begin(), line.end(), 'x', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> usr_id) {
        if (usr_id == uid) {
          return username;
        }
      }
    }
  }
  return username;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string key, line;
  long int value{0};
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream,line);
    std::istringstream linestream(line);
    for (int i=0; i<21; ++i) {
      linestream >> key;
    }
    linestream >> value;
    value = value / sysconf(_SC_CLK_TCK);
    return value;
  }
  return value;
}
