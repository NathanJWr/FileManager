#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <vector>
enum SORT_BY { ALPHABETICALLY, TYPE };
const SORT_BY sort_type = ALPHABETICALLY;

const std::string TEXT_EDITOR = "notepad";
const std::vector<std::string> TEXT_EXTENSIONS = { "", ".txt", ".cc", ".cpp", ".c", ".py", ".h" };

const std::string VIDEO_PLAYER = "mpv";
const std::vector<std::string> VIDEO_EXTENSIONS = { ".mp4", ".mkv" };

const std::string PDF_VIEWER = "/usr/bin/evince";
const std::vector<std::string> PDF_EXTENSIONS = { ".pdf" };

const std::string IMAGE_VIEWER = "/usr/bin/eog";
const std::vector<std::string> IMAGE_EXTENSIONS = { ".png", ".jpg" };
#endif // CONFIG_H_