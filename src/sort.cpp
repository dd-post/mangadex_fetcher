#include "sort.h"

bool sort_chapters(nlohmann::json& j, struct arg_struct& as) {
    nlohmann::json sorted = nlohmann::json();
    nlohmann::json md_chap = j["chapter"];

    if (md_chap.empty()) return false;

    uint64_t chap_count = 0;

    for (nlohmann::json::iterator jit = md_chap.begin(); jit != md_chap.end(); jit++) {
        // TODO: change how json is accessed. This will throw an exception if the key does not exist.
        std::string chap = jit.value()["chapter"];
        std::string group = jit.value()["group_name"];
        std::string lang = jit.value()["lang_code"];

        // If chapter/vol is unspecified, assume it is chapter 0.
        if (chap.empty()) chap = "0";
        if (jit.value()["volume"].get<std::string>().empty()) {
            jit.value()["volume"] = "0";
        }

        // Match language / group.
        if (!as.lang_code.empty() && lang != as.lang_code) continue;
        if (!as.group.empty()) {
            // Only insert a non-perfered group if our list does not yet include this chapter.
            if (as.group != group && sorted.contains(chap)) continue;
            // If we come across a perferred group after we've already got this chapter, remove it and
            // replace it. Because of how the json is sorted by chapter id, we *should* always get
            // the newest version of a chapter if it has been scanlated multiple times by a group.
            // TODO: double check this.
            else if (sorted.contains(chap)) sorted.erase(chap);
        }


        // Pad to 4 interger digits.
        uint8_t pad = 4;
        size_t chap_pos = chap.find(".");
        if (chap_pos != std::string::npos) pad -= chap_pos;
        else pad -= chap.length();

        chap = std::string(pad, '0') + chap;

        // If title is empty, replace it with a place holder.
        if (jit.value()["title"].get<std::string>().empty()) {
            jit.value()["title"] = "<no title>";
        }

        sorted[chap][jit.key()] = jit.value();
        chap_count++;
    }

    j["count"] = chap_count;
    j["sorted"] = sorted;

    return true;
}

bool print_chapters(nlohmann::json& j, struct arg_struct& as) {
    if (!j.contains("sorted")) {
        if (!sort_chapters(j, as)) return false;
    }
    nlohmann::json sorted = j["sorted"];

    int idx = 0;
    printf("%-5s | %-9s | %-10s | %-50s | %-20s | %-6s\n", "Idx", "Vol", "Chap", "Title", "Group", "Lang");
    printf("%s\n", std::string(120, '=').c_str());

    for (auto jit : j["sorted"].items()) {
        for (auto kit : jit.value().items()) {
            printf("%5.5d | %9.9s | %10.10s | %-50.40s | %-20.30s | %-6.6s\n", idx,
                                                   kit.value()["volume"].get<std::string>().c_str(),
                                                   jit.key().c_str(),
                                                   kit.value()["title"].get<std::string>().c_str(),
                                                   kit.value()["group_name"].get<std::string>().c_str(),
                                                   kit.value()["lang_code"].get<std::string>().c_str());
            idx++;
        }
    }

    return true;
}
