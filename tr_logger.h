#ifndef TRLOGGER_H_INCLUDED
#define TRLOGGER_H_INCLUDED

#include <string>
#include <iostream>
#include <log4cpp/Category.hh>
#include <log4cpp/Appender.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/RollingFileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/Layout.hh>
#include <log4cpp/BasicLayout.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/PatternLayout.hh>

static const size_t DEFLOGDILESIZE = 100 * 1024 * 1024; // 100 M
static const unsigned int DEFLOGBCKPINDEX = 1000;

#define TRAFFIC_LOG(level) (log4cpp::Category::getRoot() << level << std::string( __FILE__) << ":" << __LINE__ << ": " << __func__ << "() ")

/*!
  \brief trLogger logger class for traffic core
  Main logger class uses LOG4CPP lib
*/

class trLogger {
public:
    trLogger() {};
    ~trLogger(){}
    void log(const std::string& msg) {
        TRAFFIC_LOG(log4cpp::Priority::INFO) << msg;
    }
    void log_error(const std::string& msg) {
        TRAFFIC_LOG(log4cpp::Priority::ERROR) << "error: " << msg;
    }
};
#undef TRAFFIC_LOG

#endif  //TRLOGGER_H_INCLUDED