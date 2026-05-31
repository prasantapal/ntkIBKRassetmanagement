#ifndef VANILLA_HEADERS_HPP
#define VANILLA_HEADERS_HPP
////////////////////////////////////////
enum InputDataType {audio, eeg};
////////////////////////////////////////

bool has_only_spaces(const std::string& str) {
  return str.find_first_not_of (' ') == str.npos;
}

std::string exec(std::string cmd) {//execute a system command and get result
  std::array<char, 128> buffer;
  std::string result = {""};
  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (!feof(pipe.get())) {
    if (fgets(buffer.data(), 128, pipe.get()) != NULL)
      result += buffer.data();

  }
  static char newline_char = '\n';
  /// remove newline
  std::string::size_type i = 0;
  while (i < result.length()) {
    i = result.find(newline_char, i);
    if (i == std::string::npos) {
      break;
    }
    result.erase(i);
  }

  return result;
}

#endif
