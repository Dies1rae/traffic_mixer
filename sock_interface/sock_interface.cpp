

#include "sock_interface.h"

#include <iostream>
#include <cstring>
#include <vector>

bool netCore::connectCli(void) {
    bool conn_status { false };
    struct sockaddr_in srv;
    int client_connect_status;
    switch(this->connector_type_) {
        default:
        case netCore::ERROR:
            std::cerr << "socket type error\n";
            break;
        case netCore::UNICAST:
            this->srv_socket_id_ = socket(AF_INET, SOCK_DGRAM, 0);
            if(this->srv_socket_id_ < 0) {
                std::cerr << "Client Socket error\n";
                conn_status = false;
            }
            
            if (fcntl(this->srv_socket_id_, F_SETFL, fcntl(this->srv_socket_id_, F_GETFL, 0) |  O_NONBLOCK) == -1) {
                std::cerr << "non block socket error\n";
                conn_status = false;
            }
            srv.sin_family = AF_INET;
            srv.sin_port = htons(this->port_);
            srv.sin_addr.s_addr = inet_addr(this->srv_addr_.c_str());
            client_connect_status = connect(this->srv_socket_id_, (struct sockaddr *)&srv, sizeof(srv));
            if(client_connect_status ==  0)  {
                std::cerr << "Server disconnect cli\n";
                conn_status = false;
            } else if (client_connect_status != -1) {
                std::cerr << "Some errors\n";
                conn_status = false;
            }
            std::cerr << "ALL OK\n";
            conn_status = true;
            break;
        case netCore::MULTICAST:
            this->srv_socket_id_ = socket(AF_INET, SOCK_DGRAM, 0);
            if(this->srv_socket_id_ < 0) {
                std::cerr << "Client Socket error\n";
                conn_status = false;
            }
            
            if (fcntl(this->srv_socket_id_, IP_MULTICAST_IF, F_SETFL, fcntl(this->srv_socket_id_, F_GETFL, 0) |  O_NONBLOCK) == -1) {
                std::cerr << "non block socket multicast error\n";
                conn_status = false;
            }      
            srv.sin_family = AF_INET;
            srv.sin_port = htons(this->port_);
            srv.sin_addr.s_addr = inet_addr(this->srv_addr_.c_str());
            client_connect_status = connect(this->srv_socket_id_, (struct sockaddr *)&srv, sizeof(srv));
            if(client_connect_status ==  0)  {
                std::cerr << "Server disconnect cli\n";
                conn_status = false;
            } else if (client_connect_status != -1) {
                std::cerr << "Some errors\n";
                conn_status = false;
            }
            std::cerr << "ALL OK\n";
            conn_status = true;
            break;
    }
	return conn_status;
}

void netCore::disConnect(void) const {
	
}

bool netCore::sendToServer(void) {
    std::vector<unsigned char> buff(this->in_buff_.size() + sizeof(size_t));
    for(size_t ptr = 0; ptr < this->in_buff_.size(); ++ptr) {
        buff[ptr + sizeof(size_t)] = (unsigned char)this->in_buff_[ptr];
    }
    size_t real_size = this->in_buff_.size();
    unsigned char* ptr_to_size = (unsigned char *) &real_size;
    for(size_t ptr = 0; ptr < sizeof(size_t); ++ptr) {
        buff[ptr] = ptr_to_size[ptr];
    }
        
    return send(this->srv_socket_id_, buff.data(), buff.size(), 0);
}

bool netCore::checkMessageReceived(void) {
    this->out_buff_.resize(CLIENT_NETWORK_BUFFER_SIZE_MAX);
	int byteIn = recv(this->srv_socket_id_, (void*)this->out_buff_.data(), CLIENT_NETWORK_BUFFER_SIZE_MAX, 0);
    if(byteIn <= 0) {
		this->out_buff_.clear();
        return false;
    }
    size_t totalBytesIn = 0;
    size_t real_size = 0;
    unsigned char* ptr_to_size = (unsigned char *) &real_size;        
    for(size_t ptr = 0; ptr < sizeof(size_t); ++ptr) {
        ptr_to_size[ptr] = (unsigned char)this->out_buff_.data()[ptr];
    }
    this->out_buff_ = this->out_buff_.substr(sizeof(size_t), this->out_buff_.size());
    totalBytesIn += this->out_buff_.size();
    this->out_buff_.resize(real_size);            
	if(real_size < CLIENT_NETWORK_BUFFER_SIZE_MAX) {
		return ((size_t)byteIn - sizeof(size_t))  == real_size ? true : false;
	}
	size_t total_await = 0;
    while(totalBytesIn < real_size && total_await <= SLEEP_TIMEOUT_MAX) {
        byteIn = (size_t)recv(this->srv_socket_id_, (void*)(this->out_buff_.data() + totalBytesIn), CLIENT_NETWORK_BUFFER_SIZE_MAX, 0);
        if(byteIn > 0) {
            totalBytesIn += (size_t)byteIn;
			total_await = 0;
        } else {
			std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIMEOUT_STEP));
			total_await += SLEEP_TIMEOUT_STEP;
			byteIn = 0;
		}
    }
    
	return totalBytesIn == real_size ? true : false;
}

bool netCore::addrInit(const std::string& addr, int port) {
    this->srv_addr_ = addr;
    this->port_ = port;
    return this->port_ && this->srv_addr_.size();
}