#pragma once
#ifndef REVERSE_IMAGE_SEARCH_VOCABULARYBUILDER_H
#define REVERSE_IMAGE_SEARCH_VOCABULARYBUILDER_H

#include <opencv2/core.hpp>

void WriteVocabularyToDisk(cv::Mat &matrix, const std::string &file_name);

bool VocabularyExists(const std::string &file_name);

cv::Mat ReadVocabularyFromDisk(const std::string &file_name);

cv::Mat ConstructVocabulary(cv::Mat &training_descriptors, const std::string &file_name, bool write_to_disk=true);

#endif //REVERSE_IMAGE_SEARCH_VOCABULARYBUILDER_H
