#ifndef __SOCK_NH_H__
#define __SOCK_NH_H__

#include <event2/bufferevent.h>
#include <event2/util.h>
#include <tr1/functional>
#include "../../linux_program_libs/utils/Time.h"
#include "../../linux_program_libs/net/Sock_addr_v4.h"
#include "../../linux_program_libs/utils/Noncopyable.h"

#define NH_READ_CB              1
#define NH_WRITE_CB             2
#define NH_ERROR_CB             3
#define NH_EOF_CB               4
#define NH_CONNECT_CB           5
#define NH_READ_TIMEOUT_CB      6
#define NH_WRITE_TIMEOUT_CB     7

class Task;

class Sock_nh: public HVN::Noncopyable
{
public:
    Sock_nh(void): m_bev(NULL), m_is_recycled(0), m_cb_obj(NULL), m_ptask(NULL) {}
    ~Sock_nh(void) { this->recycle(); }

public:
    void recycle(void);
    int is_recycled(void) const;

public:
    int initial(Task *ptask, evutil_socket_t fd, HVN::Time *pcrtv, HVN::Time *pcwtv, std::tr1::function<void (Sock_nh *, int, char *, int)> cb_obj);
    int connect(HVN::Sock_addr_v4 &csa) const;
    int send_data(const void *data, size_t size) const;
    HVN::Sock_addr_v4 get_peer_addr(void) const;
    HVN::Sock_addr_v4 get_local_addr(void) const;

    bool is_read_enable(void) const;
    bool is_write_enable(void) const;
    int read_enable(void) const;
    int read_disable(void) const;
    int write_enable(void) const;
    int write_disable(void) const;

private:
    static void read_cb(bufferevent *bev, void *arg);
    static void write_cb(bufferevent *bev, void *arg);
    static void event_cb(bufferevent *bev, short what, void *arg);

private:
    bufferevent *m_bev;
    int m_is_recycled;
    std::tr1::function<void (Sock_nh *, int, char *, int)> m_cb_obj;
    Task *m_ptask;
};

#endif
