#include <vector>
#include <opencv2/opencv.hpp>
#include <regex>
#include <utils.h>
#include <Surf.h>
#include <PerceptualHash.h>

#include "Redis.h"
#include "hiredis/hiredis.h"

using namespace std;

void Redis::AddHashToRedis(string image_path) {
  Image loaded_image = get_loaded_image(image_path);
  vector<cv::KeyPoint> key_points (loaded_image.get_image_data());
  auto hash_matrix = hashes::PerceptualHash::ComputeHash(key_points);
  auto hash_string = hashes::PerceptualHash::ConvertHashToString(hash_matrix);


  redisContext *c;
  struct timeval timeout = {1, 500000}; // 1.5 seconds
  c = redisConnectWithTimeout(host_name_, port_, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection Error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection Error: cannot allocate Redis context\n");
    }
    exit(1);
  }

  // TODO implement Redis insertion

}

int Redis::FindHashMatchInRedis(string image_name) {
  auto loaded_image = get_loaded_image(image_name).get_image_data();
  vector<cv::KeyPoint> key_points = get_key_points(loaded_image);
  auto hash_matrix = hashes::PerceptualHash::ComputeHash(key_points);
  auto hash_string = hashes::PerceptualHash::ConvertHashToString(hash_matrix);

  redisContext *c;
  redisReply *reply;
  struct timeval timeout = {1, 500000}; // 1.5 seconds
  c = redisConnectWithTimeout(host_name_, port_, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    exit(1);
  }

  // TODO implement hash search

  return 0;
}

void Redis::CompareTwoImages(string first_image, string second_image) {
  // Clear the database
  ClearRedis();

  // Add image first_image to the database
  AddHashToRedis(first_image);

  // Check for matches using second_image
  cout << "{\n\tcount: " << FindHashMatchInRedis(second_image) << "\n}";
}

void Redis::ClearRedis() {
  redisContext *c;
  struct timeval timeout = {1, 500000}; // 1.5 seconds
  c = redisConnectWithTimeout(host_name_, port_, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    exit(1);
  }
  redisCommand(c,"FLUSHALL");
  redisFree(c);
}
