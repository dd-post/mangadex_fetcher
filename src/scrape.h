#include <nlohmann/json.hpp>

#include "arg.h"

// Use curl to get a json object from the api.
nlohmann::json fetch_json(std::string url);

// Get an image with curl. Filename is local to current directory, so chdir() there first!
int scrape_image(std::string url, std::string filename);

// Get a full chapter from its Json.
bool scrape_chapter(nlohmann::json& j);

// Get a full manga from its Json.
void scrape_title(nlohmann::json& j, struct arg_struct& as);

