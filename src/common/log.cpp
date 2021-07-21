#include "common/log.h"

#include "spdlog/common.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/stdout_sinks.h"

Log::Log() {
  auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
      fmt::format("logs/{}_log.txt", APP_NAME), true);
  auto std_cout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

  spdlog::set_default_logger(std::make_shared<spdlog::logger>(
      "OpenGL", spdlog::sinks_init_list{std_cout_sink, file_sink}));

  spdlog::set_level(spdlog::level::debug);
  spdlog::enable_backtrace(10);
  spdlog::flush_on(spdlog::level::debug);

  spdlog::set_pattern("%Y-%m-%d %T.%e %L %^%v%$ @%s:%#");
  SPDLOG_INFO(fmt::format("{} started", APP_NAME));
}

Log::~Log() {
  SPDLOG_INFO(fmt::format("{} terminated", APP_NAME));
}