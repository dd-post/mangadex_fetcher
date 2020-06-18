#ifndef SIG_H_
#define SIG_H_

#include <string>

extern std::string current_file;

// Signal handler to ensure no partial files are left hanging around.
void catch_sigint(int signo);

#endif // SIG_H_
