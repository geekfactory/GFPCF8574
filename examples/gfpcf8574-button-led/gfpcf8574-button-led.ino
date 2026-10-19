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

// Create a GFPCF8574 instance with I2C address (0x20) and the default Wire interface
GFPCF8574 io(0x20);

void setup()
{
	// pass true to io.begin() to initialize the I2C bus, this help keep
	// the main sketch cleaner by moving the Wire.begin() call into the library.
	// If you are sharing the I2C bus with other devices, you may want to initialize
	// the Wire interface externally and pass false to io.begin() or omit the
	// parameter in which case it defaults to false.
	io.begin(true);

	// configure pin 0 as input (button) and pin 7 as output (LED)
	io.pinMode(0, INPUT);
	io.pinMode(7, OUTPUT);

	// turn off the LED at startup
	io.digitalWrite(7, HIGH);
}

void loop()
{
	bool button;

	// copy the state of the button connected to pin 0 to the LED connected to pin 7
	if (io.digitalRead(0, button))
	{
		io.digitalWrite(7, button ? HIGH : LOW);
	}

	delay(20);
}
