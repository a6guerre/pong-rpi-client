extern "C" {
    #include <stdio.h>
    #include <unistd.h>
    #include "libenjoy.h"
}

#include <sigfn.hpp>

static void pong_rpi_client_send_data(uint32_t part_id, int16_t data)
{
    // Send to client using RPC not yet implemented
    // Using printf to mock this for now
    printf("axes %u: data val:%d\n", part_id, data);
}

static void pong_rpi_client_event_loop(libenjoy_context **ctx, libenjoy_event *ev)
{
    int counter = 0;
    while(libenjoy_poll(*ctx, ev))
    {
	switch(ev->type)
	{
	    case LIBENJOY_EV_AXIS:
	        if (ev->part_id == 0)
                   pong_rpi_client_send_data(ev->part_id, ev->data);
	        break;
	    case LIBENJOY_EV_BUTTON:
		printf("%u: button %d val %d\n", ev->joy_id, ev->part_id, ev->data);
		break;
	    case LIBENJOY_EV_CONNECTED:
		printf("%u: status changed: %d\n", ev->joy_id, ev->data);
		break;
	}
    }
    usleep(50000);
    counter += 50;
    if(counter >= 1000)
    {
	libenjoy_enumerate(*ctx);
	counter = 0;
    }

}

int main(void)
{
    libenjoy_context *ctx = libenjoy_init(); // initialize the library
    libenjoy_joy_info_list *info;

    // Updates internal list of joysticks. If you want auto-reconnect
    // after re-plugging the joystick, you should call this every 1s or so
    libenjoy_enumerate(ctx);

    // get list with available joysticks. structs are
    // typedef struct libenjoy_joy_info_list {
    //     uint32_t count;
    //     libenjoy_joy_info **list;
    // } libenjoy_joy_info_list;
    //
    // typedef struct libenjoy_joy_info {
    //     char *name;
    //     uint32_t id;
    //     char opened;
    // } libenjoy_joy_info;
    //
    // id is not linear (eg. you should not use vector or array), 
    // and if you disconnect joystick and then plug it in again,
    // it should have the same ID
    info = libenjoy_get_info_list(ctx);

    if(info->count != 0) // just get the first joystick
    {
        libenjoy_joystick *joy;
        printf("Opening joystick %s...", info->list[0]->name);
        joy = libenjoy_open_joystick(ctx, info->list[0]->id);
        if(joy)
        {
            libenjoy_event ev;
            bool loop;
            
            printf("Success!\n");
            printf("Axes: %d btns: %d\n", libenjoy_get_axes_num(joy),libenjoy_get_buttons_num(joy));
            
            sigfn::handle(SIGINT,[&loop](int signum){
                loop = false;
            });

            loop = true;
            while(loop)
            {
                // Value data are not stored in library. if you want to use
                // them, you have to store them
                pong_rpi_client_event_loop(&ctx, &ev);
                // That's right, only polling available
            }

            // Joystick is really closed in libenjoy_poll or libenjoy_close,
            // because closing it while libenjoy_poll is in process in another thread
            // could cause crash. Be sure to call libenjoy_poll(ctx, NULL); (yes,
            // you can use NULL as event) if you will not poll nor libenjoy_close
            // anytime soon.
            libenjoy_close_joystick(joy);
        }
        else
            printf("Failed!\n");
    }
    else
        printf("No joystick available\n");

    // Frees memory allocated by that joystick list. Do not forget it!
    libenjoy_free_info_list(info);

    // deallocates all memory used by lib. Do not forget this!
    // libenjoy_poll must not be called during or after this call
    libenjoy_close(ctx);
    return 0;

}
