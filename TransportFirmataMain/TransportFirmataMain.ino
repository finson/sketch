/*

  Firmata is a generic protocol for communicating with microcontrollers from
  software on a host computer. It is intended to work with any host computer
  software package.

  TransportFirmata is a pared down version of ConfigurableFirmata intended to
  provide only a basic transport link between systems upon which other protocols
  can be built.  TransportFirmata was forked from ConfigurableFirmata, version
  2.7.0, in October 2015.

  To download a host software package, please click on the following link
  to open the download page in your default browser.

  https://github.com/firmata/ConfigurableFirmata#firmata-client-libraries

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  Base version of ConfigurableFirmata last updated by Jeff Hoefs April 25, 2015
  TransportFirmata forked from that base October 2015 by Doug Johnson.
*/

#include <TransportFirmata.h>
#include <FirmataExt.h>

// To configure, save this file to your working directory so you can edit it
// then comment out the include and declaration for any features that you do
// not need below.

#include <DigitalInputFirmata.h>
DigitalInputFirmata digitalInput;

#include <DigitalOutputFirmata.h>
DigitalOutputFirmata digitalOutput;

#include <AnalogInputFirmata.h>
AnalogInputFirmata analogInput;

#include <AnalogOutputFirmata.h>
AnalogOutputFirmata analogOutput;

/*==============================================================================
 * SETUP()
 *============================================================================*/

void setup()
{

#ifdef DigitalInputFirmata_h
  FirmataExt.addFeature(digitalInput);
#endif
#ifdef DigitalOutputFirmata_h
  FirmataExt.addFeature(digitalOutput);
#endif
#ifdef AnalogInputFirmata_h
  FirmataExt.addFeature(analogInput);
#endif
#ifdef AnalogOutputFirmata_h
  FirmataExt.addFeature(analogOutput);
#endif

// start up the default Firmata using Serial interface:

Firmata.begin(57600);

Firmata.reset();
}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop()
{
#ifdef DigitalInputFirmata_h
  /* DIGITALREAD - as fast as possible, check for changes and output them to the
   * stream buffer using Firmata.write()  */
  digitalInput.report();
#endif

  /* STREAMREAD - processing incoming messagse as soon as possible, while still
   * checking digital inputs.  */
  while (Firmata.available()) {
    Firmata.processInput();
  }

  /* SEND STREAM WRITE BUFFER - TO DO: make sure that the stream buffer doesn't go over
   * 60 bytes. use a timer to sending an event character every 4 ms to
   * trigger the buffer to dump. */

  if (Firmata.elapsed()) {
#ifdef AnalogInputFirmata_h
    /* ANALOGREAD - do all analogReads() at the configured sampling interval */
    analogInput.report();
#endif
  }
}
