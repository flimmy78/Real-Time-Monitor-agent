#ifndef _RRTVMS_COMMAND__
#define _RRTVMS_COMMAND__

typedef enum __enum_from_avs_to_avsproxy_request_type__
{
	REQUEST_FROM_AVS_TO_AVSPROXY_USERS_INFO,
	REQUEST_FROM_AVS_TO_AVSPROXY_MOD_USER_MEDIA,
	REQUEST_FROM_AVS_TO_AVSPROXY_ADD_USER,
	REQUEST_FROM_AVS_TO_AVSPROXY_DEL_USER,
	REQUEST_FROM_AVS_TO_AVSPROXY_MEDIA_STREAM
} enum_from_avs_to_avsproxy_request_t;

typedef enum __enum_from_avsproxy_to_avs_request_type__
{
	REQUEST_FROM_AVSPROXY_TO_AVS_LOGIN = 60,
	REQUEST_FROM_AVSPROXY_TO_AVS_LOGOUT,
	REQUEST_FROM_AVSPROXY_TO_AVS_LINK_USER,
	REQUEST_FROM_AVSPROXY_TO_AVS_UNLINK_USER
} enum_from_avsproxy_to_avs_request_t;

typedef enum __enum_from_client_to_avsproxy_request_type__
{
	REQUEST_FROM_CLIENT_TO_AVSPROXY_LOGIN,
	REQUEST_FROM_CLIENT_TO_AVSPROXY_LOGOUT,
	REQUEST_FROM_CLIENT_TO_AVSPROXY_LINK_ROOM_USER,
	REQUEST_FROM_CLIENT_TO_AVSPROXY_UNLINK_ROOM_USER
} enum_from_client_to_avsproxy_request_t;

typedef enum __enum_from_avsproxy_to_client_request_type__
{
	REQUEST_FROM_AVSPROXY_TO_CLIENT_ROOMS_INFO,
	REQUEST_FROM_AVSPROXY_TO_CLIENT_ROOM_MOD_MEDIA,
	REQUEST_FROM_AVSPROXY_TO_CLIENT_ROOM_ADD_USER,
	REQUEST_FROM_AVSPROXY_TO_CLIENT_ROOM_DEL_USER
} enum_from_avsproxy_to_client_request_t;

typedef enum __enum_media_mask_type__
{
	MEDIA_MASK_AUDIO = 0x01,
	MEDIA_MASK_VIDEO = 0x02,
} enum_media_mask_t;

typedef enum __enum_rtp_payload_type__
{
	RTP_MEDIA_AUDIO_TYPE = 11,
	RTP_MEDIA_VIDEO_TYPE = 29,
	RTP_EXPAND_PAYLOAD_TYPE = 110,
} enum_rtp_payload_t;

#endif
