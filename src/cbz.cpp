#include "cbz.h"
#include "message.h"

#include <fcntl.h>

// C++
#include <filesystem>
#include <algorithm>

// Libarchive
#include <archive.h>
#include <archive_entry.h>

void create_zip(std::string dir, std::string filename) {
    struct archive* a;
    struct archive_entry* ae;
    struct stat stat_buf;
    int fd;
    void* buf;

    buf = malloc(BUFSIZ);

    a = archive_write_new();
    //archive_write_add_filter_gzip(a);
    archive_write_set_format_zip(a);
    archive_write_open_filename(a, filename.c_str());

    // Recurse over directory.
    for (const auto& dir_ent : std::filesystem::recursive_directory_iterator(dir)) {
        // If dir, skip it.
        stat(dir_ent.path().c_str(), &stat_buf);
        if (S_ISDIR(stat_buf.st_mode)) continue;
        printf("%s\n", dir_ent.path().c_str());

        fd = open(dir_ent.path().c_str(), O_RDONLY);
        if (fd < 0) {
            pquit(128, "Unable to open file '%s' for archiving. Did it get moved while the program was running?", dir_ent.path().c_str());
        }

        ae = archive_entry_new();
        archive_entry_set_pathname(ae, dir_ent.path().c_str());
        archive_entry_copy_stat(ae, &stat_buf);
        archive_write_header(a, ae);

        // Copy the file.
        size_t len = read(fd, buf, BUFSIZ);
        //printf("len: %d\n", len);
        while (len > 0) {
            if (archive_write_data(a, buf, len) < 0) {
                pquit(128, "Archive failure: %s\n", archive_error_string(a));
            }
            len = read(fd, buf, BUFSIZ);
        }

        close(fd);
        archive_entry_free(ae);
    }


    free(buf);
    archive_write_close(a);
    archive_write_free(a);
}
