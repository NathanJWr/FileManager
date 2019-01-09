#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <vector>

#ifdef _WIN32
const std::string ROOT = "C:\\";
const std::string SLASH = "\\";
#endif

#ifdef __unix__
const std::string ROOT = "/";
const std::string SLASH = "/";
#endif


enum SORT_BY { ALPHABETICALLY, TYPE };
const SORT_BY sort_type = ALPHABETICALLY;
#endif // CONFIG_H_
