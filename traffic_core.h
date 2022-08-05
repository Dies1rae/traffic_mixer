#ifndef TRAFFICCORE_H_INCLUDED
#define TRAFFICCORE_H_INCLUDED

#include <string>

#include "tr_logger.h"
#include "./sock_interface/sock_interface.h"

#define DEFLOGPATH                "./traffic_mixer.log"
#define UNICAST_ADDR              "127.0.0.1"
#define UNICAST_PORT              8335
#define MULTICAST_ADDR            "127.0.0.1"
#define MULTICAST_PORT            8336

/*!
  \brief trafficCore class implement base utility interface
  use socket lib, logger
*/

class trafficCore {
public:
    /*!
        Constructor with modified options path
        \param[in] log_path UNC path to config file(traffic_mixer.log)
    */
    trafficCore(const std::string& log_path = DEFLOGPATH);

    trafficCore(const trafficCore&) = delete;
    trafficCore& operator=(const trafficCore&) = delete;

    ~trafficCore() {
        if (this->main_logger_ != nullptr) {
            this->main_logger_->~trLogger();
            delete this->main_logger_;
        }
    }

    /*! 
        \brief Main init

        \param[in] uaddr address:port for unicast server
        \param[in] maddr address:port for multicast server

        Outter initialization method. Get's nothing, do something(basically init all subordinate classes)
        \return bool true if all inited without errors, false otherwise
    */
    bool init(const std::string& uaddr, const std::string& maddr);

    /*! 
        \brief Main loop of programm
        Main loop for all service. Mainly sets order for all methods call and waits
    */
    void mainLoop(void);

    /*!
        \brief Set service mode on(infinate main loop)
    */
    void setServiceModeOn(void);

    /*!
        \brief Set UNC path to otraffic_mixer.log file
        \param[in] cfg_path UNC path to config file(traffic_mixer.log)
    */
    void setLogPath(const char * log_path);  

    /*!
        \brief Set client 2 addr:port
        \param[in] addr to unicast client 2
    */
    bool setUnicastAddress(const char * addr); 

    /*!
        \brief Set client 1 addr:port
        \param[in] addr to unicast client 1
    */
    bool setMulticastAddress(const char * addr); 

    /*!
        \brief gettter for service mode on(infinate main loop)
    */
    const bool getServiceModeON(void) const;

    /*!
        \brief Check if any messages are recived(need for thread)
    */
	void onMessageReceived(void);

    /*!
        \brief Check if any messages are sended(need for thread)
    */
    void onMessageSend(void);
private:
    bool serviceMode_             =            false;                    //! application work mode - service || application
    bool svc_inited_status_       =            false;                    //! main svc init - network init&&logger init
    std::string log_path_         =            DEFLOGPATH;               //! path for log file
    std::string buffer_           =            {};                       //! internal JSON buffer
    trLogger * main_logger_       =            nullptr;                  //! LOGGER CLASS

    netCore unicast_connector_                 {1};                      //! socket interface lib for unicast cli
    netCore multicast_connector_               {0};                      //! socket interface lib for multicast cli
};

#endif  //TRAFFICCORE_H_INCLUDED
