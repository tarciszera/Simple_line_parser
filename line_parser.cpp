// Copyright 2022 Tarcis Aurélio Becher
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <chrono>
#include <ctime>

#include "ansi_colors.h"

#define VERSION 1.5

/// Help message
void usage(void) {
  std::cout << "Line parser, version " << VERSION << "\n"
            << "This program find a pattern on src_file\n"
            << "and write all line containing this pattern\n"
            << "appending it to dst_file.\n"
            << "Usage: \n"
            << "\tline_parser src_file dst_file pattern\n";
}

/// Main program
int main(int argc, char **argv) {

  /// Check for args quantity
  if (argc != 4) {
    std::cerr <<  COLORED_TEXT(RED, "Wrong arguments number")  << "\n";
    usage();
    return 1;
  }

  /// Getting args
  const std::string input_file(argv[1]), output_file(argv[2]);
  const std::string pattern(argv[3]);

  /// Init variables
  std::fstream fs;
  std::stringstream file_ss;

  /// Open file and write it to a buffer and then closing it
  fs.open(input_file, std::fstream::in);
  if (!fs.good()) {
    std::cerr << COLORED_TEXT(RED, "src_file not found") << "\n";
    return 1;
  }

  file_ss << fs.rdbuf();
  const std::string file_buffer(file_ss.str());
  fs.close();

  /// Opening output file
  fs.open(output_file, std::fstream::out | std::fstream::app);
  if (!fs.good()) {
    std::cerr << COLORED_TEXT(RED, "Coudn't open dst_file") << "\n";
    return 1;
  }

  /// Creating regex to match a line with pattern
  const std::string reg_pattern = "[^\\s].*" + pattern + ".*[^\\s]";
  const std::regex regex_parse_line(reg_pattern);

  /// Getting matchs from buffer
  std::cout << "Searching for matchs on file '" << input_file << "'...\n";

  std::sregex_iterator current_match(file_buffer.begin(), file_buffer.end(),
                                     regex_parse_line);
  std::sregex_iterator last_match;
  std::string previous_match("");

  /// Check if there's no matchs
  const int lines_match = std::distance(current_match, last_match);
  if (lines_match <= 0) {
    std::cerr << COLORED_TEXT(YELLOW, "Pattern not found") << "\n";
    return 0;
  }

  std::cout << "Found " << lines_match << " lines containing "
            << "'" << pattern << "'\n";

  /// Saving matchs on file
  std::cout << "Saving matchs on file '" << output_file << "'...\n";

  /// Save matchs on file
  std::time_t now = time(NULL);
  fs << "On '" << input_file << "' at "
     << std::ctime(&now);
  fs << "Found '" << pattern << "' at " << lines_match
     << " lines as following:\n\n";

  std::smatch match;
  do {
    match = *current_match;
    if (previous_match.compare(match.str())) {
      fs << match.str() << "\n";
      previous_match = match.str();
    }
  } while (++current_match != last_match);

  std::cout << COLORED_TEXT(GREEN, "Done!") << "\n";

  /// Close and save output file
  fs.close();

  return 0;
}
