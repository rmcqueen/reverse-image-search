#pragma once
#ifndef REVERSE_IMAGE_SEARCH_UTILS_H
#define REVERSE_IMAGE_SEARCH_UTILS_H
#include <string>
#include <iostream>
#include <stdio.h>

#include "Image.h"

using namespace std;

string get_file_name_from_full_path(string &image_full_path);

Image get_loaded_image(string &image_full_path);

vector<string> get_image_names_from_dir(string &dir_name);
#endif //REVERSE_IMAGE_SEARCH_UTILS_H
