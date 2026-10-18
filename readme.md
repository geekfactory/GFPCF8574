# GFPCF8574 - Geek Factory PCF8574 Library #

This library provides a simple and lightweight interface for controlling the **PCF8574 8-bit I²C I/O expander**. **This library is designed to be used with the Arduino framework using the Arduino IDE or PlatformIO**.

The PCF8574 is a low-cost integrated circuit that allows a microcontroller to control or monitor 8 additional digital I/O pins using only two I²C signals (SDA and SCL). Multiple devices can share the same I²C bus, making it an excellent solution for projects that require additional digital inputs or outputs without consuming more microcontroller pins.

The library was designed to provide an interface similar to Arduino's standard digital I/O functions, while keeping the implementation small, efficient, and easy to understand.

The PCF8574 and compatible breakout modules are readily available in most countries for a couple of dollars. They can be used with Arduino Uno, Nano, M0, and other Arduino-compatible microcontrollers.

## Library Features ##

* Simple Arduino-style API using `pinMode()`, `digitalWrite()`, and `digitalRead()`.
* Support for individual pin operations and full 8-bit port access.
* Software direction register to protect pins configured as inputs.
* Support for multiple PCF8574 devices on the same I²C bus.
* Support for multiple hardware I²C interfaces through the `TwoWire` class.
* I²C communication error reporting through boolean return values.
* Optional initialization of the I²C interface in the `begin()` method.
* No dynamic memory allocation or additional library dependencies beyond Arduino Wire.

## Supported Hardware ##

This library supports the **PCF8574** 8-bit I²C I/O expander and should also work with the **PCF8574A**, provided that the appropriate I²C address is specified.

The PCF8574 typically supports addresses from `0x20` to `0x27`, while the PCF8574A uses addresses from `0x38` to `0x3F`.

The library uses the standard Arduino `Wire` interface and is intended to work with boards such as:

* Arduino Uno and Nano (ATmega328P).
* Arduino M0 and other SAMD21-based boards.
* Raspberry Pi Pico and Pico 2 using an Arduino-compatible core.
* ESP32 and other boards with compatible `TwoWire` implementations.

## Important Hardware Considerations ##

Unlike conventional microcontroller GPIO pins, the PCF8574 uses **quasi-bidirectional I/O pins**.

Each pin can be driven LOW or released HIGH through an internal weak current source. There is no hardware direction register.

When a pin is configured as an input, the library ensures that it remains released HIGH. This prevents full-port write operations from accidentally driving input pins LOW.

The library implements a software direction mask to provide familiar Arduino-style pin configuration.

* `INPUT` releases the pin HIGH so it can be used as an input.
* `INPUT_PULLUP` behaves identically to `INPUT`.
* `OUTPUT` allows the pin to follow its cached output state.
* `digitalWrite(pin, LOW)` drives the pin LOW when configured as an output.
* `digitalWrite(pin, HIGH)` releases the pin HIGH rather than actively driving it HIGH.

For LED applications, it is generally preferable to connect the LED and its current-limiting resistor between VCC and the PCF8574 output, allowing the device to sink current.

## Library Installation ##

Download or clone this repository and copy the `GFPCF8574` directory into your Arduino libraries folder.

Alternatively, when using PlatformIO, add the library to your project's `lib` directory or reference the Git repository in your project dependencies.

The library only requires the standard Arduino `Wire` library.

## Basic Usage ##

The following example configures P0 as an output and blinks an LED connected between VCC and P0 through a suitable current-limiting resistor.

```cpp
#include <Wire.h>
#include <GFPCF8574.h>

GFPCF8574 io(0x20);

void setup()
{
    Wire.begin();

    if (!io.begin()) {
        // Device initialization failed
        while (true);
    }

    io.pinMode(0, OUTPUT);
}

void loop()
{
    io.digitalWrite(0, LOW);
    delay(500);

    io.digitalWrite(0, HIGH);
    delay(500);
}
```

The PCF8574 output sinks current when LOW, so the LED turns on when the pin is driven LOW.

## Reading Digital Inputs ##

The following example reads a pushbutton connected between P0 and GND.

```cpp
#include <Wire.h>
#include <GFPCF8574.h>

GFPCF8574 io(0x20);

void setup()
{
    Serial.begin(115200);
    Wire.begin();

    io.begin();
    io.pinMode(0, INPUT);
}

void loop()
{
    bool button;

    if (io.digitalRead(0, button)) {
        Serial.println(button ? "Released" : "Pressed");
    } else {
        Serial.println("I2C communication error");
    }

    delay(100);
}
```

The `digitalRead()` method returns `true` when the I²C transaction succeeds. The actual logic level is returned through the reference parameter.

This allows applications to distinguish a LOW input from a communication error.

## Using Multiple I²C Interfaces ##

The library supports microcontrollers with more than one I²C interface.

The desired `TwoWire` instance can be supplied to the constructor.

```cpp
#include <Wire.h>
#include <GFPCF8574.h>

GFPCF8574 io1(0x20, Wire);
GFPCF8574 io2(0x21, Wire1);

void setup()
{
    Wire.begin();
    Wire1.begin();

    io1.begin();
    io2.begin();

    io1.pinMode(0, OUTPUT);
    io2.pinMode(0, OUTPUT);
}

void loop()
{
    io1.digitalWrite(0, LOW);
    io2.digitalWrite(0, HIGH);
    delay(500);

    io1.digitalWrite(0, HIGH);
    io2.digitalWrite(0, LOW);
    delay(500);
}
```

**Note:** The availability of `Wire1` depends on the selected microcontroller and Arduino core.

## Full Port Operations ##

The library also allows reading and writing all eight pins in a single I²C transaction.

```cpp
uint8_t portState;

// Write the output latch
io.writePort(0x55);

// Read the physical pin states
if (io.readPort(portState)) {
    Serial.println(portState, BIN);
}
```

The software direction mask is automatically applied to every write operation. Pins configured as inputs remain released HIGH regardless of the value passed to `writePort()`.

The output latch stores the requested state of every pin, including those currently configured as inputs. These stored values become effective if the corresponding pins are later configured as outputs.

## API Reference ##

| Method | Description |
|---|---|
| `GFPCF8574(address, wire)` | Creates a device instance with an optional I²C interface. |
| `begin(initWire)` | Initializes the device and optionally the I²C interface. |
| `pinMode(pin, mode)` | Configures a pin as INPUT, INPUT_PULLUP, or OUTPUT. |
| `digitalWrite(pin, value)` | Sets the cached output state of an individual pin. |
| `digitalRead(pin, value)` | Reads the physical logic level of an individual pin. |
| `writePort(value)` | Updates the complete 8-bit output latch. |
| `readPort(value)` | Reads the physical state of all eight pins. |
| `getAddress()` | Returns the configured I²C device address. |

All methods that communicate with the device return a boolean indicating whether the operation succeeded.

The library preserves the requested output and direction configuration even when an I²C transaction fails. Subsequent write operations retransmit the complete cached configuration.

## Examples ##

The repository includes examples demonstrating the most common operations:

* **BasicOutput:** Blink an LED using a PCF8574 output.
* **BasicInput:** Read a pushbutton using a PCF8574 input.
* **ButtonAndLED:** Control an LED using a pushbutton connected to the same expander.
* **PortReadWrite:** Read and write the complete 8-bit port.
* **MultipleDevices:** Control multiple PCF8574 devices sharing an I²C bus.
* **MultipleBuses:** Use separate hardware I²C interfaces on supported microcontrollers.

## License ##

GFPCF8574 is free software distributed under the **GNU Lesser General Public License version 3 or later (LGPL-3.0-or-later)**.

Copyright (C) 2026 Jesus Ruben Santa Anna Zamudio.

For more information, visit [Geek Factory](https://www.geekfactory.mx).

## Contributions ##

Bug reports, suggestions, improvements, and contributions are welcome. Feel free to open an issue or submit a pull request to help improve the library.