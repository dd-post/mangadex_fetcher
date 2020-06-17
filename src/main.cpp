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

    if (parsed_args.list_chap) {
        print_chapters(j, parsed_args);
        goto cleanup;
    }

    scrape_title(j, parsed_args);

    cleanup:
    curl_global_cleanup();
    return 0;
}
