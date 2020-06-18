// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// C++
#include <filesystem>

// Curl
#include <curl/curl.h>

// Local
#include "message.h"
#include "scrape.h"
#include "sort.h"
#include "cbz.h"
#include "arg.h"

int main(int argc, char* argv[]) {
    setenv(ARGP_HELP_FMT_EVAR, ARGP_HELP_FMT_FMT, 0);

    struct arg_struct parsed_args;
    argp_parse(&arg_params, argc, argv, 0, nullptr, &parsed_args);
    if (!parsed_args.valid) return 128;

    printf("Using API url: %s\n", parsed_args.url.c_str());

    curl_global_init(CURL_GLOBAL_ALL);

    nlohmann::json j = fetch_json(parsed_args.url);
    std::string manga_title = j["manga"]["title"];

    // Sanitize output filename.
    std::replace(manga_title.begin(), manga_title.end(), '/', '_');
    j["manga"]["title"] = manga_title;

    if (parsed_args.list_chap) {
        print_chapters(j, parsed_args);
        goto cleanup;
    }

    scrape_title(j, parsed_args);

    if (parsed_args.output_type == "cbz") {
        printf("\nCollecting for archives...\n");

        if (parsed_args.by_volume) {
            for (auto jit : j["volumes"].items()) {
                std::string vol = jit.value();
                printf("\nPackaging volume: %s\n", vol.c_str());

                create_zip(vol, manga_title + " - Volume " + vol + ".cbz");
            }
        }
        else create_zip(".", manga_title + ".cbz");
        // Call to libarchive stuff.
    }

    cleanup:
    curl_global_cleanup();

    printf("\nFinished download of title '%s'.\n\n", manga_title.c_str());
    return 0;
}
