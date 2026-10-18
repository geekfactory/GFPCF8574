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

#include "GFPCF8574.h"

GFPCF8574::GFPCF8574(uint8_t address, TwoWire &wire)
    : _address(address), _wire(wire), _inputMask(0xFF), _outputLatch(0xFF) {}

bool GFPCF8574::begin(bool initWire)
{
	if (initWire)
		_wire.begin();
	return writeRegister();
}

bool GFPCF8574::pinMode(uint8_t pin, uint8_t mode)
{
	if (pin > 7)
		return false;

	const uint8_t mask = static_cast<uint8_t>(1U << pin);

	if (mode == INPUT || mode == INPUT_PULLUP)
		_inputMask |= mask;
	else if (mode == OUTPUT)
		_inputMask &= static_cast<uint8_t>(~mask);
	else
		return false; // Invalid mode

	return writeRegister();
}

bool GFPCF8574::digitalWrite(uint8_t pin, bool value)
{
	if (pin > 7)
		return false;

	const uint8_t mask = static_cast<uint8_t>(1U << pin);

	if (value)
		_outputLatch |= mask;
	else
		_outputLatch &= static_cast<uint8_t>(~mask);

	return writeRegister();
}

bool GFPCF8574::digitalRead(uint8_t pin, bool &value)
{
	if (pin > 7)
		return false;

	uint8_t portValue;
	if (!readPort(portValue))
		return false;

	const uint8_t mask = static_cast<uint8_t>(1U << pin);
	value = (portValue & mask) != 0;
	return true;
}

bool GFPCF8574::writePort(uint8_t value)
{
	_outputLatch = value;
	return writeRegister();
}

bool GFPCF8574::readPort(uint8_t &value)
{
	if (_wire.requestFrom(_address, static_cast<uint8_t>(1)) != 1)
		return false;
	value = static_cast<uint8_t>(_wire.read());
	return true;
}

bool GFPCF8574::writeRegister()
{
	uint8_t value = _outputLatch | _inputMask;
	_wire.beginTransmission(_address);
	_wire.write(value);
	return _wire.endTransmission() == 0;
}
