#pragma once

/**
 * @file singleton.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief   common functions related to singleton
 * @version 0.1
 * @date 2021-05-12
 *
 * @copyright Copyright (c) 2021
 *
 */

#define no_copy(T)      \
  T(const T&) = delete; \
  auto operator=(const T&)->T& = delete

#define no_move(T) \
  T(T&&) = delete; \
  auto operator=(T&&)->T& = delete

#define enable_singleton(T)    \
                               \
 public:                       \
  static auto Instance()->T& { \
    static T instance;         \
    return instance;           \
  }                            \
  no_copy(T);                  \
  no_move(T);                  \
                               \
 private:
