#include <pebble.h>

Window *window;
TextLayer *text_date_layer;
TextLayer *text_time_layer;
TextLayer *text_parking_layer;
Layer *line_layer;

char date_text[] = "Xxxxxxxxx 00";
char parking_text[] = "Xxxxxx Xxx";
char* weeks[] = {NULL, NULL, NULL, NULL, NULL};

void line_layer_update_callback(Layer *layer, GContext* ctx) {
  graphics_context_set_fill_color(ctx, GColorWhite);
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, GCornerNone);
}

void update_date(struct tm *tick_time) {
  strftime(date_text, sizeof(date_text), "%B %e", tick_time);
  text_layer_set_text(text_date_layer, date_text);

  int week_num = (tick_time->tm_mday-1)/7;
  char *week_num_text = weeks[week_num];

  char *day_name = "Xxxxxxxxx";
  memset(day_name, 0, sizeof(day_name));
  strftime(day_name, sizeof(day_name), "%a", tick_time);

  memset(parking_text, 0, sizeof(parking_text));
  strcat(parking_text, week_num_text);
  strcat(parking_text, day_name);
  text_layer_set_text(text_parking_layer, parking_text);
}

void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // Need to be static because they're used by the system later.
  static char time_text[] = "00:00";

  char *time_format;
  bool day_changed = (units_changed & DAY_UNIT) > 0 ;
  if (day_changed || strcmp(parking_text, "Xxxxxx Xxx") == 0) {
      update_date(tick_time);
  }

  if (clock_is_24h_style()) {
    time_format = "%R";
  } else {
    time_format = "%I:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);

  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }

  text_layer_set_text(text_time_layer, time_text);
}

void handle_deinit(void) {
  tick_timer_service_unsubscribe();
}

void handle_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorBlack);

  Layer *window_layer = window_get_root_layer(window);

  text_date_layer = text_layer_create(GRect(8, 68, 144-8, 168-68));
  text_layer_set_text_color(text_date_layer, GColorWhite);
  text_layer_set_background_color(text_date_layer, GColorClear);
  text_layer_set_font(text_date_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_date_layer));

  text_time_layer = text_layer_create(GRect(7, 92, 144-7, 168-92));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49)));
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  text_parking_layer = text_layer_create(GRect(8,46, 144-6, 168-68));
  text_layer_set_text_color(text_parking_layer, GColorWhite);
  text_layer_set_background_color(text_parking_layer, GColorClear);
  text_layer_set_font(text_parking_layer, fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_21)));
  layer_add_child(window_layer, text_layer_get_layer(text_parking_layer));

  GRect line_frame = GRect(8, 97, 139, 2);
  line_layer = layer_create(line_frame);
  layer_set_update_proc(line_layer, line_layer_update_callback);
  layer_add_child(window_layer, line_layer);


  weeks[0] = "1st ";
  weeks[1] = "2nd ";
  weeks[2] = "3rd ";
  weeks[3] = "4th ";
  weeks[4] = "5th ";

  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  // TODO: Update display here to avoid blank display on launch?

}


int main(void) {
  handle_init();

  app_event_loop();

  handle_deinit();
}
