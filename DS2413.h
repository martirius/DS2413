#define DS2413_FAMILY_ID 0x3A
#define DS2413_ACCESS_READ 0xF5
#define DS2413_ACCESS_WRITE 0x5A
#define DS2413_ACK_SUCCESS 0xAA
#define DS2413_ACK_ERROR 0xFF
#include <OneWire.h>

class DS2413
{
private:
    OneWire *oneWire;
    uint8_t getIOState(uint8_t address[8])
    {
        bool ok = false;
        uint8_t results;

        oneWire->reset();
        oneWire->select(address);
        oneWire->write(DS2413_ACCESS_READ);

        results = oneWire->read();                /* Get the register results   */
        ok = (!results & 0x0F) == (results >> 4); /* Compare nibbles            */
        results &= 0x0F;                          /* Clear inverted values      */

        oneWire->reset();
        // return ok ? results : -1;
        return results;
    }

public:
    DS2413(OneWire *oneWire);
    ~DS2413();
    void setPinA(uint8_t address[8], uint8_t state)
    {
        uint8_t ack = 0;
        /* Top six bits must '1' */
        state |= 0xFC;
        oneWire->reset();
        oneWire->select(address);
        oneWire->write(DS2413_ACCESS_WRITE);
        oneWire->write(state);
        oneWire->write(~state); /* Invert data and resend     */
        ack = oneWire->read();  /* 0xAA=success, 0xFF=failure */
        if (ack == DS2413_ACK_SUCCESS)
        {
            oneWire->read(); /* Read the status byte      */
        }
        oneWire->reset();
    }
    void setPinB(uint8_t address[8], uint8_t state)
    {
        uint8_t ack = 0;
        /* Top six bits must '1' */
        state |= 0xFC;
        oneWire->reset();
        oneWire->select(address);
        oneWire->write(DS2413_ACCESS_WRITE);
        oneWire->write(state << 1);
        oneWire->write(~(state << 1)); /* Invert data and resend     */
        ack = oneWire->read();  /* 0xAA=success, 0xFF=failure */
        if (ack == DS2413_ACK_SUCCESS)
        {
            oneWire->read(); /* Read the status byte      */
        }
        oneWire->reset();
    
    }
    uint8_t getPinAState(uint8_t address[8])
    {
        uint8_t ioState = getIOState(address);
        return ioState == -1 ? -1 : ioState & 0x1;
    }
    uint8_t getPinBState(uint8_t address[8])
    {
        uint8_t ioState = getIOState(address);
        return ioState == -1 ? -1 : ioState & 0x2;
    }
};

DS2413::DS2413(OneWire *oneWire)
{
    this->oneWire = oneWire;
}

DS2413::~DS2413()
{
}
