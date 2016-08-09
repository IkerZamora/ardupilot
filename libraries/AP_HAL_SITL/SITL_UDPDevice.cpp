#include <AP_HAL/AP_HAL.h>

#if CONFIG_HAL_BOARD == HAL_BOARD_SITL

#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include "SITL_UDPDevice.h"

SITLUDPDevice::SITLUDPDevice(const char *ip, uint16_t port, bool bcast):
    _ip(ip),
    _port(port),
    _bcast(bcast)
{
}

SITLUDPDevice::~SITLUDPDevice()
{
}

ssize_t SITLUDPDevice::write(const uint8_t *buf, uint16_t n)
{
    if (!socket.pollout(0)) {
        return -1;
    }
    if (_connected) {
        return socket.send(buf, n);
    }
    return socket.sendto(buf, n, _ip, _port);
}

ssize_t SITLUDPDevice::read(uint8_t *buf, uint16_t n)
{
    ssize_t ret = socket.recv(buf, n, 0);
    if (!_connected && ret > 0) {
        const char *ip;
        uint16_t port;
        socket.last_recv_address(ip, port);
        _connected = socket.connect(ip, port);
    }
    return ret;
}

bool SITLUDPDevice::open()
{
    if (_bcast) {
        // open now, then connect on first received packet
        socket.set_broadcast();
        return true;
    }
    _connected = socket.connect(_ip, _port);
    return _connected;
}

bool SITLUDPDevice::close()
{
    return true;
}

void SITLUDPDevice::set_blocking(bool blocking)
{
    socket.set_blocking(blocking);
}

void SITLUDPDevice::set_speed(uint32_t speed)
{

}

#endif
