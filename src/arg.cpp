#include "arg.h"
#include "message.h"

// C++
#include <regex>

error_t parse_opt(int key, char* arg, argp_state* state) {
    struct arg_struct* as = (struct arg_struct*)state->input;

    switch (key) {
        case 's': {
            char* endptr;
            int chap = strtol(arg, &endptr, 10);

            if ((chap == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read interger for start chapter. Did you make a typo?\n");
            }

            as->start_chap = chap;

            break;
        }
        case 'e': {
            char* endptr;
            int chap = strtol(arg, &endptr, 10);

            if ((chap == 0 && errno != 0) || (*endptr != '\0')) {
                pquit(128, "Unable to read interger for end chapter. Did you make a typo?\n");
            }

            as->end_chap = chap;

            break;
        }
        case 'L': {
            as->list_chap = true;
            break;
        }
        case 'd': {
            as->output_path = std::string(arg);
            break;
        }
        case 'f': {
            as->output_type = std::string(arg);
            break;
        }
        case 'l': {
            as->lang_code = std::string(arg);
            break;
        }
        case 'g': {
            as->group = std::string(arg);
            break;
        }
        case 'v': {
            as->by_volume = true;
            break;
        }
        case 'q': {
            as->output = nullptr;
            break;
        }
        case 'D': {
            as->no_write = true;
            break;
        }
        case 'k': {
            as->keep_temp = true;
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
                printf("Unable to parse URL, please check it for errors. You can use the full URL or just the title ID.\n");
                return ARGP_KEY_ERROR;
            }

            as->url = MD_API_TITLE + in_url;

            printf("Using API url: %s\n", as->url.c_str());


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
