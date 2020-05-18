#ifndef ARG_H
#define ARG_H

#include <argp.h>
#include <string>

//const char* argp_program_version = "mangadex_scrapper - v0.01";
const char doc[] = "Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to plain directories, pdf, cbz, or epub.";
static char args_doc[] = "URL";

struct arguments {
    std::string url;
    int start_chap = -1;
    int end_chap = -1;
    bool list_chap = false;
    std::string output_path;
    std::string output_type;
    std::string lang_code;
    std::string group;
    bool by_volume = false;
};

const struct argp_option options[] = {
    {"start-chapter",   's', "START_CHAP",  0,  "Specify the chapter to start fetch from. Use indices from --list-chapters output.",                               0},
    {"end-chapter",     'e', "END_CHAP",    0,  "Specify the chapter to end fetch at. Use indices from --list-chapters output.",                                   0},
    {"list-chapters",   'L', 0,             0,  "List all chapters and generate indices to reference them. Use --language to restrict output to a language.",      0},
    {"output-dir",      'd', "PATH",        0,  "Change the output directory for the downloaded chapters to PATH. Defaults to current working directory.",         0},
    {"output-type",     'f', "TYPE",        0,  "Define how output will be saved. TYPE can be folder (default), cbz, pdf, or epub.",                               0},
  //{"volume-format",   'f', "FORMAT",      0,  "Alter the format of output name when saving as a volume. See --help format for more details.",                    0},
  //{"chapter-format",  'c', "FORMAT",      0,  "Alter the format of output name when saving as a chapter. See --help format for more details",                    0},
    {"language",        'l', "LANG_CODE",   0,  "Fetch chapters only matching this language. See --help language for more details.",                               0},
    {"group",           'g', "GROUP",       0,  "When multiple groups have translated a chapter, perfer this group's translation.",                                0},
    {"volume",          'v', 0,             0,  "Output chapters in groups by volume.",                                                                            0},
    { 0 }
};

error_t parse_opt(int key, char* arg, struct argp_state* state);

const struct argp argp = {options, NULL, args_doc, doc};


// Old help for reference.
//#define ESC_CLR_RED     "\033[0;31m"
//#define ESC_CLR_GREEN   "\033[0;32m"
//#define ESC_CLR_YELLOW  "\033[0;33m"
//#define ESC_CLR_CYAN    "\033[0;36m"
//#define ESC_CLR_RESET   "\033[0m"
//
//#define D_USAGE \
//"Usage: %s [options] <url>\n\n"\
//"Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to folder, cbz, or epub.\n\n" \
//ESC_CLR_GREEN "Options:\n" ESC_CLR_RESET \
//  " -V, --version"                                                     "\t\t\tPrint the version string.\n"\
//  " -c, --chapter "        ESC_CLR_RED "<a> <b>"   ESC_CLR_RESET       "\t\tSpecify the chapter(s) to fetch. Only " ESC_CLR_RED "<a>" ESC_CLR_RESET " must be specified (single chapter download). If " ESC_CLR_RED "<b>" ESC_CLR_RESET " is '-',\n"\
//                                                                       "\t\t\t\tthe downloader will attempt to grab all chapters starting from " ESC_CLR_RED "<a>" ESC_CLR_RESET ". Use indices from --list-chapters output.\n"\
//  " -L, --list-chapters"                                               "\t\tList all chapters and their indices.\n"\
//  " -d, --output-dir "     ESC_CLR_RED "<path>"    ESC_CLR_RESET       "\tChange the output directory for the downloaded chapters.\n"\
//  " -o, --output-type "    ESC_CLR_RED "<type>"    ESC_CLR_RESET       "\tChange the file type that chapters get saved in. " ESC_CLR_RED "<type>" ESC_CLR_RESET " can be folder, cbz, or epub. Defaults to folder.\n"\
//  " -f, --output-format "  ESC_CLR_RED "<format>"  ESC_CLR_RESET       "\tChange the format of the output. See '--help format' for more details.\n"\
//  " -l, --language "       ESC_CLR_RED "<language_code>" ESC_CLR_RESET "\tFetch chapters only matching this language. See '--help language' for more details.\n"\
//  " -g, --group "          ESC_CLR_RED "<group>"   ESC_CLR_RESET       "\tWhen multiple groups have translated a chapter, perfer this one.\n"\
//  " -v, --volume"                                                      "\t\t\tGroup chapters together by volume.\n"\
//  " -h, --help "           ESC_CLR_RED "<topic>"   ESC_CLR_RESET       "\t\tThis help message or help on a topic.\n"

#endif // ARG_H