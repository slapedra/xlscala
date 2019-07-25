#ifndef XLSCALA_HPP_SOCKETCLIENT
#define XLSCALA_HPP_SOCKETCLIENT

#include <xlscala/defines.hpp>
#include <map>
#include <xlscala/xl/xlsession.hpp>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <xlscala/scala/messagepacket.hpp>
#include <winsock2.h>
#include <boost/timer.hpp>
#include <boost/thread/mutex.hpp>


namespace xlscala {

    template <int MaxSize>
    class SocketClient {
    protected:
         mutable boost::timer _timer;
         mutable boost::mutex _mutex;
         mutable boost::mutex::scoped_lock scoped_lock;

    public:
        SocketClient()
        : _connected(false) { /*,  scoped_lock(_mutex) {
            scoped_lock.release();*/
        }

        inline std::string host() const { return _host; }
        inline int port() const { return _port; }

        inline void open(int port, const std::string& host) {
            _port = port;
            _host = host;
            try {
                _connect();
            } catch(Error&) {
            }
        }

        ~SocketClient() {
            char r = 0;
            if (_connected) send(_sock, &r, 1, 0);
            WSACleanup();
        }

        inline int _write(char *data, int size) const {
            int nbytes = send(_sock, data, size, 0);
            if (nbytes == SOCKET_ERROR && WSAGetLastError () == WSAEWOULDBLOCK)
                return 0;
            if (nbytes == -1 && (
                WSAGetLastError () == WSAENETDOWN ||
                WSAGetLastError () == WSAENETRESET ||
                WSAGetLastError () == WSAEHOSTUNREACH ||
                WSAGetLastError () == WSAECONNABORTED ||
                WSAGetLastError () == WSAETIMEDOUT ||
                WSAGetLastError () == WSAECONNRESET))
                return -1;
            if(nbytes == SOCKET_ERROR) {
                _connected = false;
                FAIL("error socket sending data");
            }
            return nbytes;
        }

        inline int _read(char *data, int size) const {
            int nbytes = recv(_sock, data, size, 0);
            if (nbytes == SOCKET_ERROR && WSAGetLastError () == WSAEWOULDBLOCK)
                return 0;
            if (nbytes == -1 && (
                WSAGetLastError () == WSAENETDOWN ||
                WSAGetLastError () == WSAENETRESET ||
                WSAGetLastError () == WSAECONNABORTED ||
                WSAGetLastError () == WSAETIMEDOUT ||
                WSAGetLastError () == WSAECONNRESET ||
                WSAGetLastError () == WSAECONNREFUSED ||
                WSAGetLastError () == WSAENOTCONN))
                return -1;
            if(nbytes == SOCKET_ERROR){
                _connected = false;
                FAIL("error socket reading data");
            }
            if (nbytes == 0) return -1;
            return nbytes;
        }

        inline void write(char *data, int len) const {
            //scoped_lock.lock();
            //try {
                char buf[4];
                buf[0] = ((char*)(&len))[0];
                buf[1] = ((char*)(&len))[1];
                buf[2] = ((char*)(&len))[2];
                buf[3] = ((char*)(&len))[3];
                if (_write(buf, 4) == -1){
                    _connected = false;
                    //scoped_lock.release();
                    FAIL("error writing socket size");
                }
                if (_write(data, len) == -1) {
                    _connected = false;
                    //scoped_lock.release();
                    FAIL("error writing socket");
                }
                //scoped_lock.release();
            /*} catch (Error& e) {
                scoped_lock.release();
                throw e;
            }*/
        }

        inline int read(char *data) const {
            //scoped_lock.lock();
            //try {
                char buf[4];
                int count = 0;
                bool end = false;
                int l = _read(buf, 4);
                if (l != 4) {
                    _connected = false;
                    //scoped_lock.release();
                    FAIL("error reading size packet");
                }
                int size = (int((buf[0]) & 0xff) << 0) +
                           (int((buf[1]) & 0xff) << 8) +
                           (int((buf[2]) & 0xff) << 16) +
                           (int((buf[3]) & 0xff) << 24);
                while (!end) {
                    l = _read(data + count, size - count);
                    if (l == -1) {
                        _connected = false;
                        //scoped_lock.release();
                        FAIL("error reading data");
                    }
                    count += l;
                    if (count == size) {
                        end = true;
                    }
                }
                //scoped_lock.release();
                return count;
            /*} catch (Error& e) {
                scoped_lock.release();
                throw e;
            }*/
        }

        inline void operator()(const MessagePacket<MaxSize>& msg, MessagePacket<MaxSize>& rmsg) const {
            if ( (!_connected) && (_timer.elapsed() > 10)) {
                _connect();
                if (_connected) _timer.restart();
            }
            if (_connected) {
                write(msg.data(), msg.size());
                int size = read(rmsg.data());
                REQUIRE(size != 0, "empty message");
                rmsg.fixIndex(0);
            } else {
                 FAIL("not connected to XLScala server");
            }
        }

        inline bool connected() const { return _connected; }

        inline void tryconnect() {
            _connect();
        }

    protected:
        int _port;
        std::string _host;
        mutable int _sock;
        mutable struct sockaddr_in _addrRemote;
        mutable bool _connected;

        inline void _connect() const {
            WORD wVersionRequested;
            WSADATA wsaData;
            int err;
            wVersionRequested = MAKEWORD(2, 2);
            err = WSAStartup(wVersionRequested, &wsaData);
            REQUIRE(err == 0, "WSAStartup failed with error");
            struct hostent*	he = NULL;
            he = gethostbyname(_host.c_str());
            REQUIRE(he != NULL, "error gethostbyname")
            _sock = socket(AF_INET, SOCK_STREAM, 0);
            REQUIRE(_sock != -1, "error geting socket: " << _port)
            _addrRemote.sin_family = AF_INET;
            _addrRemote.sin_port = htons(_port);
            _addrRemote.sin_addr = *((struct in_addr *) he->h_addr);
            memset(&(_addrRemote.sin_zero), 0, 8);
            if (connect(_sock, (struct sockaddr *) &_addrRemote, sizeof(struct sockaddr)) == -1) {
                _connected = false;
                FAIL ("fail connecting to " << _host << " at port: " << _port);
            }
            _connected = true;
        }
    };

}

#endif



