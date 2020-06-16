// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// Curl
#include<curl/curl.h>

// Local
#include "message.h"
#include "scrape.h"
#include "sort.h"
#include "arg.h"

int main(int argc, char* argv[]) {
    setenv(ARGP_HELP_FMT_EVAR, ARGP_HELP_FMT_FMT, 0);

    // Parse our args.
    struct arg_struct parsed_args;
    argp_parse(&arg_params, argc, argv, 0, nullptr, &parsed_args);
    if (!parsed_args.valid) return 128;

    curl_global_init(CURL_GLOBAL_ALL);

    nlohmann::json j = fetch_json(parsed_args.url);

    print_chapters(j, parsed_args);

    //printf("%s\n", j.dump(4).c_str());


    //scrape_title(j, "gb");

    ////scrape_image("https://s5.mangadex.org/data/2b1ae74e6fb4302b5d0fa3cb7cc1f9d2/x1.png", "x1.png");

    // Cleanup.
    curl_global_cleanup();
    return 0;
}
