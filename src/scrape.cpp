#include "scrape.h"
#include "message.h"

// Curl
#include <curl/curl.h>

// C/Unix
#include <unistd.h>
#include <sys/stat.h>

#define MD_API_CHAP  "https://mangadex.org/api/chapter/"
#define MD_API_TITLE "https://mangadex.org/api/manga/"

// Code stolen from: https://curl.haxx.se/libcurl/c/getinmemory.html and https://curl.haxx.se/libcurl/c/url2file.html

struct mem_struct {
    uint8_t* mem;
    size_t size;
};

size_t write_mem_callback(void* data, size_t size, size_t nmemb, void* userp) {
    size_t realloc_size = size * nmemb;
    struct mem_struct* mem_st = (struct mem_struct*) userp;

    uint8_t* ptr = (uint8_t*)realloc(mem_st->mem, mem_st->size + realloc_size + 1);

    if (ptr == nullptr) {
        printf("Out of memory!\n");
        return 0;
    }

    mem_st->mem = ptr;

    memcpy(mem_st->mem + mem_st->size, data, realloc_size);
    mem_st->size += realloc_size;
    mem_st->mem[mem_st->size] = '\0';

    return realloc_size;
}

size_t write_file_callback(void* data, size_t size, size_t nmemb, void* userp) {
    return fwrite(data, size, nmemb, (FILE*)userp);
}

nlohmann::json fetch_json(std::string url) {
    nlohmann::json j;
    struct mem_struct dl_chunk;
    CURL* handle;
    CURLcode cret;

    // Setup the memory for the downloader.
    dl_chunk.mem = (uint8_t*)malloc(1);
    dl_chunk.size = 0;

    handle = curl_easy_init();
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_mem_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)(&dl_chunk));

    cret = curl_easy_perform(handle);

    if (cret != CURLE_OK) printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(cret));
    else j = nlohmann::json::parse((char*)dl_chunk.mem);

    curl_easy_cleanup(handle);
    free(dl_chunk.mem);

    return j;
}

int scrape_image(std::string url, std::string filename) {
    FILE* fptr;
    bool ret = 0;
    CURL* handle;
    CURLcode cret;
    struct stat stat_buf;

    // If file exists, don't bother redownloading it.

    if (!stat(filename.c_str(), &stat_buf)) {
        if (stat_buf.st_size != 0) {
            printf("Skipping '%s': file already exists.\n", filename.c_str());
            return 1;
        }
    }

    fptr = fopen(filename.c_str(), "wb");
    if (!fptr) return false;

    handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_file_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)(fptr));

    cret = curl_easy_perform(handle);

    if (cret != CURLE_OK) {
        printf("curl_easy_perform() failed: %s\n", curl_easy_strerror(cret));
        ret = -1;
    }

    curl_easy_cleanup(handle);
    fclose(fptr);

    return ret;
}


void scrape_chapter(nlohmann::json j) {
    uint64_t    chap_id     = j["id"];
    std::string chap_hash   = j["hash"];
    std::string volume      = j["volume"];
    std::string chapter     = j["chapter"];
    std::string chap_title  = j["title"];
    std::string chap_server = j["server"];
    nlohmann::json pg_array = j["page_array"];

    uint64_t n_zeros = 3;
    std::string dir_name;

    if (chapter.find(".") == std::string::npos) {
        dir_name = std::string(n_zeros - chapter.length(), '0').append(chapter);
    }
    else {
        size_t pos = chapter.find(".");
        std::string frac = chapter.substr(pos);
        std::string whole = chapter.substr(0, pos);

        dir_name = std::string(n_zeros - whole.length(), '0').append(whole);
        dir_name.append(frac);
    }

    // Make the directory and start downloading files.
    if (mkdir(dir_name.c_str(), 0755) < 0) {
        if (errno != EEXIST) pquit(-1, "Couldn't make directory '%s': %s\n", dir_name.c_str(), strerror(errno));
    }
    chdir(dir_name.c_str());

    for (nlohmann::json::iterator it = pg_array.begin(); it != pg_array.end(); it++) {
        std::string fn = it->get<std::string>();
        std::string image_url = chap_server + chap_hash + "/" + fn;
        printf("Fetching chapter '%s' - '%s'\n", dir_name.c_str(), fn.c_str());

        // Try to scrape 5 times before moving on.
        int ret;
        for (int j = 0; j < 5; j++) {
            ret = scrape_image(image_url, fn);
            if (ret >= 0) break;
        }

        fflush(stdout);
        if (ret == 0) usleep(1000000);
    }

    chdir("..");
}

void scrape_title(nlohmann::json j, std::string lang) {
    std::string dir_name        = j["manga"]["title"];
    nlohmann::json chap_array   = j["chapter"];

    if (mkdir(dir_name.c_str(), 0755) < 0) {
        if (errno != EEXIST) pquit(-1, "Couldn't make directory '%s': %s\n", dir_name.c_str(), strerror(errno));
    }
    chdir(dir_name.c_str());

    for (nlohmann::json::iterator it = chap_array.begin(); it != chap_array.end(); it++) {
        nlohmann::json chap_it_json = it.value();
        if (chap_it_json["lang_code"].get<std::string>() != lang) continue;

        std::string chap_api_url = MD_API_CHAP + it.key();
        printf("Using: %s\n", chap_api_url.c_str());

        nlohmann::json chap_json = fetch_json(chap_api_url);
        scrape_chapter(chap_json);
    }

    chdir("..");
}
