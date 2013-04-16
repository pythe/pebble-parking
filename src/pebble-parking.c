#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x5B, 0xD4, 0x13, 0x03, 0xE6, 0x8E, 0x46, 0x20, 0xA2, 0x8C, 0x09, 0xB9, 0x4B, 0xD8, 0x59, 0xE3 }
PBL_APP_INFO(MY_UUID,
                 "Parking", "Dungeoneer",
                              0, 1, /* App version */
                                           DEFAULT_MENU_ICON,
                                                        APP_INFO_STANDARD_APP);

Window window;


void handle_init(AppContextRef ctx) {
    (void)ctx;

      window_init(&window, "whoops");
        window_stack_push(&window, true /* Animated */);
}


void pbl_main(void *params) {
    PebbleAppHandlers handlers = {
          .init_handler = &handle_init
              };
      app_event_loop(params, &handlers);
}
