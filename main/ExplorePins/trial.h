#define isPinI2C(p) (((p) == SDA) || ((p) == SCL))
#define isPinSPI(p) (((p) == SS) || ((p) == MOSI) || ((p) == MISO) || ((p) == SCK))
#define isPinDigital(p) ((p) < NUM_DIGITAL_PINS)

