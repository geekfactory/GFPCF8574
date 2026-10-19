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
	// Prepare serial communication for debugging
	Serial.begin(9600);
	// initialize the I2C bus
	Wire.begin();
	// initialize the PCF8574 device
	io.begin();
	// configure pin 7 as input (button)
	io.pinMode(0, INPUT);
}

void loop()
{
	bool state;
	// read the state of the button connected to pin 0 and print the result
	// note that the buton state is placed in the 'state' variable by reference
	if (io.digitalRead(0, state))
	{
		Serial.println(state ? "Released" : "Pressed");
	}
	else
	{
		Serial.println("I2C communication error");
	}

	delay(100);
}
