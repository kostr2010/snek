#pragma once

// #include <chrono>
// #include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

extern std::fstream log_model;
extern const char*  log_model_path;

extern std::fstream log_view;
extern const char*  log_view_path;

extern std::fstream log_controller;
extern const char*  log_controller_path;

extern std::ostringstream buf;

extern const int log_width;

// extern std::time_t result;

// ====================
// LVL_SYSTEM

#define LOG_LVL_MODEL_INIT()                                                                       \
  log_model.open(log_model_path, std::ios::trunc | std::ios::out);                                 \
  log_model.close()

#define LOG_LVL_MODEL_ROUTINE(msg)                                                                 \
  log_model.open(log_model_path, std::ios::app | std::ios::in);                                    \
  /* result = std::time(nullptr); */                                                               \
  buf << "[MODEL] " << __PRETTY_FUNCTION__;                                                        \
  log_model /* << std::asctime(std::localtime(&result))*/                                          \
      << std::setw(log_width) << std::left << buf.str() << msg << std::endl;                       \
  buf.str(std::string());                                                                          \
  log_model.close()

#define LOG_LVL_MODEL_FAILURE(msg)                                                                 \
  log_model.open(log_model_path, std::ios::app | std::ios::in);                                    \
  /* result = std::time(nullptr); */                                                               \
  buf << "[MODEL] {FAILURE} " << __PRETTY_FUNCTION__;                                              \
  log_model /* << std::asctime(std::localtime(&result))*/                                          \
      << std::setw(log_width) << buf.str() << msg << std::endl;                                    \
  buf.str(std::string());                                                                          \
  log_model.close()

// ====================
// LVL_COMPONENT

#define LOG_LVL_VIEW_INIT()                                                                        \
  log_view.open(log_view_path, std::ios::trunc | std::ios::out);                                   \
  log_view.close()

#define LOG_LVL_VIEW_ROUTINE(msg)                                                                  \
  log_view.open(log_view_path, std::ios::app | std::ios::in);                                      \
  /*result = std::time(nullptr);*/                                                                 \
  buf << "[VIEW] " << __PRETTY_FUNCTION__;                                                         \
  log_view /*<< std::asctime(std::localtime(&result))*/ << std::setw(log_width) << std::left       \
                                                        << buf.str() << msg << std::endl;          \
  buf.str(std::string());                                                                          \
  log_view.close()

#define LOG_LVL_VIEW_FAILURE(msg)                                                                  \
  log_view.open(log_view_path, std::ios::app | std::ios::in);                                      \
  /* result = std::time(nullptr); */                                                               \
  buf << "[VIEW] {FAILURE} " << __PRETTY_FUNCTION__;                                               \
  log_view /* << std::asctime(std::localtime(&result))*/                                           \
      << std::setw(log_width) << std::left << buf.str() << msg << std::endl;                       \
  buf.str(std::string());                                                                          \
  log_view.close()

// ====================
// LVL_ENTITY

#define LOG_LVL_CONTROLLER_INIT()                                                                  \
  log_controller.open(log_controller_path, std::ios::trunc | std::ios::out);                       \
  log_controller.close()

#define LOG_LVL_CONTROLLER_ROUTINE(msg)                                                            \
  log_controller.open(log_controller_path, std::ios::app | std::ios::in);                          \
  /*result = std::time(nullptr);*/                                                                 \
  buf << "[CONTROLLER] {FAILURE} " << __PRETTY_FUNCTION__;                                         \
  log_controller /*<< std::asctime(std::localtime(&result))*/ << std::setw(log_width) << std::left \
                                                              << buf.str() << msg << std::endl;    \
  buf.str(std::string());                                                                          \
  log_controller.close()

#define LOG_LVL_CONTROLLER_FAILURE(msg)                                                            \
  log_controller.open(log_controller_path, std::ios::app | std::ios::in);                          \
  /* result = std::time(nullptr); */                                                               \
  buf << "[CONTROLLER] {FAILURE} " << __PRETTY_FUNCTION__;                                         \
  log_controller /* << std::asctime(std::localtime(&result))*/                                     \
      << std::setw(log_width) << std::left << buf.str() << msg << std::endl;                       \
  buf.str(std::string());                                                                          \
  log_controller.close()
