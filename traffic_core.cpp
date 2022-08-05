#include <iostream>
#include <fstream>
#include <stdexcept>
#include <chrono>
#include <thread>

#include "traffic_core.h"

trafficCore::trafficCore(const std::string& log_path) {
    this->main_logger_ = new trLogger();
    log4cpp::Appender *appender = new log4cpp::RollingFileAppender("default"
        , log_path
        , DEFLOGDILESIZE
        , DEFLOGBCKPINDEX
    );
    log4cpp::PatternLayout *pl = new log4cpp::PatternLayout;
    pl->setConversionPattern( "[[%d::%t]] %p - %m%n" );
    appender->setLayout( pl );
    log4cpp::Category& log = log4cpp::Category::getRoot();
    log.setPriority(log4cpp::Priority::INFO);
    log.addAppender(appender);
    this->main_logger_->log("::Logger init done::");
}

bool trafficCore::setMulticastAddress(const char * addr) {
    std::string addrall = addr;
    this->main_logger_->log("::set multicast cli addr:port to ::" + addrall);
    int port = std::stoi(addrall.substr(addrall.find_last_of(':') + 1));
    addrall = addrall.substr(0, addrall.size() - (addrall.size() - addrall.find_last_of(':')));
    this->multicast_connector_.addrInit(addrall, port);
}

bool trafficCore::setUnicastAddress(const char * addr) {
    std::string addrall = addr;
    this->main_logger_->log("::set unicast cli addr:port to ::" + addrall);
    int port = std::stoi(addrall.substr(addrall.find_last_of(':') + 1));
    addrall = addrall.substr(0, addrall.size() - (addrall.size() - addrall.find_last_of(':')));
    this->unicast_connector_.addrInit(addrall, port);
}

bool trafficCore::init(const std::string& uaddr, const std::string& maddr) {
    this->main_logger_->log("::init main part of core::");
    this->svc_inited_status_ = this->setUnicastAddress(uaddr.c_str()) 
                            && this->setMulticastAddress(maddr.c_str())
                            && this->main_logger_
                            && this->unicast_connector_.connectCli()
                            && this->multicast_connector_.connectCli();
    return this->svc_inited_status_;
}

void trafficCore::setServiceModeOn(void) {
    this->main_logger_->log("::service mode ON::");
    this->serviceMode_ = true;
}

const bool trafficCore::getServiceModeON(void) const {
    return this->serviceMode_;
}

void trafficCore::onMessageReceived(void) {
    this->main_logger_->log("::Entering arrival Message check::");
    if(this->unicast_connector_.checkMessageReceived()) {
        this->main_logger_->log("::unicast message has arrived::" + this->unicast_connector_.out_buff_);
        this->multicast_connector_.in_buff_ = this->unicast_connector_.out_buff_;
        this->unicast_connector_.out_buff_.clear();
    }
    if(this->multicast_connector_.checkMessageReceived()) {
        this->main_logger_->log("::multicast message has arrived::" + this->multicast_connector_.out_buff_);
        this->unicast_connector_.in_buff_ = this->multicast_connector_.out_buff_;
        this->multicast_connector_.out_buff_.clear(); 
    }
}

void trafficCore::onMessageSend(void) {
    this->main_logger_->log("::Entering send Message check::");
    if(!this->multicast_connector_.in_buff_.empty()) {
        this->main_logger_->log("::multicast message has sended::" + this->multicast_connector_.in_buff_);
        if(this->multicast_connector_.sendToServer()) {
            this->multicast_connector_.in_buff_.clear();
        }
    }
    if(!this->unicast_connector_.in_buff_.empty()) {
        this->main_logger_->log("::unicast message has sended::" + this->unicast_connector_.in_buff_);
        if(this->unicast_connector_.sendToServer()) {
            this->unicast_connector_.in_buff_.clear();
        }
    }
}

void trafficCore::mainLoop(void) {
    this->main_logger_->log("::Entering main loop::");
    this->onMessageReceived();
    this->onMessageSend();
}

void trafficCore::setLogPath(const char * log_path) {
    this->log_path_ = log_path;
}