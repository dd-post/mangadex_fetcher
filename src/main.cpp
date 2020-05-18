// C
#include <stdio.h>
#include <unistd.h>

// Curl
#include<curl/curl.h>

// Local
#include "message.h"
#include "scrape.h"
#include "arg.h"

int main(int argc, char* argv[]) {
    struct arguments parsed_args;
    argp_parse(&argp, argc, argv, 0, 0, &parsed_args);

    // TODO either custom print help or setting env vars.



    //curl_global_init(CURL_GLOBAL_ALL);
    //nlohmann::json j = fetch_json("https://mangadex.org/api/manga/22369");

    ////printf("%s\n", j.dump(4).c_str());

    //scrape_title(j, "gb");

    ////scrape_image("https://s5.mangadex.org/data/2b1ae74e6fb4302b5d0fa3cb7cc1f9d2/x1.png", "x1.png");

    //curl_global_cleanup();
    return 0;
}
