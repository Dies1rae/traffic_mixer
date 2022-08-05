#ifndef SOCK_INTERFACE_H_INCLUDED
#define SOCK_INTERFACE_H_INCLUDED

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string>
#include <chrono>
#include <thread>

#define CLIENT_NETWORK_BUFFER_SIZE_MAX 1024
inline static constexpr size_t SLEEP_TIMEOUT_MAX = 500;
inline static constexpr size_t SLEEP_TIMEOUT_STEP = 50;

/*!
  \brief netCore class implement lib with interface under unix socket
  use socket lib and threads
*/

class netCore {
public:
    enum CONNTYPE : short {
        ERROR = -1,
        MULTICAST = 0,
        UNICAST = 1,
    };

    //! Base constructor, all init by default
	netCore() = default;

    /*!
        Constructor with modified options path
        \param[in] conntype int represent as netCore::CONNTYPE, by def -1 as ERROR
    */
    netCore(int conntype) : connector_type_(CONNTYPE(conntype)), srv_socket_id_(0) {};

	~netCore() = default;

    /*!
        Connection to server
    */
	bool connectCli(void);

    /*!
        DisConnection from server
    */
	void disConnect(void) const;

    /*!
        Send to server from buffer in
    */
	bool sendToServer(void);

    /*!
        Recv from server to buffer out
    */
	bool checkMessageReceived(void);

    /*!
        init priv field with svr info
    */
    bool addrInit(const std::string& addr, int port);

	std::string      in_buff_;                      //! buffer in
    std::string      out_buff_;                     //! buffer out
private:
    CONNTYPE    connector_type_ = CONNTYPE::ERROR;  //! CONNTYPE
	int         srv_socket_id_ = 0;                 //! SOCK STATUS

    std::string srv_addr_;                          //! SRV ADDR
    int port_;                                      //! SRV PORT
};

#endif   //SOCK_INTERFACE_H_INCLUDED
