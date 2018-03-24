/*
* This is a short example of how to write uIP applications using
* protosockets.
*/

/*
* We define the application state (struct hello_world_state) in the
* hello-world.h file, so we need to include it here. We also include
* uip.h (since this cannot be included in hello-world.h) and
* <string.h>, since we use the memcpy() function in the code.
*/
#include "example_test_1.h"
#include "uip.h"
#include <string.h>

#define example4
/*---------------------------------------------------------------------------*/
void
example_test_1_init(void)
{
#ifdef example4
	static uip_ipaddr_t ipaddr;
	//uip_ipaddr(ipaddr, 192,168,1,10);
	/* Su beo dynamic IP */
	uip_ipaddr(ipaddr, 116,100,178,90);
	/* Test server static IP Bk house*/
	//uip_ipaddr(ipaddr, 192,168,11,11);
	if(uip_connect(&ipaddr, HTONS(1000))!=NULL)
	{
		printf("Connected!");
	};
#endif
#ifdef example2
	/* We start to listen for connections on TCP port 1000. */
	uip_listen(HTONS(1000));
#endif
}
/*---------------------------------------------------------------------------*/
#define UIP_APPSTATE_SIZE sizeof(struct example2_state)
void
example_test_1_appcall(void)
{
#ifdef example4
	if(uip_connected() || uip_rexmit()) {
		//		uip_send("GET /file HTTP/1.0\r\nServer:192.186.0.1\r\n\r\n",
		//				 48);
		uip_send("Do you hear me?",
				 sizeof("Do you hear me?"));
		return;
	}
	if(uip_newdata()) 
	{
		uip_send("I'm hear!",
				 sizeof("I'm hear!"));
	};
	if(uip_poll() && uip_stopped(uip_conn)) {
			uip_restart();
	}
#endif
#ifdef example2
	example2_state *s;
	s = (example2_state*)(&(uip_conn->appstate));
	if(uip_connected()) {
		s->state = WELCOME_SENT;
		uip_send("Welcome!\n", 9);
		return;
	}
	if(uip_acked() && s->state == WELCOME_SENT) {
		s->state = WELCOME_ACKED;
	}
	if(uip_newdata()) {
		uip_send("ok\n", 3);
	}
	if(uip_rexmit()) {
		switch(s->state) {
		case WELCOME_SENT:
			uip_send("Welcome!\n", 9);
			break;
		case WELCOME_ACKED:
			uip_send("ok\n", 3);
			break;
		}
	}
#endif
}