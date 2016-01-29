#include <FirmataFeatureLibrary.h>
#include <FirmataDeviceLibrary.h>

#include <FirmataCore.h>
#include <FirmataExt.h>
#include <FirmataFeature.h>

#include <Wire.h>

#include "SelectedFeatures.h"

/*

  Firmata is a generic protocol for communicating with microcontrollers from
  software on a host computer. It is intended to work with any host computer
  software package.

  FirmataCore is a pared down version of ConfigurableFirmata intended to
  be a stable base upon which various extensions can be built.
  FirmataCore was forked from ConfigurableFirmata, version 2.7.0, in
  October 2015.

  To download a host software package, please click on the following link
  to open the download page in your default browser.

  https://github.com/firmata/ConfigurableFirmata#firmata-client-libraries

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  See file LICENSE.txt for further informations on licensing terms.

  Base version of ConfigurableFirmata last updated by Jeff Hoefs April 25, 2015
  FirmataCore forked from that base October 2015 by Doug Johnson.
*/
/*==============================================================================
 * SETUP()
 *============================================================================*/

void setup()
{
// start up the default Firmata using Serial interface:

Firmata.begin(57600);

FirmataExt.addSelectedFeatures();

Firmata.reset();

}

/*==============================================================================
 * LOOP()
 *============================================================================*/
void loop() {

while (Firmata.available()) {
  Firmata.processInputStream();
}

FirmataExt.dispatchTimers();
}
