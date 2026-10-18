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
	bool state;

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
