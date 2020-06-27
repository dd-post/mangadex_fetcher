#include "arg.h"
#include "message.h"

// C++
#include <regex>

error_t parse_opt(int key, char* arg, argp_state* state) {
    struct arg_struct* as = (struct arg_struct*)state->input;

    switch (key) {
        case 's': {
            if (as->start_chap != -1) {
                pquit(128, "Chapter operations are single use. Please remove the extra specifications.\n");
            }

            char* endptr;
            int chap = strtol(arg, &endptr, 10);

            if ((chap == 0 && errno != 0) || (*endptr != '\0') || (chap < 0)) {
                pquit(128, "Unable to read interger for start chapter.\n");
            }

            as->start_chap = chap;

            break;
        }
        case 'e': {
            if (as->end_chap != -1) {
                pquit(128, "Chapter operations are single use. Please remove the extra specifications.\n");
            }

            char* endptr;
            int chap = strtol(arg, &endptr, 10);

            if ((chap == 0 && errno != 0) || (*endptr != '\0') || (chap < 0)) {
                pquit(128, "Unable to read interger for end chapter.\n");
            }

            as->end_chap = chap;

            break;
        }
        case 'S': {
            if (as->start_chap != -1) {
                pquit(128, "Chapter operations are single use. Please remove the extra specifications.\n");
            }

            char* endptr;
            int chap = strtol(arg, &endptr, 10);

            if ((chap == 0 && errno != 0) || (*endptr != '\0') || (chap < 0)) {
                pquit(128, "Unable to read interger for single chapter.\n");
            }

            as->start_chap = as->end_chap = chap;

            break;

        }
        case 'L': {
            as->list_chap = true;
            break;
        }
        case 'd': {
            if (!as->output_path.empty()) {
                pquit(128, "--output-dir has be specified two or more times. Please remove the extra specifications.\n");
            }

            as->output_path = std::string(arg);
            break;
        }
        case 't': {
            if (!as->output_type.empty()) {
                pquit(128, "--output-type has be specified two or more times. Please remove the extra specifications.\n");
            }

            as->output_type = std::string(arg);

            if (as->output_type != "cbz" && as->output_type != "dir") {
                pquit(128, "Unknown output type '%s',\n", arg);
            }

            break;
        }
        case 'l': {
            if (!as->lang_code.empty()) {
                pquit(128, "--language has be specified two or more times. Please remove the extra specifications.\n");
            }

            as->lang_code = std::string(arg);
            break;
        }
        case 'g': {
            as->groups.push_back(std::string(arg));
            break;
        }
        case 'v': {
            as->by_volume = true;
            break;
        }
        case 'q': {
            fclose(stdout);
            break;
        }
        case 'r': {
            as->one_scan = true;
            break;
        }
        case 'f': {
            as->force = true;
            break;
        }

        case ARGP_KEY_ARG: {
            if (state->arg_num > 1) argp_usage(state);

            // Test our url.
            std::string in_url = std::string(arg);

            // Use regex to pull out our title id.
            if (std::regex_match(in_url, std::regex(MD_SITE_REGEX))) {
                size_t url_pos = 0;
                std::string token;

                for (;;) {
                    url_pos = in_url.find("/");
                    if (url_pos == std::string::npos) break;
                    token = in_url.substr(0, url_pos);

                    if (std::regex_match(token, std::regex("[0-9]+"))) {
                        in_url = token;
                        break;
                    }

                    in_url.erase(0, url_pos + 1);
                }
            }
            // Otherwise this is an error.
            else {
                pquit(128, "Unable to parse URL, please check it for errors. You can use the full URL or just the title ID.\n");
                return ARGP_KEY_ERROR;
            }

            as->url = MD_API_TITLE + in_url;


            break;
        }
        case ARGP_KEY_END: {
            if (state->arg_num < 1) argp_usage(state);
            else as->valid = true;
            break;
        }

        default: {
            return ARGP_ERR_UNKNOWN;
        }
    }

    return 0;
}
