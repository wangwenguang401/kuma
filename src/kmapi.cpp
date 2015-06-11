/* Copyright (c) 2014, Fengping Bao <jamol@live.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "EventLoopImpl.h"
#include "TcpSocketImpl.h"
#include "UdpSocketImpl.h"
#include "TcpServerSocketImpl.h"
#include "TimerManager.h"
#include "http/HttpRequestImpl.h"
#include "http/HttpResponseImpl.h"
#include "ws/WebSocketImpl.h"
#include "kmapi.h"

KUMA_NS_BEGIN

EventLoop::EventLoop(PollType poll_type)
: pimpl_(new EventLoopImpl(poll_type))
{

}

EventLoop::~EventLoop()
{
    delete pimpl_;
}

bool EventLoop::init()
{
    return pimpl_->init();
}

PollType EventLoop::getPollType()
{
    return pimpl_->getPollType();
}

bool EventLoop::isPollLT()
{
    return  pimpl_->isPollLT();
}

int EventLoop::registerFd(SOCKET_FD fd, uint32_t events, IOCallback& cb)
{
    return pimpl_->registerFd(fd, events, cb);
}

int EventLoop::registerFd(SOCKET_FD fd, uint32_t events, IOCallback&& cb)
{
    return pimpl_->registerFd(fd, events, std::move(cb));
}

int EventLoop::updateFd(SOCKET_FD fd, uint32_t events)
{
    return pimpl_->updateFd(fd, events);
}

int EventLoop::unregisterFd(SOCKET_FD fd, bool close_fd)
{
    return pimpl_->unregisterFd(fd, close_fd);
}

void EventLoop::loopOnce(uint32_t max_wait_ms)
{
    pimpl_->loopOnce(max_wait_ms);
}

void EventLoop::loop(uint32_t max_wait_ms)
{
    pimpl_->loop(max_wait_ms);
}

void EventLoop::stop()
{
    pimpl_->stop();
}

EventLoopImpl* EventLoop::getPimpl()
{
    return pimpl_;
}

int EventLoop::runInEventLoop(LoopCallback& cb)
{
    return pimpl_->runInEventLoop(cb);
}

int EventLoop::runInEventLoop(LoopCallback&& cb)
{
    return pimpl_->runInEventLoop(std::move(cb));
}

int EventLoop::runInEventLoopSync(LoopCallback& cb)
{
    return pimpl_->runInEventLoopSync(cb);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
TcpSocket::TcpSocket(EventLoop* loop)
    : pimpl_(new TcpSocketImpl(loop->getPimpl()))
{

}

TcpSocket::~TcpSocket()
{
    delete pimpl_;
}

int TcpSocket::bind(const char* bind_host, uint16_t bind_port)
{
    return pimpl_->bind(bind_host, bind_port);
}
int TcpSocket::connect(const char* host, uint16_t port, EventCallback& cb, uint32_t flags, uint32_t timeout)
{
    return pimpl_->connect(host, port, cb, flags, timeout);
}

int TcpSocket::connect(const char* host, uint16_t port, EventCallback&& cb, uint32_t flags, uint32_t timeout)
{
    return pimpl_->connect(host, port, std::move(cb), flags, timeout);
}

int TcpSocket::attachFd(SOCKET_FD fd, uint32_t flags, uint8_t* init_data, uint32_t init_len)
{
    return pimpl_->attachFd(fd, flags, init_data, init_len);
}

int TcpSocket::detachFd(SOCKET_FD &fd)
{
    return pimpl_->detachFd(fd);
}

int TcpSocket::startSslHandshake(bool is_server)
{
    return pimpl_->startSslHandshake(is_server);
}

int TcpSocket::send(uint8_t* data, uint32_t length)
{
    return pimpl_->send(data, length);
}

int TcpSocket::send(iovec* iovs, uint32_t count)
{
    return pimpl_->send(iovs, count);
}

int TcpSocket::receive(uint8_t* data, uint32_t length)
{
    return pimpl_->receive(data, length);
}

int TcpSocket::close()
{
    return pimpl_->close();
}

int TcpSocket::pause()
{
    return pimpl_->pause();
}

int TcpSocket::resume()
{
    return pimpl_->resume();
}

void TcpSocket::setReadCallback(EventCallback& cb)
{
    pimpl_->setReadCallback(cb);
}

void TcpSocket::setWriteCallback(EventCallback& cb)
{
    pimpl_->setWriteCallback(cb);
}

void TcpSocket::setErrorCallback(EventCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void TcpSocket::setReadCallback(EventCallback&& cb)
{
    pimpl_->setReadCallback(std::move(cb));
}

void TcpSocket::setWriteCallback(EventCallback&& cb)
{
    pimpl_->setWriteCallback(std::move(cb));
}

void TcpSocket::setErrorCallback(EventCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

SOCKET_FD TcpSocket::getFd()
{
    return pimpl_->getFd();
}

TcpSocketImpl* TcpSocket::getPimpl()
{
    return pimpl_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
TcpServerSocket::TcpServerSocket(EventLoop* loop)
: pimpl_(new TcpServerSocketImpl(loop->getPimpl()))
{

}
TcpServerSocket::~TcpServerSocket()
{
    delete pimpl_;
}

int TcpServerSocket::startListen(const char* host, uint16_t port)
{
    return pimpl_->startListen(host, port);
}

int TcpServerSocket::stopListen(const char* host, uint16_t port)
{
    return pimpl_->stopListen(host, port);
}

int TcpServerSocket::close()
{
    return pimpl_->close();
}

void TcpServerSocket::setAcceptCallback(AcceptCallback& cb)
{
    pimpl_->setAcceptCallback(cb);
}

void TcpServerSocket::setErrorCallback(ErrorCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void TcpServerSocket::setAcceptCallback(AcceptCallback&& cb)
{
    pimpl_->setAcceptCallback(std::move(cb));
}

void TcpServerSocket::setErrorCallback(ErrorCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

TcpServerSocketImpl* TcpServerSocket::getPimpl()
{
    return pimpl_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
UdpSocket::UdpSocket(EventLoop* loop)
: pimpl_(new UdpSocketImpl(loop->getPimpl()))
{
    
}

UdpSocket::~UdpSocket()
{
    delete pimpl_;
}

int UdpSocket::bind(const char* bind_host, uint16_t bind_port, uint32_t flags)
{
    return pimpl_->bind(bind_host, bind_port, flags);
}

int UdpSocket::send(uint8_t* data, uint32_t length, const char* host, uint16_t port)
{
    return pimpl_->send(data, length, host, port);
}

int UdpSocket::send(iovec* iovs, uint32_t count, const char* host, uint16_t port)
{
    return pimpl_->send(iovs, count, host, port);
}

int UdpSocket::receive(uint8_t* data, uint32_t length, char* ip, uint32_t ip_len, uint16_t& port)
{
    return pimpl_->receive(data, length, ip, ip_len, port);
}

int UdpSocket::close()
{
    return pimpl_->close();
}

int UdpSocket::mcastJoin(const char* mcast_addr, uint16_t mcast_port)
{
    return pimpl_->mcastJoin(mcast_addr, mcast_port);
}

int UdpSocket::mcastLeave(const char* mcast_addr, uint16_t mcast_port)
{
    return pimpl_->mcastLeave(mcast_addr, mcast_port);
}

void UdpSocket::setReadCallback(EventCallback& cb)
{
    pimpl_->setReadCallback(cb);
}

void UdpSocket::setErrorCallback(EventCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void UdpSocket::setReadCallback(EventCallback&& cb)
{
    pimpl_->setReadCallback(std::move(cb));
}

void UdpSocket::setErrorCallback(EventCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

UdpSocketImpl* UdpSocket::getPimpl()
{
    return pimpl_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

Timer::Timer(EventLoop* loop)
: pimpl_(new TimerImpl(loop->getPimpl()->getTimerMgr()))
{
    
}

Timer::~Timer()
{
    delete pimpl_;
}

bool Timer::schedule(unsigned int time_elapse, TimerCallback& cb, bool repeat)
{
    return pimpl_->schedule(time_elapse, cb, repeat);
}

bool Timer::schedule(unsigned int time_elapse, TimerCallback&& cb, bool repeat)
{
    return pimpl_->schedule(time_elapse, std::move(cb), repeat);
}

void Timer::cancel()
{
    pimpl_->cancel();
}

TimerImpl* Timer::getPimpl()
{
    return pimpl_;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////
HttpRequest::HttpRequest(EventLoop* loop)
: pimpl_(new HttpRequestImpl(loop->getPimpl()))
{
    
}
HttpRequest::~HttpRequest()
{
    delete pimpl_;
}

void HttpRequest::addHeader(const char* name, const char* value)
{
    pimpl_->addHeader(name, value);
}

void HttpRequest::addHeader(const char* name, uint32_t value)
{
    pimpl_->addHeader(name, value);
}

int HttpRequest::sendRequest(const char* method, const char* url, const char* ver)
{
    return pimpl_->sendRequest(method, url, ver);
}

int HttpRequest::sendData(uint8_t* data, uint32_t len)
{
    return pimpl_->sendData(data, len);
}

int HttpRequest::close()
{
    return pimpl_->close();
}

int HttpRequest::getStatusCode()
{
    return pimpl_->getStatusCode();
}

const char* HttpRequest::getVersion()
{
    return pimpl_->getVersion();
}

const char* HttpRequest::getHeaderValue(const char* name)
{
    return pimpl_->getHeaderValue(name);
}

void HttpRequest::forEachHeader(EnumrateCallback cb)
{
    pimpl_->forEachHeader(cb);
}

void HttpRequest::setDataCallback(DataCallback& cb)
{
    pimpl_->setDataCallback(cb);
}

void HttpRequest::setWriteCallback(EventCallback& cb)
{
    pimpl_->setWriteCallback(cb);
}

void HttpRequest::setErrorCallback(EventCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void HttpRequest::setHeaderCompleteCallback(HttpEventCallback& cb)
{
    pimpl_->setHeaderCompleteCallback(cb);
}

void HttpRequest::setResponseCompleteCallback(HttpEventCallback& cb)
{
    pimpl_->setResponseCompleteCallback(cb);
}

void HttpRequest::setDataCallback(DataCallback&& cb)
{
    pimpl_->setDataCallback(std::move(cb));
}

void HttpRequest::setWriteCallback(EventCallback&& cb)
{
    pimpl_->setWriteCallback(std::move(cb));
}

void HttpRequest::setErrorCallback(EventCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

void HttpRequest::setHeaderCompleteCallback(HttpEventCallback&& cb)
{
    pimpl_->setHeaderCompleteCallback(std::move(cb));
}

void HttpRequest::setResponseCompleteCallback(HttpEventCallback&& cb)
{
    pimpl_->setResponseCompleteCallback(std::move(cb));
}

HttpRequestImpl* HttpRequest::getPimpl()
{
    return pimpl_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

HttpResponse::HttpResponse(EventLoop* loop)
: pimpl_(new HttpResponseImpl(loop->getPimpl()))
{
    
}
HttpResponse::~HttpResponse()
{
    delete pimpl_;
}

int HttpResponse::attachFd(SOCKET_FD fd, uint8_t* init_data, uint32_t init_len)
{
    return pimpl_->attachFd(fd, init_data, init_len);
}

void HttpResponse::addHeader(const char* name, const char* value)
{
    return pimpl_->addHeader(name, value);
}

void HttpResponse::addHeader(const char* name, uint32_t value)
{
    return pimpl_->addHeader(name, value);
}

int HttpResponse::sendResponse(int status_code, const char* desc, const char* ver)
{
    return pimpl_->sendResponse(status_code, desc, ver);
}

int HttpResponse::sendData(uint8_t* data, uint32_t len)
{
    return pimpl_->sendData(data, len);
}

int HttpResponse::close()
{
    return pimpl_->close();
}

const char* HttpResponse::getMethod()
{
    return pimpl_->getMethod();
}

const char* HttpResponse::getUrl()
{
    return pimpl_->getUrl();
}

const char* HttpResponse::getVersion()
{
    return pimpl_->getVersion();
}

const char* HttpResponse::getParamValue(const char* name)
{
    return pimpl_->getParamValue(name);
}

const char* HttpResponse::getHeaderValue(const char* name)
{
    return pimpl_->getHeaderValue(name);
}

void HttpResponse::forEachHeader(EnumrateCallback cb)
{
    return pimpl_->forEachHeader(cb);
}

void HttpResponse::setDataCallback(DataCallback& cb)
{
    pimpl_->setDataCallback(cb);
}

void HttpResponse::setWriteCallback(EventCallback& cb)
{
    pimpl_->setWriteCallback(cb);
}

void HttpResponse::setErrorCallback(EventCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void HttpResponse::setHeaderCompleteCallback(HttpEventCallback& cb)
{
    pimpl_->setHeaderCompleteCallback(cb);
}

void HttpResponse::setRequestCompleteCallback(HttpEventCallback& cb)
{
    pimpl_->setRequestCompleteCallback(cb);
}

void HttpResponse::setResponseCompleteCallback(HttpEventCallback& cb)
{
    pimpl_->setResponseCompleteCallback(cb);
}

void HttpResponse::setDataCallback(DataCallback&& cb)
{
    pimpl_->setDataCallback(std::move(cb));
}

void HttpResponse::setWriteCallback(EventCallback&& cb)
{
    pimpl_->setWriteCallback(std::move(cb));
}

void HttpResponse::setErrorCallback(EventCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

void HttpResponse::setHeaderCompleteCallback(HttpEventCallback&& cb)
{
    pimpl_->setHeaderCompleteCallback(std::move(cb));
}

void HttpResponse::setRequestCompleteCallback(HttpEventCallback&& cb)
{
    pimpl_->setRequestCompleteCallback(std::move(cb));
}

void HttpResponse::setResponseCompleteCallback(HttpEventCallback&& cb)
{
    pimpl_->setResponseCompleteCallback(std::move(cb));
}

HttpResponseImpl* HttpResponse::getPimpl()
{
    return pimpl_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

WebSocket::WebSocket(EventLoop* loop)
: pimpl_(new WebSocketImpl(loop->getPimpl()))
{
    
}
WebSocket::~WebSocket()
{
    delete pimpl_;
}

int WebSocket::connect(const char* ws_url, const char* proto, EventCallback& cb)
{
    return pimpl_->connect(ws_url, proto, cb);
}

int WebSocket::connect(const char* ws_url, const char* proto, EventCallback&& cb)
{
    return pimpl_->connect(ws_url, proto, std::move(cb));
}

int WebSocket::attachFd(SOCKET_FD fd, uint8_t* init_data, uint32_t init_len)
{
    return pimpl_->attachFd(fd, init_data, init_len);
}

int WebSocket::send(uint8_t* data, uint32_t len)
{
    return pimpl_->send(data, len);
}

int WebSocket::close()
{
    return pimpl_->close();
}

void WebSocket::setDataCallback(DataCallback& cb)
{
    pimpl_->setDataCallback(cb);
}

void WebSocket::setWriteCallback(EventCallback& cb)
{
    pimpl_->setWriteCallback(cb);
}

void WebSocket::setErrorCallback(EventCallback& cb)
{
    pimpl_->setErrorCallback(cb);
}

void WebSocket::setDataCallback(DataCallback&& cb)
{
    pimpl_->setDataCallback(std::move(cb));
}

void WebSocket::setWriteCallback(EventCallback&& cb)
{
    pimpl_->setWriteCallback(std::move(cb));
}

void WebSocket::setErrorCallback(EventCallback&& cb)
{
    pimpl_->setErrorCallback(std::move(cb));
}

WebSocketImpl* WebSocket::getPimpl()
{
    return pimpl_;
}

KUMA_NS_END
