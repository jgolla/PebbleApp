#include <pebble.h>
#include <pebble_fonts.h>

static Window *window;
static TextLayer *text_layer;
static char title[5];

enum {
  QUOTE_KEY_TITLE = 0x0,
  QUOTE_KEY_DESCRIPTION = 0x1,
  QUOTE_KEY_FETCH = 0x2
};

void out_sent_handler(DictionaryIterator *sent, void *context) {
}

void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void in_received_handler(DictionaryIterator *received, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "in_received_handler");

  Tuple *title_tuple = dict_find(received, QUOTE_KEY_TITLE);

  if (title_tuple) {
    APP_LOG(APP_LOG_LEVEL_INFO, title_tuple->value->cstring);
    strncpy(title, title_tuple->value->cstring, 5);
    text_layer_set_text(text_layer, title);
  }
  /*if (description_tuple) {
    strncpy(price, description_tuple->value->cstring, 10);
    text_layer_set_text(price_layer, price);
  }*/
}

void in_dropped_handler(AppMessageResult reason, void *context) {
}

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 20 }, .size = { bounds.size.w, 50 } });
  //text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);
}

static void fetch_msg(void) {
  APP_LOG(APP_LOG_LEVEL_INFO, "FETching message");

  Tuplet fetch_tuple = TupletInteger(QUOTE_KEY_FETCH, 1);

  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);

  if (iter == NULL) {
    return;
  }

  dict_write_tuplet(iter, &fetch_tuple);
  dict_write_end(iter);

  app_message_outbox_send();
}

static void app_message_init(void) {
  // Register message handlers
  app_message_register_inbox_received(in_received_handler);
  app_message_register_inbox_dropped(in_dropped_handler);
  app_message_register_outbox_failed(out_failed_handler);
  // Init buffers
  app_message_open(64, 64);
  fetch_msg();
}

static void init(void) {
  window = window_create();
  app_message_init();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;

  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_INFO, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}