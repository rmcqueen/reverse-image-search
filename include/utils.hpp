#pragma once
#ifndef REVERSE_IMAGE_SEARCH_UTILS_H
#define REVERSE_IMAGE_SEARCH_UTILS_H

#include <string>
#include <vector>

namespace utils {
    class Utility {
      public:
        Utility() {};

        static std::string get_file_name_from_full_path(std::string &image_full_path);

        static std::string get_image_label(std::string &image_full_path);

        static std::vector<std::string> get_image_names_from_dir(std::string db_dir);

        static std::vector<std::string> get_classes(const std::string &dir_path);

    };
}
#endif //REVERSE_IMAGE_SEARCH_UTILS_H
