#include "sort.h"

bool sort_chapters(nlohmann::json& j, struct arg_struct& as) {
    nlohmann::json sorted = nlohmann::json();
    nlohmann::json md_chap = j["chapter"];

    if (md_chap.empty()) return false;

    uint64_t chap_count = 0;

    for (auto jit : md_chap.items()) {
        // TODO: change how json is accessed. This will throw an exception if the key does not exist.
        std::string chap = jit.value()["chapter"];
        std::string group = jit.value()["group_name"];
        std::string lang = jit.value()["lang_code"];

        // If chapter/vol is unspecified, assume it is chapter 0.
        if (chap.empty()) chap = "0";
        if (jit.value()["volume"].get<std::string>().empty()) {
            jit.value()["volume"] = "0";
        }

        // Pad chapter to 4 interger digits.
        uint8_t pad = 4;
        size_t chap_pos = chap.find(".");
        if (chap_pos != std::string::npos) pad -= chap_pos;
        else pad -= chap.length();

        chap = std::string(pad, '0') + chap;

        // Match language / group.
        if (!as.lang_code.empty() && lang != as.lang_code) continue;

        if (!as.groups.empty()) {
            if (sorted.contains(chap)) {
                bool match = true;

                for (int i = 0; i < as.groups.size(); i++) {
                    std::string match_group = as.groups[i];

                    // If the chapter to insert matches one of our groups, check any previously inserted
                    // versions to determine if they have a higher priority than this chapter. If this
                    // chapter has priority, then remove the previous selection and select this one.
                    // If this chapter has lower priority, then skip selection.
                    if (match_group == group) {
                        std::string prev_group = sorted[chap].front()["group_name"];
                        //printf("prev_group: %s\n", prev_group.c_str());

                        for (int j = i - 1; j >= 0; j--) {
                            if (as.groups[j] == prev_group) {
                                match = false;
                            };
                        }

                        break;
                    }
                }

                if (match) sorted.erase(chap);
                else continue;
            }
        }
        else if (as.one_scan) {
            if (sorted.contains(chap)) sorted.erase(chap);
        }


        /*
        if (!as.group.empty()) {
            // Only insert a non-perfered group if our list does not yet include this chapter.
            if (as.group != group && sorted.contains(chap)) continue;
            // If we come across a perferred group after we've already got this chapter, remove it and
            // replace it. Because of how the json is sorted by chapter id, we *should* always get
            // the newest version of a chapter if it has been scanlated multiple times by a group.
            // TODO: double check this.
            else if (sorted.contains(chap)) sorted.erase(chap);
        }
        // If a dl of one group has been been activated, replace any chapters with this newer chapter.
        else if (as.one_scan) {
            if (sorted.contains(chap)) sorted.erase(chap);
        }
        */

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
    printf("%-5s | %-9s | %-10s | %-20s | %-6s | %-50s\n", "Idx", "Vol", "Chap", "Group", "Lang", "Title");
    printf("%s\n", std::string(120, '=').c_str());

    for (auto jit : j["sorted"].items()) {
        for (auto kit : jit.value().items()) {
            printf("%5.5d | %9.9s | %10.10s | %-20.20s | %-6.6s | %-50.50s\n", idx,
                                                   kit.value()["volume"].get<std::string>().c_str(),
                                                   jit.key().c_str(),
                                                   kit.value()["group_name"].get<std::string>().c_str(),
                                                   kit.value()["lang_code"].get<std::string>().c_str(),
                                                   kit.value()["title"].get<std::string>().c_str());
            idx++;
        }
    }

    return true;
}
