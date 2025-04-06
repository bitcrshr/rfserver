#include "http.h"
#include "command.h"
#include "mongoose/mongoose.h"
#include <boards/pico_w.h>
#include <string.h>
#include <hardware/watchdog.h>
#include <pico/cyw43_arch.h>

#define MAX_METHOD_STR_LEN

static void mg_str_to_str(struct mg_str *mgs, char *buf, size_t buflen) {
  if (mgs == NULL || mgs->buf == NULL || buf == NULL) {
    return;
  }

  if (mgs->len > 0 && mgs->buf[mgs->len - 1] == '\0') {
    if (mgs->len <= buflen) {
      memcpy(buf, mgs->buf, mgs->len);
    } else {
      memcpy(buf, mgs->buf, buflen);
    }

    return;
  }

  if (mgs->len + 1 > buflen) {
    return;
  }

  memcpy(buf, mgs->buf, mgs->len);
  buf[mgs->len] = '\0';
}

inline static bool nt(const struct mg_str *s) {
  if (s == NULL || s->buf == NULL || s->len == 0) {
    return false;
  }

  return s->buf[s->len - 1] == '\0';
}

static void handle_exec_cmd(struct mg_connection *c,
                            struct mg_http_message *req, struct rf_cfg *cfg) {
  struct mg_str *body = &req->body;

  if (body->len > MAX_COMMAND_STR_LEN) {
    MG_ERROR(("command of len %d was too large to be valid (must be < %d)",
              body->len, MAX_COMMAND_STR_LEN));

    mg_http_reply(c, 400, "", "invalid command");
    return;
  }

  enum Command cmd = INVALID;
  if (nt(body)) {
    char cmd_str[body->len];
    mg_str_to_str(body, cmd_str, body->len);

    MG_INFO(("exec cmd: %s", cmd_str));

    cmd = cmd_from_str(cmd_str);
  } else {
    char cmd_str[body->len + 1];
    mg_str_to_str(body, cmd_str, body->len + 1);

    MG_INFO(("exec cmd: %s", cmd_str));

    cmd = cmd_from_str(cmd_str);
  }

  if (cmd == INVALID) {
    MG_ERROR(("invalid cmd"));

    mg_http_reply(c, 400, "", "invalid command");
    return;
  }

  MG_INFO(("about to call send_cmd"));
  send_cmd(cmd);
  MG_INFO(("send_cmd done"));

  mg_http_reply(c, 200, "", "");
}

static void handle_get_state(struct mg_connection *c, struct rf_cfg *cfg) {
  if (cfg->state == INVALID) {
    MG_INFO(("no current command state"));
    mg_http_reply(c, 200, "", "");
    return;
  }

  const char *cmd_str = cmd_to_str(cfg->state);

  MG_INFO(("current state is %s", cmd_str));

  mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "%s", cmd_str);
}

static void handle_not_found(struct mg_connection *c) {
  mg_http_reply(c, 404, "", "Not Found");
}

static void handle_bad_method(struct mg_connection *c) {
  mg_http_reply(c, 405, "", "Method not allowed");
}

static void handler(struct mg_connection *c, int ev, void *ev_data) {
  if (ev != MG_EV_HTTP_MSG) {
    return;
  }

  struct mg_http_message *req = (struct mg_http_message *)ev_data;
  struct rf_cfg *cfg = (struct rf_cfg *)c->fn_data;

  if (mg_match(req->uri, mg_str("/command/exec"), NULL)) {
    if (mg_strcasecmp(req->method, mg_str("POST"))) {
      char method_str[5];
      mg_str_to_str(&req->method, method_str, 5);

      MG_ERROR(("bad method for /command/exec: %s", method_str));

      handle_bad_method(c);
      return;
    }

    handle_exec_cmd(c, req, cfg);
    return;
  }

  if (mg_match(req->uri, mg_str("/state"), NULL)) {
    if (mg_strcasecmp(req->method, mg_str("GET"))) {
      MG_ERROR(("bad method for /state"));

      handle_bad_method(c);
      return;
    }

    handle_get_state(c, cfg);
    return;
  }

  char url_str[req->uri.len + 1];
  mg_str_to_str(&req->uri, url_str, req->uri.len + 1);

  MG_ERROR(("not found: %s", url_str));

  handle_not_found(c);
}

void listen(const char *uri, struct rf_cfg *cfg) {
  mg_log_set(MG_LL_DEBUG);

  if (watchdog_enable_caused_reboot()) {
    MG_INFO(("BOOTING AFTER WATCHDOG CAUSED RESET"));
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
  }

  struct mg_mgr mgr;
  mg_mgr_init(&mgr);

  mg_http_listen(&mgr, uri, handler, cfg);

  watchdog_enable(1000, 1);

  while (true) {
    mg_mgr_poll(&mgr, 10);
    watchdog_update();
  }
}
