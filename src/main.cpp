// C
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <signal.h>

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
#include "sig.h"

std::string current_file = std::string();

int main(int argc, char* argv[]) {
    setenv(ARGP_HELP_FMT_EVAR, ARGP_HELP_FMT_FMT, 0);
    signal(SIGINT, &catch_sigint);

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

    printf("Title: %s\n", manga_title.c_str());

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
                std::string vol_cbz = manga_title + " - Volume " + vol + ".cbz";
                printf("\nPackaging volume: %s\n", vol.c_str());

                struct stat stat_buf;

                // If there was no dl, then check for existing volumes. No need to rebuild if there's nothing new.
                if (j["successful_dl"] == false && !stat(vol_cbz.c_str(), &stat_buf) && stat_buf.st_size != 0 && !parsed_args.force) {
                    printf("Skipping '%s': file already exists.\n", vol_cbz.c_str());
                    continue;
                }

                create_zip(vol, vol_cbz);
            }
        }
        else {
            std::string cbz = manga_title + ".cbz";
            struct stat stat_buf;

            if (j["successful_dl"] == false && !stat(cbz.c_str(), &stat_buf) && stat_buf.st_size != 0 && !parsed_args.force) {
                printf("Skipping '%s': file already exists.\n", cbz.c_str());
            }
            else create_zip(".", cbz);
        }
    }

    printf("\nFinished download of title '%s'.\n\n", manga_title.c_str());

    cleanup:
    curl_global_cleanup();

    return 0;
}
