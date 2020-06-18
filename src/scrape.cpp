#include "scrape.h"
#include "message.h"
#include "sort.h"

// Curl
#include <curl/curl.h>

// C/Unix
#include <unistd.h>
#include <sys/stat.h>

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

int progress_callback(void* clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow) {
    printf("\b\b\b\b\b\b%4ldkB", dltotal / 1000);
    fflush(stdout);

    return 0;
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
            printf(": Skipping '%s': file already exists.\n", filename.c_str());
            return 1;
        }
    }

    fptr = fopen(filename.c_str(), "wb");
    if (!fptr) return -1;

    handle = curl_easy_init();

    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(handle, CURLOPT_XFERINFOFUNCTION, &progress_callback);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, &write_file_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)(fptr));

    cret = curl_easy_perform(handle);

    if (cret != CURLE_OK) {
        printf(": curl_easy_perform() failed: %s\n", curl_easy_strerror(cret));
        ret = -1;
    }

    curl_easy_cleanup(handle);
    fclose(fptr);

    return ret;
}


void scrape_chapter(nlohmann::json& j) {
    std::string chap_hash   = j["hash"];
    std::string chap_server = j["server"];
    nlohmann::json pg_array = j["page_array"];

    int i = 0;
    for (auto jit : pg_array.items()) {
        std::string fn = jit.value().get<std::string>();
        std::string image_url = chap_server + chap_hash + "/" + fn;

        // Replace image name with padded number.
        std::string pg_num = std::to_string(i);
        if (pg_num.length() <= 3) pg_num = std::string(3 - pg_num.length(), '0') + pg_num;
        fn.replace(0, fn.find('.'), pg_num);

        printf("Downloading: %s -> %s... ----kB", image_url.c_str(), fn.c_str());
        fflush(stdout);

        // Try to scrape 5 times before moving on.
        int ret = -1;
        for (int j = 0; j < 5; j++) {
            ret = scrape_image(image_url, fn);
            if (ret >= 0) break;
        }

        if (ret == 0) {
            printf(": Done.\n");
            usleep(100000);
            fflush(stdout);
        }
        else if (ret == -1) {
            pquit(128, "\nFailed to get %s, Mangadex might be down. Please try again later.\n\n", image_url.c_str());
        }

        i++;
    }
}

void scrape_title(nlohmann::json& j, arg_struct& as) {
    if (as.output_path.empty()) as.output_path = j["manga"]["title"];

    // If this title has not been sorted, sort it!
    if (!j.contains("sorted")) {
        sort_chapters(j, as);
    }
    //printf("%s\n", j["sorted"].dump(4).c_str());

    // Make our output dir.
    if (mkdir(as.output_path.c_str(), 0755) < 0) {
        if (errno != EEXIST) pquit(-1, "Couldn't make directory '%s': %s\n", as.output_path.c_str(), strerror(errno));
    }
    chdir(as.output_path.c_str());

    std::string cur_vol;
    int c_idx = 0;

    if (as.start_chap == -1) as.start_chap = 0;
    if (as.end_chap == -1) as.end_chap = j["count"];

    for (auto jit : j["sorted"].items()) {
        std::string chap = jit.key();

        for (auto kit : jit.value().items()) {
            // Handle start/end idx.
            if (c_idx < as.start_chap) continue;
            if (c_idx > as.end_chap) continue;
            c_idx++;

            std::string chap_dir = chap + " - " + kit.value()["group_name"].get<std::string>();
            // Sanitize the file name.
            std::replace(chap_dir.begin(), chap_dir.end(), '/', '_');

            // If the user wants to group by volume, create a volume dir and change to it.
            if (as.by_volume) {
                std::string vol = kit.value()["volume"];
                // Pad to 2.
                if (vol.length() <= 2) vol = std::string(2 - vol.length(), '0') + vol;
                if (cur_vol != vol) {
                    if (!cur_vol.empty()) chdir("..");
                    if (mkdir(vol.c_str(), 0755) < 0) {
                        if (errno != EEXIST) pquit(-1, "Couldn't make directory '%s': %s\n", vol.c_str(), strerror(errno));
                    }
                    chdir(vol.c_str());
                    cur_vol = vol;

                    j["volumes"].push_back(vol);
                }
            }

            // Output dir for chapter.
            if (mkdir(chap_dir.c_str(), 0755) < 0) {
                if (errno != EEXIST) pquit(-1, "Couldn't make directory '%s': %s\n", chap_dir.c_str(), strerror(errno));
            }
            chdir(chap_dir.c_str());

            printf("\nFetching chapter: %s\n", chap_dir.c_str());

            // Generate the URL, scape the info, and pass to scrape_chapter().
            std::string chap_api = MD_API_CHAP + kit.key();
            nlohmann::json chap_json = fetch_json(chap_api);
            scrape_chapter(chap_json);

            chdir("..");
        }
    }

    if (!cur_vol.empty()) chdir("..");
}
