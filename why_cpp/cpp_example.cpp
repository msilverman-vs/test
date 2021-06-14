
/* fake hal functions */
void hal_write(uint8_t port, uint8_t pin, uint8_t value) {

}
uint8_t hal_read(uint8_t port, uint8_t pin) {

}
/* fake unit test harness functions */
void mock_read(uint8_t value) {

}
void mock_write(uint8_t value) {

}

/**********************************/

/* the gpio interface */
class if_gpio {
public:
    virtual ~if_gpio() = default;
    virtual void read(uint8_t &value) = 0;
    virtual void write(uint8_t const value) = 0;
};

/* the 'real' gpio class */
class gpio :
    public if_gpio
{
public:
    struct config {
        uint8_t port; /* hal port */
        uint8_t pin; /* hal pin */
    };
    gpio(config cfg) {
        m_config = cfg;
    }
    virtual void read(uint8_t &value) override {
        value = hal_read(m_config.port, m_config.pin);
    }
    virtual void write(const uint8_t value) override {
        hal_write(m_config.port, m_config.pin, value);
    }
private:
    config m_config;
};

/* the mock gpio class */
class mock_gpio :
    public if_gpio
{
public:
    virtual void read(uint8_t &value) override {
        mock_read(value);
    }
    virtual void write(const uint8_t value) override {
        mock_write(value);
    }
};

/* class which uses a GPIO */
class something_fancy {
    struct config {
        if_gpio *gpio;
    };
    something_fancy(config cfg) {
        m_cfg = cfg;
    }
    void do_something() {
        /* read value */
        uint8_t value;
        m_cfg.gpio->read(value);
        /* value gets processed and stuff happens */
        m_cfg.gpio->write(value);
    }
    config m_cfg;
};

/* our 'real' main */
void cpp_main() {
    gpio::config gpio_config = {
        .pin = 1,
        .port = 3
    };
    gpio real_gpio(gpio_config);

    something_fancy::config config = {
       .gpio = &real_gpio
    };

    something_fancy something_fancy_obj(config);
    something_fancy_obj.do_something();
}

/* our main in unit testing */
void cpp_utest() {
    gpio mock_gpio;

    something_fancy::config config = {
       .gpio = &mock_gpio
    };

    something_fancy something_fancy_obj(config);
    /****** function under test *******/
    something_fancy_obj.do_something();
    /* mock expectations here */
}
