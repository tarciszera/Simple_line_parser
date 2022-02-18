#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

#define VERSION 1.2

/// Help message
void usage(void) {
  std::cout << "Line parser, version " << VERSION << "\n"
            << "Usage: "
            << "./line_parser src_file dst_file pattern\n";
}

/// Main program
int main(int argc, char **argv) {

  /// Check for args quantity
  if (argc != 4) {
    std::cerr << "Wrong arguments number\n";
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
  if (!fs.good()) {
    std::cout << "src_file not found\n";
    return 1;
  }

  file_ss << fs.rdbuf();
  std::string file_buffer(file_ss.str());
  fs.close();

  /// Creating regex to match a line with pattern
  std::string reg_pattern = "[^\\s].*" + pattern + ".*[^\\s]";
  std::regex regex_parse_line(reg_pattern);

  /// Getting matchs from buffer
  std::cout << "Searching for matchs on file '" << input_file << "'...\n";

  std::sregex_iterator current_match(file_buffer.begin(), file_buffer.end(),
                                     regex_parse_line);
  std::sregex_iterator last_match;
  std::string previous_match("");

  /// Check if there's no matchs
  int lines_match = std::distance(current_match, last_match);
  if (lines_match <= 0) {
    std::cout << "Pattern not found\n";
    return 0;
  }

  std::cout << "Found " << lines_match << " lines containing "
            << "'" << pattern << "'\n";

  /// Saving matchs on file
  std::cout << "Saving matchs on file '" << output_file << "'...\n";

  /// Opening output file
  fs.open(output_file, std::fstream::out | std::fstream::app);
  if (!fs.good()) {
    std::cout << "Coudn't open dst_file\n";
    return 1;
  }

  /// Save matchs on file
  std::smatch match;
  do {
    match = *current_match;
    if (previous_match.compare(match.str())) {
      fs << match.str() << "\n";
      previous_match = match.str();
    }
  } while (++current_match != last_match);

  std::cout << "Done!\n";

  /// Close and save output file
  fs.close();

  return 0;
}
