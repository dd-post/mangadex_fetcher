// C
#include <stdio.h>
#include <unistd.h>

// Curl
#include<curl/curl.h>

// Local
#include "message.h"
#include "scrape.h"

int main(int argc, char* argv[]) {
    //if (argc == 1) pquit(128, D_USAGE, argv[0]);

    curl_global_init(CURL_GLOBAL_ALL);
    nlohmann::json j = fetch_json("https://mangadex.org/api/manga/41243");

    //printf("%s\n", j.dump(4).c_str());

    scrape_title(j, "gb");

    //scrape_image("https://s5.mangadex.org/data/2b1ae74e6fb4302b5d0fa3cb7cc1f9d2/x1.png", "x1.png");

    curl_global_cleanup();
    return 0;
}