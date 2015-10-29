<table width='100%'>
<tr>
<td width='75%' valign='top'>
<h1>Mechanic</h1>

This library allows you to query diagnostic values and other information<br>
from your car using an<br>
<a href='http://www.arduino.cc'>Arduino</a>
and a<br>
<a href='http://en.wikipedia.org/wiki/CAN_bus'>CAN bus</a>
shield. It assumes your car<br>
provides an<br>
<a href='http://en.wikipedia.org/wiki/On-board_diagnostics'>OBD-II interface</a>
that supports the CAN bus. This should be<br>
true for most recent vehicles, since it has been a standard since 2008.<br>
Even some older vehicles do have (possibly reduced) CAN bus support on the<br>
ODB-II interface.<br>
<br>
OBD-II, by the way, means "On-board Diagnostics, version II". The OBD-II<br>
interface is usually found inside the car, within reach of the driver's<br>
seat. The connector is large and almost rectangular. It looks a bit like<br>
the SCART connector on older TVs.<br>
<br>
<img src='http://mechanic.googlecode.com/files/picture1.jpg' />

Mechanic works fine with the following Arduino boards:<br>
<br>
<ul><li>Diecimila (ATmega168 and 328P, some sketches too big for 168)<br>
</li><li>Duemilanove (ATmega168 and 328P, some sketches too big for 168)<br>
</li><li>Uno<br>
</li><li>Leonardo<br>
</li><li>Mega (ATmega1280 and 2560)</li></ul>

As a CAN bus shield I recommend the<br>
<a href='http://www.watterott.com/de/Arduino-CANdiy-Shield'>CANdiy shield from Watterott</a>.<br>
This is<br>
not only because I had a hand in it's making, but because it offers the<br>
greatest flexibility for a really low price. In particular it will work<br>
with several different Arduino boards. The Mechanic library does operate<br>
fine on<br>
<a href='https://www.sparkfun.com/products/10039'>Sparkfun's</a>
or<br>
<a href='http://www.seeedstudio.com/wiki/CAN-BUS_Shield'>Seed Studio's</a>
CAN shields, too, but at least on the Sparkfun<br>
Shield it is limited to the Arduino Diecimila, Duemilanove or Uno, because<br>
this shield carries the SPI on pins 11-13 instead of the ICSP header.<br>
<br>
Note that when using the CANdiy shield you <b>must</b> have the 2x3 pin ICSP<br>
header soldered. Otherwise the Arduino cannot talk to the CAN controller.<br>
<br>
Here are the cable pinouts (RJ45 is for CANdiy, Sub-D is for Sparkfun and<br>
Seedstudio). I usually cut a CAT5/6 patch cable at one end and solder the<br>
OBD-II connector there.<br>
<br>
<table><thead><th> <b>Signal</b> </th><th> <b>OBD-II</b> </th><th> <b>RJ-45</b> </th><th> <b>Sub-D</b> </th></thead><tbody>
<tr><td> CAN_H         </td><td> 6             </td><td> 1            </td><td> 3            </td></tr>
<tr><td> CAN_L         </td><td> 14            </td><td> 2            </td><td> 5            </td></tr>
<tr><td> GND           </td><td> 5             </td><td> 7            </td><td> 9            </td></tr>
<tr><td> +12V          </td><td> 16            </td><td> 8            </td><td> 2            </td></tr></tbody></table>

Installation is easy: Just get the latest release from the downloads page<br>
and place its contents into a "Mechanic" directory under your Arduino<br>
"libraries" directory. Then restart Arduino. You should now see a bunch of<br>
new examples that teach you how to use Mechanic. The "SelfTest"<br>
example is a good way of validating your setup (unless your Arduino has an<br>
ATmega168, which doesn't have enough RAM for the sketch).<br>
<br>
There are two parameters in the sample sketches that might need to be<br>
adapted to your car: CAN bus speed can be 500 kbps or 250 kbps and<br>
identifiers can be standard or extended. Most vehicles I have come across<br>
seem to use the 500 kbps / standard IDs combination. The following table<br>
lists the ones I have tested successfully myself or have been reported to<br>
work by users. If your vehicle should be in this table please let me know.<br>
<br>
<table><thead><th> <b>Manufacturer</b> </th><th> <b>Model</b> </th><th> <b>Year</b> </th><th> <b>Speed</b> </th><th> <b>Identifiers</b> </th></thead><tbody>
<tr><td> Audi                </td><td> A4           </td><td> 2012        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> Ford                </td><td> B-Max        </td><td> 2013        </td><td> 500 kbps     </td><td> Both               </td></tr>
<tr><td> Ford                </td><td> Fiesta       </td><td> 2005        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> Ford                </td><td> Fiesta       </td><td> 2012        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> Opel                </td><td> Zafira       </td><td> 2008        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> Skoda               </td><td> Yeti         </td><td> 2013        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> Volvo               </td><td> V70          </td><td> 2013        </td><td> 500 kbps     </td><td> Standard           </td></tr>
<tr><td> VW                  </td><td> Caddy        </td><td> 2011        </td><td> 500 kbps     </td><td> Standard           </td></tr></tbody></table>

The repository also contains a small Android app that shows a live<br>
dashboard with some values transmitted from Arduino via Bluetooth. I use<br>
an inexpensive Bluetooth board from<br>
<a href='http://www.mib-instruments.com/servlet/the-2459/Arduino-Bluetooth-Module-Slave/Detail'>MiB Instruments</a> (around 10 US$, check<br>
eBay), but, again, there is no reason why others shouldn't work just as<br>
well assuming they provide a serial interface for communicating with the<br>
main Arduino board.<br>
<br>
<img src='http://mechanic.googlecode.com/files/picture2.png' />

For documentation on the functions I currently recommend to read the<br>
comments in the "Mechanic.h" header file. There is also an article in<br>
German<br>
<a href='http://www.heise.de/hardware-hacks/'>c't Hardware Hacks</a>
magazine 2/2013.<br>
<br>
The library itself is made available under the GNU Lesser General Public<br>
License (LGPL). See the LICENSE file for details. All examples except the<br>
test suite are licensed under the Creative Commons Zero license, which<br>
effectively makes them public domain. The test suite sketch is licensed<br>
under the LGPL.<br>
<br>
<br>
Mechanic uses parts of a<br>
<a href='http://www.kreatives-chaos.com/artikel/universelle-can-bibliothek'>CAN library</a>
which is (c) 2007 Fabian Greif and published under a BSD-style license.<br>
<br>
Have fun!<br>
<br>
</td>
<td width='25%' valign='top'>
<h1>News</h1>
<ul>

<li>2013-10-28: Mechanic version 0.6 available for download. The library now works with more Arduino boards (see list to the left). The updated Android app is able to show the vehicle ID and trouble codes.<br>
</li>

<li>2013-02-22: Mechanic featured in German <a href='http://shop.heise.de/katalog/ct-hardware-hacks-2-2013'>c't Hardware Hacks</a> magazine. This article should give a good introduction to the project.<br>
</li>

<li>2013-05-21: Mechanic version 0.5 available for download. This is the first stable release of the library and the companion Android app. You can find some pictures <a href='https://plus.google.com/photos/100423523947235262131/albums/5880515454530644273?authkey=CJ3BmvPUp9zq_QE'>here</a>.</li>

<li>2013-05-05: Project goes online. This is actually a <a href='http://code.google.com/p/railuino/'>Railuino</a> spin-off using the same hardware.</li>

</ul>

<h1>Hardware</h1>
<ul>

<li>If you are looking for the CANdiy Shield, <a href='http://www.watterott.com/de/Arduino-CANdiy-Shield'>Watterott</a> has it (and everything else you need for the project).<br>
</li>

</ul>

</td>

</tr>
</table>