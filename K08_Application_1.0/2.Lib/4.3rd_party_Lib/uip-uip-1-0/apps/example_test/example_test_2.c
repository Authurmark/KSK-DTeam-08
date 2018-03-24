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
/*---------------------------------------------------------------------------*/
void
example_test_1_init(void)
{
	/* We start to listen for connections on TCP port 1000. */
	uip_listen(HTONS(1000));
}
/*---------------------------------------------------------------------------*/
#define UIP_APPSTATE_SIZE sizeof(struct example2_state)
void
example_test_1_appcall(void)
{
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
}