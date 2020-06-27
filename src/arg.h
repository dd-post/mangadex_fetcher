#ifndef ARG_H
#define ARG_H

#include <argp.h>
#include <string>
#include <vector>

// The default 80 cols of argp is a bit limiting. If user hasn't setup their own
// ARGP_HELP_FMT variable, change it to be wider. Will not replace if already set.
#define ARGP_HELP_FMT_EVAR "ARGP_HELP_FMT"
#define ARGP_HELP_FMT_FMT  "rmargin=180, opt-doc-col=40, no-dup-args-note"

//const char* argp_program_version = "mangadex_scrapper - v0.01";
const char prog_doc[] = "Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to plain directories or cbz."; // add pdf and epub when possible
static char args_doc[] = "URL";

struct arg_struct {
    std::string url;
    int start_chap = -1;
    int end_chap = -1;
    bool list_chap = false;
    std::string output_path;
    std::string output_type;
    std::string lang_code;
    std::vector<std::string> groups;
    bool by_volume = false;
    bool valid = false;
    bool one_scan = false;
    bool force = false;
};

const struct argp_option arg_options[] = {
    {"start-chapter",   's', "START_CHAP",  0,  "Specify the chapter to start fetch from. Use indices from --list-chapters output",                                 0},
    {"end-chapter",     'e', "END_CHAP",    0,  "Specify the chapter to end fetch at. Use indices from --list-chapters output",                                     0},
    {"single-chapter",  'S', "CHAP",        0,  "Specify a single chapter to download. Use indices from --list-chapters output",                                    0},
    {"list-chapters",   'L', nullptr,       0,  "List all chapters and generate indices to reference them. Use --language and/or --group to narrow down output",    0},
    {"output-dir",      'd', "PATH",        0,  "Change the output directory for the downloaded chapters to PATH. Defaults to name of manga",                       0},
    {"output-type",     't', "TYPE",        0,  "Define how output will be saved. TYPE can be dir (default) or cbz",                                                0},
  //{"volume-format",   'f', "FORMAT",      0,  "Alter the format of output name when saving as a volume. See --help format for more details",                      0},
  //{"chapter-format",  'c', "FORMAT",      0,  "Alter the format of output name when saving as a chapter. See --help format for more details",                     0},
    {"language",        'l', "LANG_CODE",   0,  "Fetch chapters only matching this language",                                                                       0},
    {"group",           'g', "GROUP",       0,  "When multiple groups have translated a chapter, prefer this group's translation. Can be specified multiple times to form a priority list", 0},
    {"volume",          'v', nullptr,       0,  "Write out chapters in groups by volume",                                                                           0},
    {"quiet",           'q', nullptr,       0,  "Suppress all standard output",                                                                                     0},
    {"one-group",       'O', nullptr,       0,  "Get only the most recent version of a chapter. Use --language to narrow down output",                              0},
    {"force",           'f', nullptr,       0,  "Force overwrites of files. Be careful with this option as it will overwrite files if they share the same name",    0},
  //{"dry-run",         'D', nullptr,       0,  "Perform a dry run to display what directories will be created",                                                    0},
  //{"keep-files",      'k', nullptr,       0,  "Keep any temporary files",                                                                                         0}, // TODO: deleting anything is a potential hazard.
    { 0 }
};

error_t parse_opt(int key, char* arg, struct argp_state* state);

const struct argp arg_params = {arg_options, &parse_opt, args_doc, prog_doc};

// Old help for reference.
/*
#define ESC_CLR_RED     "\033[0;31m"
#define ESC_CLR_GREEN   "\033[0;32m"
#define ESC_CLR_YELLOW  "\033[0;33m"
#define ESC_CLR_CYAN    "\033[0;36m"
#define ESC_CLR_RESET   "\033[0m"

#define D_USAGE \
"Usage: %s [options] <url>\n\n"\
"Simple tool for scrapping manga from mangadex.org using their API (https://mangadex.org/api/). Supports writing to folder, cbz, or epub.\n\n" \
ESC_CLR_GREEN "Options:\n" ESC_CLR_RESET \
  " -V, --version"                                                     "\t\t\tPrint the version string.\n"\
  " -c, --chapter "        ESC_CLR_RED "<a> <b>"   ESC_CLR_RESET       "\t\tSpecify the chapter(s) to fetch. Only " ESC_CLR_RED "<a>" ESC_CLR_RESET " must be specified (single chapter download). If " ESC_CLR_RED "<b>" ESC_CLR_RESET " is '-',\n"\
                                                                       "\t\t\t\tthe downloader will attempt to grab all chapters starting from " ESC_CLR_RED "<a>" ESC_CLR_RESET ". Use indices from --list-chapters output.\n"\
  " -L, --list-chapters"                                               "\t\tList all chapters and their indices.\n"\
  " -d, --output-dir "     ESC_CLR_RED "<path>"    ESC_CLR_RESET       "\tChange the output directory for the downloaded chapters.\n"\
  " -o, --output-type "    ESC_CLR_RED "<type>"    ESC_CLR_RESET       "\tChange the file type that chapters get saved in. " ESC_CLR_RED "<type>" ESC_CLR_RESET " can be folder, cbz, or epub. Defaults to folder.\n"\
  " -f, --output-format "  ESC_CLR_RED "<format>"  ESC_CLR_RESET       "\tChange the format of the output. See '--help format' for more details.\n"\
  " -l, --language "       ESC_CLR_RED "<language_code>" ESC_CLR_RESET "\tFetch chapters only matching this language. See '--help language' for more details.\n"\
  " -g, --group "          ESC_CLR_RED "<group>"   ESC_CLR_RESET       "\tWhen multiple groups have translated a chapter, perfer this one.\n"\
  " -v, --volume"                                                      "\t\t\tGroup chapters together by volume.\n"\
  " -h, --help "           ESC_CLR_RED "<topic>"   ESC_CLR_RESET       "\t\tThis help message or help on a topic.\n"
*/

#endif // ARG_H
