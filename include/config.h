#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
const std::string TEXT_EDITOR = "/usr/local/bin/st -e vim";
const std::vector<std::string> TEXT_EXTENSIONS = {"", ".txt", ".cc", ".cpp", ".c", ".py", ".h"};

const std::string VIDEO_PLAYER = "/usr/bin/mpv";
const std::vector<std::string> VIDEO_EXTENSIONS = {".mp4", ".mkv"};
#endif // CONFIG_H_
