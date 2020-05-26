#include "arg.h"
#include "message.h"

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

        case ARGP_KEY_ARG: {
            if (state->arg_num > 1) argp_usage(state);

            // Test our url.
            std::string in_url = std::string(arg);

            // If this is a full url, chop off the ip and
            if (in_url.find("://") != std::string::npos) {

            }


            break;
        }
        case ARGP_KEY_END: {
            if (state->arg_num < 1) argp_usage(state);
            break;
        }

        default: {
            return ARGP_ERR_UNKNOWN;
        }
    }

    return 0;
}
