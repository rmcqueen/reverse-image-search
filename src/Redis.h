#pragma once
#ifndef REVERSE_IMAGE_SEARCH_REDIS_H
#define REVERSE_IMAGE_SEARCH_REDIS_H

#include <stdio.h>

using namespace std;

class Redis {

 private:
  const char *hostName;
  int port_;

  void AddHashesToRedis(string image_path);

  void FindHashMatchInRedis(string image_name);

  void CompareTwoImages(string first_image, string second_image);

  void ClearRedis();

 public:
  Redis(char *host_name, int port) {
    hostName = host_name;
    port_ = port;
  }

  const int get_port() {
    return port_;
  };

  const char &get_host_name() const {
    return *hostName;
  };

};

#endif //REVERSE_IMAGE_SEARCH_REDIS_H
