#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <regex>

#define VERSION 1.0

void usage(void) {
  std::cout
    << "Log parser, version " << VERSION << "\n"
    << "Usage: "<< "./logparser src_file dst_file pattern\n";
}

int main(int argc, char** argv) {

  if (argc < 4) {
    std::cerr << "Missing args\n";
    usage();
    return 1;
  }
  /// Getting args
  std::string input_file(argv[1]), output_file(argv[2]);
  std::string pattern(argv[3]);

  /// Init variables
  std::fstream fs;
  std::stringstream file_ss;

  /// Open file and write it to a buffer and then closing it
  fs.open(input_file, std::fstream::in);
  file_ss << fs.rdbuf();
  std::string file_buffer(file_ss.str());
  fs.close();

  /// Creating regex to match a line with pattern
  std::string reg_pattern = "[^\\s].*" + pattern + ".*\\s";
  std::regex regex_parse_line(reg_pattern);

  /// Opening output file
  fs.open(output_file, std::fstream::out | std::fstream::app);

  /// Getting matchs from buffer
  std::cout << "Searching for matchs on file '" << input_file << "'...\n";
  std::sregex_iterator current_match(file_buffer.begin(), file_buffer.end(),
                                     regex_parse_line);
  std::sregex_iterator last_match;
  std::cout << "Found " << std::distance(current_match, last_match)
    << " lines containing " << "'" << pattern << "'\n";

  /// Saving matchs on file
  std::cout << "Saving matchs on file '" << output_file << "'...\n";
  while (current_match != last_match) {
    std::smatch match = *current_match;
    fs << match.str();
    current_match++;
  }
  std::cout << "Done!\n";

  /// Close and save output file
  fs.close();
  return 0;
}
