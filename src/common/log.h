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

#include "common/singleton.h"
#include "spdlog/spdlog.h"
#include "spdlog/stopwatch.h"

namespace log_details {
class Log {
  enable_singleton(Log);
  Log();

 public:
  ~Log();
};
}  // namespace log_details