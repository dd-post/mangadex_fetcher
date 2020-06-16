// Header for messages in the program.
#include <stdarg.h>

#define MD_SITE         "mangadex.org"
#define MD_SITE_REGEX   "(https?:\\/\\/mangadex.org\\/title\\/[0-9]+((\\/)|(\\/[a-z\\-]+\\/?))?)|([0-9]+)"
#define MD_SITE_URL     "https://" MD_SITE "/"
#define MD_API_URL      MD_SITE_URL "api/"
#define MD_API_CHAP     MD_API_URL "chapter/"
#define MD_API_TITLE    MD_API_URL "manga/"

// Print a message and quit.
void pquit(int ret, const char* format, ...);

