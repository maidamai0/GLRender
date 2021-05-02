#pragma once

/**
 * @file log.h
 * @author tonghao.yuan (yuantonghao@gmail.com)
 * @brief thin warpper of spdlog
 * @version 0.1
 * @date 2021-05-02
 *
 * @copyright Copyright (c) 2021
 *
 */

#define LOGD SPDLOG_DEBUG
#define LOGI SPDLOG_INFO
#define LOGW SPDLOG_WARN
#define LOGE SPDLOG_ERROR

#ifndef NDEBUG
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_DEBUG
#endif

#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"

namespace log_details {
class Log {
 public:
  Log();

  Log(const Log&) = delete;
  Log(Log&&) = delete;
  auto operator=(const Log&) -> Log& = delete;
  auto operator=(Log&&) -> Log& = delete;

  ~Log();

 private:
  static const Log log_;
};
}  // namespace log_details