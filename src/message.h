// Header for messages in the program.
#include <stdarg.h>

#define ESC_CLR_RED     "\033[0;31m"
#define ESC_CLR_GREEN   "\033[0;32m"
#define ESC_CLR_YELLOW  "\033[0;33m"
#define ESC_CLR_CYAN    "\033[0;36m"
#define ESC_CLR_RESET   "\033[0m"

#define D_USAGE \
"Usage: %s [options] <url>\n\n"\
"Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to folder, cbz, or epub.\n\n"\
ESC_CLR_GREEN "Options:\n" ESC_CLR_RESET \
  " -V, --version"                                                     "\t\t\tPrint the version string.\n"\
  " -c, --chapter "        ESC_CLR_RED "<a> <b>"   ESC_CLR_RESET       "\t\tSpecify the chapter(s) to fetch. Only " ESC_CLR_RED "<a>" ESC_CLR_RESET " must be specified (single chapter download). If " ESC_CLR_RED "<b>" ESC_CLR_RESET " is '-',\n"\
                                                                       "\t\t\t\tthe downloader will attempt to grab all chapters starting from " ESC_CLR_RED "<a>" ESC_CLR_RESET ". Use indices from --list-chapters output.\n"\
  " -L, --list-chapters"                                               "\t\tList all chapters and their indices.\n"\
  " -d, --output-dir "     ESC_CLR_RED "<path>"    ESC_CLR_RESET       "\tChange the output directory for the downloaded chapters.\n"\
  " -o, --output-type "    ESC_CLR_RED "<type>"    ESC_CLR_RESET       "\tChange the file type that chapters get saved in. " ESC_CLR_RED "<type>" ESC_CLR_RESET " can be folder, cbz, or epub. Defaults to folder.\n"\
  " -f, --output-format "  ESC_CLR_RED "<format>"  ESC_CLR_RESET       "\tChange the format of the output. See '--help format' for more details.\n"\
  " -g, --group"                                                       "\t\t\tGroup chapters together by volume. Chapters with no assigned volume will use volume 0.\n"\
  " -v, --verbose"                                                     "\t\t\tGive more detailed output from CURL.\n"\
  " -h, --help "           ESC_CLR_RED "<topic>"   ESC_CLR_RESET       "\t\tThis help message or help on a topic.\n"

// Print a message and quit.
void pquit(int ret, const char* format, ...);

