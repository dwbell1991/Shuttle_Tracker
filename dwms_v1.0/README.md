# Dude Where's My Shuttle?
## Prototype Version 1.0

The goal of this application is to update students of the University of Nebraska
campus of when a shuttle is going to show up. To accomplish this we used a Raspberry Pi (RPi) and FONA 808 GSM/GPS breakout board.
The FONA is capable of using serial "AT" commands to get, set, receive, and transfer data from it's built in GPS and 2G GSM hardware.

## To begin the following hardware was used:
<ol>
	<li><a href="https://www.raspberrypi.org/products/model-b/">Raspberry Pi Model B</a></li>
	<li><a href="https://www.adafruit.com/products/2542">FONA 808 GSM/GPS Breakout</a></li>
	<li><a href="https://www.adafruit.com/products/746">Adafruit Ultimate GPS Breakout</a></li>
	<li><a href="https://www.adafruit.com/products/954">USB to TTL Serial Cable</a></li>
	<li><a href="https://www.adafruit.com/products/1991">Quad-Band GSM 3dBi uFL Antenna</a></li>
	<li><a href="https://www.adafruit.com/products/2460">Passive GPS -2dBi uFL Antenna</a></li>
	<li><a href="https://www.adafruit.com/product/1578">Lithium Ion 3.7v 500mAh Battery</a></li>
	<li>
		<a href="http://www.edimax.com/edimax/merchandise/merchandise_detail/data/edimax/global/wireless_adapters_n150/ew-7811un">Edimax USB Wi-Fi Adapter</a>
	</li>
	<li>SD card (4GB+ recommended)</li> 
</ol>

### Hardware Setup:
Hardware setup for serial communication between the RPi and FONA:
#### Pin setup:
<ul>
	<li>RPi 3.3 volt to FONA Vio</li>
	<li>RPi Ground to FONA GND</li>
	<li>RPi TXD to FONA RX</li>
	<li>RPi RXD to FONA TX</li>
	<li>Optional Pins</li>
		<ul>
			<li>FONA Key to FONA GND</li>
		</ul>
</ul>
Hardware setup for serial communication between the RPi and GPS breakout via USB serial
<ul>
	<li>Serial Red to GPS breakout VIN</li>
	<li>Serial Black to GPS breakout GND</li>
	<li>Serial Green to GPS breakout RX</li>
	<li>Serial White to GPS breakout TX</li>
</ul>

## The following software was used:
<ol>
	<li>Raspbian Jessie Lite <a href="https://drive.google.com/file/d/0B4dklNKVEGmEdVVMMTlDWldBLWM/view?usp=sharing" >(2015-11-21-raspbian-jessie-lite.zip</a>)</li>
	<p>Burn Rasian to the SD card using dd, rufus, or other utilites to burn the image file.</p>
	<p>SSH into the RPi while connected via ethernet or connect the RPi to a monitor and keyboard in order to:</p>
	<li>Run the following commands on to install software on the RPi:</li>
		<pre><code>sudo apt-get update</code></pre>
		<pre><code>sudo apt-get install ppp screen isc-dhcp-server hostapd gcc</code></pre>
	<p>This will install all the necessary software from the repositories to interface and test the hardware.</p>
	<li>To test if the FONA and GPS breakout boards are able to communicate to the RPi the 		<i>screen</i> program can be used (minicom or other serial consoles work).
		<p>To test the FONA try:</p>
		<pre><code>sudo screen /dev/ttyAMA0 115200</pre></code>
		<p>Then try typing "AT" if you get an "OK" then your hardware should be correctly setup.</p>
		<p>For the GPS breakout use:</p>
		<pre><code>sudo screen /dev/ttyUSB0 9600</pre></code>
		<p>If a bunch of random $GP** output is dumped then the hardware should be correctly setup.</p>
		<p>The above hardware device files may be different depending on the users setup to find your devices use the <i>ls</i> command.</p>
	</li>
</ol>
Copy config files from rpi_config_files directory to their proper location (most go in the /etc/ directory).
