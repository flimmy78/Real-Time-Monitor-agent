#ifndef __AVS_PROXY_CLIENT_SCREEN_MGR__
#define __AVS_PROXY_CLIENT_SCREEN_MGR__

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

#include "MessageQueue.hpp"
#include "Resource.h"
#include "Parameter.h"
#include "Scene.h"
#include "Screen.h"
#include "Config.h"
#include "ToolTip.h"
#include "TitlesCtl.h"
#include "RoomsInfoScene.h"
#include "ModMediaScene.h"
#include "AddUserScene.h"
#include "DelUserScene.h"
#include "KeepAliveScene.h"
#include "DiscProxyScene.h"
#include "ResLoginScene.h"
#include "AvsProxyStructs.h"
#include "RTPSession.h"
#include "NATScene.h"
#include "TitleRoom.h"
#include "AvsProxy.h"

#define TOP_SIDE_SIZE          30
#define SIDE_SIZE              8
#define MININUM_PADDING        0
#define MININUM_TREE_CTL_WIDTH 200
#define MININUM_SCREEN_WIDTH   176
#define NIMINUM_SCREEN_HEIGHT  144
#define ROUND(num, fraction) (num) /= (fraction), (num) *= (fraction), (num) / (fraction)
#define GET_FUNC_INDEX(idx) ((idx) < 0 || (idx) >= SCREEN_RES_END ? SCREEN_RES_END : (idx))

using std::string;
using std::thread;
using std::mutex;
using std::lock_guard;
using std::vector;
using std::map;

typedef struct
{
	pj_uint32_t x;
	pj_uint32_t y;
} resolution_t;

typedef struct
{
	pj_uint32_t h; // horizonal
	pj_uint32_t v; // vertical
} round_t;

typedef struct
{
	pj_uint16_t proxy_id;
	string      proxy_ip;
	pj_uint16_t proxy_tcp_port;
	pj_uint16_t proxy_udp_port;
	TitleRoom  *title_room;
} link_room_param_t;

class ScreenMgr;
typedef void (ScreenMgr::*screenmgr_func_t)(pj_uint32_t, pj_uint32_t);
typedef map<pj_uint16_t, AvsProxy *> proxy_map_t;
class ScreenMgr
	: public Noncopyable
{
public:
	ScreenMgr(CWnd *wrapper,
		pj_uint16_t client_id,
		const pj_str_t &local_ip,
		pj_uint16_t local_udp_port);
	~ScreenMgr();

	pj_status_t LoginProxy();
	pj_status_t Prepare();
	pj_status_t Launch();
	void        Destory();
	pj_status_t OnLinkRoom(TitleRoom *title_room, Title *title);
	pj_status_t OnUnlinkRoom(TitleRoom *title_room);
	void        LinkScreenUser(pj_uint32_t new_screen_idx, User *new_user);
	void        UnlinkScreenUser(Screen *screen, User *old_user);
	void        ChangeLayout(enum_screen_mgr_resolution_t resolution);
	void        GetSuitedSize(LPRECT lpRect);
	void        Adjest(pj_int32_t &cx, pj_int32_t &cy);
	void        HideAll();
	pj_status_t LinkRoom(const link_room_param_t &param);
	void        DelAllProxys();
	void        CleanScreens();
	pj_status_t AddProxy(pj_uint16_t id, pj_str_t &ip, pj_uint16_t tcp_port, pj_uint16_t udp_port, pj_sock_t sock, proxy_map_t::mapped_type &proxy);
	pj_status_t DelProxy(proxy_map_t::mapped_type proxy);
	pj_status_t GetProxy(pj_uint16_t id, proxy_map_t::mapped_type &proxy);
	static resolution_t GetDefaultResolution();

protected:
	static void event_func_proxy(evutil_socket_t, short, void *);

	void EventOnTcpRead(evutil_socket_t fd, short event, void *arg);
	void EventOnUdpRead(evutil_socket_t fd, short event, void *arg);
	void EventOnPipe(evutil_socket_t fd, short event, void *arg);
	void EventThread();

private:
	pj_status_t ConnProxy(AvsProxy *proxy);
	/*
	 * @desc 为了兼容libevent断开连接, 此函数会在libevent线程中执行
	 */
	pj_status_t DiscProxy(AvsProxy *proxy);
	pj_status_t SendTCPPacket(const void *buf, pj_ssize_t *len);

public:
	pj_status_t ParseHttpResponse(pj_uint16_t &proxy_id, string &proxy_ip, pj_uint16_t &proxy_tcp_port, pj_uint16_t &proxy_udp_port,
		const vector<pj_uint8_t> &response);
private:
	void TcpParamScene(const pj_uint8_t *, pj_uint16_t);
	void UdpParamScene(const pjmedia_rtp_hdr *rtp_hdr, const pj_uint8_t *storage, pj_uint16_t storage_len);
	void ChangeLayout_1x1(pj_uint32_t width, pj_uint32_t height);
	void ChangeLayout_2x2(pj_uint32_t width, pj_uint32_t height);
	void ChangeLayout_1x5(pj_uint32_t width, pj_uint32_t height);
	void ChangeLayout_3x3(pj_uint32_t width, pj_uint32_t height);
	void ChangeLayout_3x5(pj_uint32_t width, pj_uint32_t height);

private:
	const CWnd         *wrapper_;
	pj_uint32_t         width_;
	pj_uint32_t         height_;
	pj_uint32_t         vertical_padding_;
	pj_uint32_t         horizontal_padding_;
	pj_uint16_t         client_id_;
	pj_sock_t           local_tcp_sock_;
	mutex               local_tcp_lock_;
	pj_caching_pool     caching_pool_;
	evutil_socket_t     pipe_fds_[2];
	pj_pool_t		   *pool_;
	struct event       *tcp_ev_, *udp_ev_, *pipe_ev_;
	struct event_base  *evbase_;
	thread              connector_thread_;
	thread              event_thread_;
	pj_bool_t           active_;
	TitlesCtl          *titles_;
	mutex               linked_proxys_lock_;
	proxy_map_t         linked_proxys_;
	vector<screenmgr_func_t> screenmgr_func_array_;
	vector<round_t>     num_blocks_;
	Screen             *screens_[MAXIMAL_SCREEN_NUM];
	enum_screen_mgr_resolution_t screen_mgr_res_;
	PoolThread<std::function<void ()>> sync_thread_pool_;

	static const resolution_t DEFAULT_RESOLUTION;
};

#endif
