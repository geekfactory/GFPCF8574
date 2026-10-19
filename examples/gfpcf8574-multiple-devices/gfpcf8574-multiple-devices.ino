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

#include <GFPCF8574.h>

// Create two GFPCF8574 instances with different I2C addresses (0x20 and 0x21)
// and use the default Wire interface for both. This allows you to control two
// separate PCF8574 devices on the same I2C bus.
GFPCF8574 io1(0x20);
GFPCF8574 io2(0x21);

void setup()
{
	// Initialize the I2C bus
	Wire.begin();

	// Initialize both instances
	io1.begin();
	io2.begin();

	// Configure pin 0 on both devices as output
	io1.pinMode(0, OUTPUT);
	io2.pinMode(0, OUTPUT);
}

void loop()
{
	// blink the LED connected to pin 0 on both devices simultaneously
	io1.digitalWrite(0, LOW);
	io2.digitalWrite(0, HIGH);
	delay(500);

	io1.digitalWrite(0, HIGH);
	io2.digitalWrite(0, LOW);
	delay(500);
}
