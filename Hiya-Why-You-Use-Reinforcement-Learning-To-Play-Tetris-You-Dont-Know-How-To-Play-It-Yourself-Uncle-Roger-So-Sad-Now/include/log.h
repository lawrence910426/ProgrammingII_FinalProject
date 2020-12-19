#pragma once

#include <iostream>

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define FATAL(MSG) { \
    std::cerr << "FATAL: " << __FILENAME__ << ":" << __LINE__ << " " << MSG << std::endl; \
    exit(1);         \
}

#define WARN(MSG) std::cerr << "WARN: " << __FILENAME__ << ":" << __LINE__ << " " << MSG << std::endl;

#define INFO(MSG) std::cout << "INFO: " << __FILENAME__ << ":" << __LINE__ << " " << MSG << std::endl;