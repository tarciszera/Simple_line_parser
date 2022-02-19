#ifndef __ANSI_COLORS_H__
#define __ANSI_COLORS_H__

#define CLEAR "0"
#define BOLD "1;"
#define UNDERLINE "4;"
#define RED "31"
#define GREEN "32"
#define YELLOW "33"
#define BLUE "34"
#define MAGENTA "35"
#define CYAN "36"
#define WHITE "37"

#define ANSI_CODE(code) "\033[" code "m"

#define COLORED_TEXT(color, text) ANSI_CODE(color) text ANSI_CODE(CLEAR)

#endif //__ANSI_COLORS_H__
