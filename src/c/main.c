#include <pebble.h>

static Window *s_main_window;
static BitmapLayer *s_background_layer;
static GBitmap *s_background_bitmap;
static TextLayer *s_left_time_layer;
static TextLayer *s_right_time_layer;
static GBitmap *s_nose_bitmap;
static GBitmap *s_nose_empty_bitmap;
static BitmapLayer *s_nose_layer;
static TextLayer *s_word_layer;
static TextLayer *s_birth_layer;
static TextLayer *s_forever_layer;
static GBitmap *s_eye_bitmap;
static BitmapLayer *s_eye_layer;

static void main_window_load(Window *window) {

  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  // add background image
  s_background_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_BACKGROUND);
  s_background_layer = bitmap_layer_create(bounds);
  
  bitmap_layer_set_bitmap(s_background_layer, s_background_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_background_layer));
  float background_left = (bounds.size.w - 144) / 2;
  float background_top = (bounds.size.h - 108) / 2;
  
  // add eye image
  s_eye_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_EYE);
  s_eye_layer = bitmap_layer_create(GRect(background_left + 15, background_top + 17, 111, 20));
  bitmap_layer_set_bitmap(s_eye_layer, s_eye_bitmap);
  layer_set_hidden((Layer *)s_eye_layer, true);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_eye_layer));
  
  // add hour and minute text
  s_left_time_layer = text_layer_create(
      GRect(background_left + 144 / 4 - 25, background_top + 5, 50, 36));
  
  text_layer_set_background_color(s_left_time_layer, GColorClear);
  text_layer_set_text_color(s_left_time_layer, GColorBlack);
  text_layer_set_text(s_left_time_layer, "00");
  text_layer_set_font(s_left_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_left_time_layer, GTextAlignmentCenter);

  layer_add_child(window_layer, text_layer_get_layer(s_left_time_layer));
  
  s_right_time_layer = text_layer_create(
      GRect(background_left + 144 / 4 * 3 - 25, background_top + 5, 50, 36));
  
  text_layer_set_background_color(s_right_time_layer, GColorClear);
  text_layer_set_text_color(s_right_time_layer, GColorBlack);
  text_layer_set_text(s_right_time_layer, "00");
  text_layer_set_font(s_right_time_layer, fonts_get_system_font(FONT_KEY_BITHAM_30_BLACK));
  text_layer_set_text_alignment(s_right_time_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(s_right_time_layer));
  
  // add nose/+1s image
  s_nose_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_NOSE);
  s_nose_empty_bitmap = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ADD_ONE);
  s_nose_layer = bitmap_layer_create(GRect(bounds.size.w / 2 - 16.5, background_top + 60, 33, 14));
  bitmap_layer_set_bitmap(s_nose_layer, s_nose_empty_bitmap);
  
  layer_add_child(window_layer, bitmap_layer_get_layer(s_nose_layer));
  
  // add word text
  s_word_layer = text_layer_create(
      GRect(0, bounds.size.h - 30, bounds.size.w, 15));
  text_layer_set_background_color(s_word_layer, GColorClear);
  text_layer_set_text_color(s_word_layer, GColorBlack);
  text_layer_set_text(s_word_layer, "Haha");
  text_layer_set_font(s_word_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
  text_layer_set_text_alignment(s_word_layer, GTextAlignmentCenter);
  
  layer_add_child(window_layer, text_layer_get_layer(s_word_layer));
  
  // add life text
  float battery_left = 20;
  s_birth_layer = text_layer_create(GRect(background_left + battery_left - 20, bounds.size.h - background_top - 30, 25, 15));
  text_layer_set_background_color(s_birth_layer, GColorClear);
  text_layer_set_text_color(s_birth_layer, GColorBlack);
  text_layer_set_text(s_birth_layer, "1926");
  text_layer_set_font(s_birth_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_birth_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_birth_layer));
  
  s_forever_layer = text_layer_create(GRect(bounds.size.w - background_left - battery_left, bounds.size.h - background_top - 30, 20, 15));
  text_layer_set_background_color(s_forever_layer, GColorClear);
  text_layer_set_text_color(s_forever_layer, GColorBlack);
  text_layer_set_text(s_forever_layer, "+∞");
  text_layer_set_font(s_forever_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(s_forever_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_forever_layer));
}

static void main_window_unload(Window *window) {
  gbitmap_destroy(s_background_bitmap);
  bitmap_layer_destroy(s_background_layer);
  text_layer_destroy(s_left_time_layer);
  text_layer_destroy(s_right_time_layer);
  gbitmap_destroy(s_nose_bitmap);
  gbitmap_destroy(s_nose_empty_bitmap);
  bitmap_layer_destroy(s_nose_layer);
  text_layer_destroy(s_word_layer);
  text_layer_destroy(s_birth_layer);
  text_layer_destroy(s_forever_layer);
  gbitmap_destroy(s_eye_bitmap);
  bitmap_layer_destroy(s_eye_layer);
}

static void update_time() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char s_hour_buffer[4];
  strftime(s_hour_buffer, sizeof(s_hour_buffer), clock_is_24h_style() ?
                                          "%H" : "%I", tick_time);
  text_layer_set_text(s_left_time_layer, s_hour_buffer);
  static char s_minute_buffer[4];
  strftime(s_minute_buffer, sizeof(s_minute_buffer), "%M", tick_time);
  text_layer_set_text(s_right_time_layer, s_minute_buffer);
  if (tick_time->tm_sec % 2 == 0) {
    layer_set_hidden((Layer *)s_eye_layer, true);
    layer_set_hidden((Layer *)s_left_time_layer, false);
    layer_set_hidden((Layer *)s_right_time_layer, false);
    bitmap_layer_set_bitmap(s_nose_layer, s_nose_empty_bitmap);
  } else {
    layer_set_hidden((Layer *)s_eye_layer, false);
    layer_set_hidden((Layer *)s_left_time_layer, true);
    layer_set_hidden((Layer *)s_right_time_layer, true);
    bitmap_layer_set_bitmap(s_nose_layer, s_nose_bitmap);
  }
  static char *s_words[] = {"You Naive!", "I'm angry!", "2 young 2 simple", "Wearing 3 watch", "Apply for Professor", "Excited!", "Sometimes naive!"};
  text_layer_set_text(s_word_layer, s_words[tick_time->tm_wday]);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

void init(void) {
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  window_stack_push(s_main_window, true);
  tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
  update_time();
}

void deinit(void) {
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


