/*	GFPCF8574 - Geek Factory PCF8574 Library
	Copyright (C) 2026 Jesus Ruben Santa Anna Zamudio.

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published
	by the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser
	General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program. If not, see <http://www.gnu.org/licenses/>.

	Author website: https://www.geekfactory.mx
	Author e-mail: ruben at geekfactory dot mx
 */

/**
 * @file GFPCF8574.h
 * @brief Arduino library for the PCF8574 8-bit I2C I/O expander.
 *
 * Provides an Arduino-style interface for controlling the eight
 * quasi-bidirectional I/O pins of the PCF8574.
 *
 * Features include:
 * - Individual pin configuration and control.
 * - Full 8-bit port read and write operations.
 * - Software direction masking to protect input pins.
 * - Support for multiple I2C interfaces.
 * - I2C communication error reporting.
 *
 * @note The PCF8574 does not have a hardware direction register. Pins are
 * 	released HIGH to operate as inputs and driven LOW to operate as outputs.
 * 	Writing HIGH to a pin enables its weak internal current source.
 */

#pragma once

#include <Arduino.h>
#include <Wire.h>

/**
 * @class GFPCF8574
 * @brief Driver for the PCF8574 8-bit I2C I/O expander.
 *
 * This class provides basic digital I/O functionality through the PCF8574 using
 * Arduino like syntax and calls.
 *
 * A software direction mask ensures that pins configured as inputs are always
 * written HIGH, preventing them from being accidentally driven LOW during port
 * operations.
 *
 * The class maintains a cached output latch representing the requested output
 * states. This state is preserved even when an I2C transaction fails.
 *
 * @note INPUT and INPUT_PULLUP are functionally equivalent because the PCF8574
 *	 uses quasi-bidirectional I/O pins.
 *
 * @note The application must initialize the selected I2C interface before
 *	 calling begin(), unless initWire is set to true.
 */
class GFPCF8574
{
public:
	/**
	 * @brief Construct a PCF8574 device instance.
	 *
	 * Creates a driver instance associated with the specified I2C address
	 * and interface.
	 *
	 * All pins are initially configured as inputs, with their output
	 * latches set HIGH.
	 *
	 * @param address 7-bit I2C address of the device. Defaults to 0x20.
	 * @param wire Reference to the I2C interface. Defaults to the standard
	 * Wire instance.
	 */
	explicit GFPCF8574(uint8_t address = 0x20, TwoWire &wire = Wire);

	/**
	 * @brief Initialize the PCF8574 device.
	 *
	 * Optionally initializes the selected I2C interface and writes the
	 * current cached configuration to the device.
	 *
	 * On the first call, all pins are released HIGH because the initial
	 * input mask and output latch are both 0xFF.
	 *
	 * Subsequent calls retransmit the current configuration without
	 * resetting the cached pin states.
	 *
	 * @param initWire If true, calls begin() on the selected TwoWire
	 * 	 interface before accessing the device. Defaults to false.
	 *
	 * @return true if the I2C write succeeds.
	 * @return false if the I2C transaction fails.
	 *
	 * @note When sharing an I2C bus with other devices, initialize Wire
	 * externally and use initWire = false.
	 */
	bool begin(bool initWire = false);

	/**
	 * @brief Configure the software direction of an I/O pin.
	 *
	 * Configures a pin as INPUT, INPUT_PULLUP, or OUTPUT.
	 *
	 * Input pins are always released HIGH regardless of the corresponding
	 * cached output latch value.
	 *
	 * Output pins follow their cached output latch value.
	 *
	 * @param pin Pin number, from 0 to 7.
	 * @param mode Pin mode: INPUT, INPUT_PULLUP, or OUTPUT.
	 *
	 * @return true if the configuration was transmitted successfully.
	 * @return false if the pin or mode is invalid, or the I2C transaction
	 * 	 fails.
	 *
	 * @note INPUT and INPUT_PULLUP have identical behavior.
	 *
	 * @note Changing an input pin to OUTPUT immediately applies its
	 * 	 previously cached output latch value.
	 */
	bool pinMode(uint8_t pin, uint8_t mode);

	/**
	 * @brief Set the output latch value of an individual pin.
	 *
	 * Updates the cached output latch and transmits the complete port
	 * configuration to the device.
	 *
	 * Pins configured as inputs remain released HIGH regardless of the
	 * requested value.
	 *
	 * @param pin Pin number, from 0 to 7.
	 * @param value HIGH or LOW.
	 *
	 * @return true if the I2C write succeeds.
	 * @return false if the pin is invalid or the I2C transaction fails.
	 *
	 * @note Writing LOW to a pin configured as INPUT only modifies its
	 * 	 cached output latch. The pin remains released until configured
	 * 	 as OUTPUT.
	 *
	 * @note Writing HIGH releases the pin rather than actively driving it
	 * 	 HIGH with a push-pull output stage.
	 */
	bool digitalWrite(uint8_t pin, bool value);

	/**
	 * @brief Read the current logic level of an I/O pin.
	 *
	 * Reads the complete PCF8574 port and extracts the state of the
	 * specified pin.
	 *
	 * The returned value represents the actual pin level, not the cached
	 * output latch value.
	 *
	 * @param pin Pin number, from 0 to 7.
	 * @param[out] value Receives HIGH (true) or LOW (false) when the read
	 * 	 succeeds.
	 *
	 * @return true if the I2C read succeeds.
	 * @return false if the pin is invalid or the I2C transaction fails.
	 *
	 * @note The value parameter is left unchanged if the operation fails.
	 */
	bool digitalRead(uint8_t pin, bool &value);

	/**
	 * @brief Read the current state of all eight I/O pins.
	 *
	 * Reads one byte from the PCF8574 representing the physical logic
	 * levels of pins P0 through P7.
	 *
	 * Bit 0 corresponds to P0 and bit 7 corresponds to P7.
	 *
	 * @param[out] value Receives the 8-bit port state.
	 *
	 * @return true if the I2C read succeeds.
	 * @return false if the I2C transaction fails.
	 *
	 * @note This operation does not modify the cached output latch or
	 * 	 software direction mask.
	 *
	 * @note The value parameter is left unchanged if the
	 * 	 operation fails.
	 */
	bool readPort(uint8_t &value);

	/**
	 * @brief Write an 8-bit value to the output latch.
	 *
	 * Updates the cached output latch and transmits the resulting port
	 * configuration to the PCF8574.
	 *
	 * The software input mask is applied before transmission, ensuring that
	 * pins configured as inputs remain released HIGH regardless of the
	 * supplied value.
	 *
	 * Bit 0 corresponds to P0 and bit 7 corresponds to P7.
	 *
	 * @param value Desired 8-bit output latch value.
	 *
	 * @return true if the I2C write succeeds.
	 * @return false if the I2C transaction fails.
	 *
	 * @note The cached output latch is updated even when the I2C
	 * 	 transaction fails.
	 *
	 * @note Bits corresponding to input pins are also stored
	 * 	 in the output latch. These values become effective if the pins
	 * 	 are later configured as outputs.
	 */
	bool writePort(uint8_t value);

	/**
	 * @brief Get the configured I2C device address.
	 *
	 * @return The 7-bit I2C address associated with this instance.
	 */
	uint8_t getAddress() const { return _address; }

private:
	/**
	 * @brief Transmit the current port configuration.
	 *
	 * Combines the cached output latch with the software
	 * input mask and writes the resulting byte to the device.
	 *
	 * Input pins are forced HIGH using a bitwise OR operation.
	 *
	 * @return true if the I2C write succeeds.
	 * @return false if the I2C transaction fails.
	 */
	bool writeRegister();

	/** @brief 7-bit I2C address of the device. */
	uint8_t _address;

	/** @brief Reference to the selected Arduino I2C interface. */
	TwoWire &_wire;

	/**
	 * @brief Software input direction mask.
	 *
	 * A bit set to 1 indicates an input pin.
	 * A bit cleared to 0 indicates an output pin.
	 */
	uint8_t _inputMask;

	/**
	 * @brief Cached output latch values.
	 *
	 * Stores the requested output states independently
	 * of the software direction mask.
	 */
	uint8_t _outputLatch;
};
