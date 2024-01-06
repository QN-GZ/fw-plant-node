#ifndef LOG_H
#define LOG_H

#include "mgos.h"

#define LOGI(...) LOG(LL_INFO, (__VA_ARGS__))
#define LOGD(...) LOG(LL_DEBUG, (__VA_ARGS__))
#define LOGW(...) LOG(LL_WARN, (__VA_ARGS__))
#define LOGE(...) LOG(LL_ERROR, (__VA_ARGS__))

#endif // LOG_H
