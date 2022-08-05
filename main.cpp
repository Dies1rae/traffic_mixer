#include <iostream>
#include <getopt.h>
#include <chrono>
#include <thread>

#include "traffic_core.h"

void messRecv(trafficCore * cl) {
    while(true) {
        cl->onMessageReceived();
    }
}

void messSend(trafficCore * cl) {
  while(true) {
    cl->onMessageSend();
  }
}

/*!
    \brief Функция содержащая синопсис помощи пользователя
*/
void printHelp(void) {
    std::cout <<
    "Synopsis: \n"
    "    -h, --help \n"
    "        Show help message and exit \n"
    "    -S, --service \n"
    "        Run as service in background \n"
    "    -C, --log-dir=<logfile directory> \n"
    "        Directory where log file reside \n"
    "    -U, --unicast=<unicast address:port to source> \n"
    "        Address|port to client 2 \n"
    "    -M, --multicast=<multicast address:port to source> \n"
    "        Address|port to client 1 \n";
}

/*!
    \brief Функция отвечающая за парсинг аргументов

    \details Функция принимающая полный список аргументов, их количество и экземпляр класса-службы promoApiSvcMain
    \param[in] argc Размер буфера аргументов
    \param[in] argv Массив массивов аргументов
    \return Запуск службы traffic_mixer с нужными аргументами, либо вывод помощи на экран
*/
void processArgs(int argc, char ** argv) {
    trafficCore * mainApp = new trafficCore();
    int arg_c = 0;
    char * logdir = nullptr;
    char * unicast_port = nullptr;
    char * multicast_port = nullptr;
    const char* const short_opts = "hSC:U:M:";
    const option long_opts[] = {
        { "help",          no_argument,        NULL, 'h'},
        { "service",       no_argument,        NULL, 'S'},
        { "log-dir",       required_argument,  NULL, 'C'},
        { "unicast",       required_argument,  NULL, 'U'},
        { "multicast",     required_argument,  NULL, 'M'},
        { NULL,            no_argument,        NULL, 0  },
    };
    while(arg_c != -1) {
        arg_c = getopt_long(argc, argv, short_opts, long_opts, nullptr);
        std::cerr << "args c = " << arg_c << std::endl;

        switch(arg_c) {
        default:
            std::cerr << "Wrong args\n";
            printHelp();
            exit(0);
        case -1:
    	    break;
        case 'S':
            mainApp->setServiceModeOn();
            break;
        case '?':
        case 'h':
	        printHelp();
	        exit(0);
            break;
	    case 'C':
	        logdir = optarg;
            mainApp->setLogPath(logdir);
            break;
        case 'U':
	        unicast_port = optarg;
            break;
        case 'M':
	        multicast_port = optarg;
            break;
        }
    }
    if(!unicast_port) {
        std::cerr << "Please specifi -U arg with address:port format\n";
        printHelp();
        exit(0);
    }
    if(!multicast_port) {
        std::cerr << "Please specifi -M arg with address:port format\n";
        printHelp();
        exit(0);
    }
    try {
        if(mainApp->init(unicast_port, multicast_port)) {
            if(mainApp->getServiceModeON()) {
                auto thrRcv = std::thread(&messRecv, mainApp);
                messSend(mainApp);
                thrRcv.detach();
            } else {
                mainApp->mainLoop();
            }
        }
    } catch(...) {
        std::cerr << "Init part of svc is wrong!\n";
	    exit(-1);
    }
}

int main(int argc, char ** argv) {     
    processArgs(argc, argv);
    
    std::cerr << "Program exited Bye-Bye\n" << std::endl;
    
    return 0;
}
