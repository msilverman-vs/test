
/* fake hal functions */
void c_hal_write(uint8_t port, uint8_t pin, uint8_t value) {

}
uint8_t c_hal_read(uint8_t port, uint8_t pin) {

}
/* fake unit test harness functions */
void c_mock_read(uint8_t value) {

}
void c_mock_write(uint8_t value) {

}

/**********************************/

struct c_if_gpio {
    void (*read)(void *arg, uint8_t *value);
    void (*write)(void *arg, uint8_t const value);
};

struct c_gpio_config {
    uint8_t pin;
    uint8_t port;
};
struct c_gpio {
    struct c_if_gpio gpio_if;
    struct c_gpio_config cfg;
}

void real_read(void *arg, uint8_t *value) {
    struct c_gpio_config cfg = (struct c_gpio_config)arg;
    hal_read(cfg.port, cfg.pin, *value);
}
void real_write(void *arg, uint8_t const value) {
    struct c_gpio_config cfg = (struct c_gpio_config)arg;
    hal_write(cfg.port, cfg.pin, value);
}

void do_something(struct c_gpio gpio) {
    /* read value */
    uint8_t value;
    gpio.gpio_if->read((void *)&gpio.cfg, value);
    /* value gets processed and stuff happens */
    gpio.gpio_if->write((void *)&gpio.cfg, value);
}

void c_main() {
    struct c_if_gpio if_cfg;
    if_cfg.read = real_read;
    if_cfg.write = real_write;

    struct c_gpio_config cfg;
    cfg.pin = 1;
    cfg.port = 3;

    struct c_gpio gpio_obj;
    gpio_obj.cfg = cfg;
    gpio_obj.gpio_if = if_cfg;

    do_something(gpio_obj);

}

void cpp_utest() {
    struct c_if_gpio if_cfg;
    if_cfg.read = c_mock_read;
    if_cfg.write = c_mock_write;

    struct c_gpio gpio_obj;
    gpio_obj.gpio_if = if_cfg;

    do_something(gpio_obj);
}
