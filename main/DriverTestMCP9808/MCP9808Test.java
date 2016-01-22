package ws.tuxi.lib.firmata.test.connected.MCP9808;

import static ws.tuxi.lib.firmata.test.tools.Hope.hopeEquals;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.IOException;
import java.util.Collection;

import ws.tuxi.lib.firmata.call.ProxyDeviceDriver;
import ws.tuxi.lib.firmata.call.MCP9808.MCP9808;
import ws.tuxi.lib.firmata.call.MCP9808.MCP9808.Register;
import ws.tuxi.lib.firmata.call.MCP9808.MCP9808Library;
import ws.tuxi.lib.firmata.feature.device.DeviceDriver;
import ws.tuxi.lib.firmata.test.tools.FirmataPreAndPost;
import ws.tuxi.lib.firmata.test.tools.FirmataTestWatcher;
import ws.tuxi.lib.firmata.test.tools.TestParameters;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.TestRule;
import org.junit.runner.RunWith;
import org.junit.runners.Parameterized;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * This LiveMCP9808Test class tests an MCP9808 Device Driver implementation on an Arduino system and
 * simultaneously exercises the DeviceFeature and DeviceDriver capabilities of Firmata. This test
 * requires that an MCP9808 temperature sensor (Adafruit) be connected to the Arduino server.
 *
 * @author Doug Johnson
 * @since Nov 2015
 *
 */
@RunWith(Parameterized.class)
public class LiveMCP9808Test {
    private final Logger logger = LoggerFactory.getLogger(this.getClass());

    private final FirmataPreAndPost pap;

    private MCP9808Library device;

    // --------------------------------------------------------

    @Rule
    public TestRule watcher = new FirmataTestWatcher();

    // --------------------------------------------------------

    @Parameterized.Parameters
    public static Collection<?> deviceDrivers() {
        return TestParameters.getConnectedTestParameters("MCP9808");
    }

    // --------------------------------------------------------

    public LiveMCP9808Test(String deviceName, String portName) throws Exception {
        pap = new FirmataPreAndPost(portName);
        logger.info("Starting {} test of server {} on port {}", this.getClass().getSimpleName(),deviceName, portName);
    }

    // --------------------------------------------------------

    @Before
    public void setUpBeforeTest() throws Exception {
        pap.doSetUpBeforeTest();
        device = new MCP9808Library(new ProxyDeviceDriver(pap.getFiq(), pap.getFoq()));
        logger.info("{} setUpBeforeTest. fiq: {}, foq: {}", this.getClass().getSimpleName(),
                pap.getFiq().getClass().getSimpleName(), pap.getFoq().getClass().getSimpleName());
    }

    // --------------------------------------------------------

    @After
    public void cleanUpAfterTest() throws Exception {
        pap.doCleanUpAfterTest();
        logger.info("{} cleanUpAfterTest is done.", this.getClass().getSimpleName());
    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808OpenClose() throws IOException, ReflectiveOperationException {

        int status = device.open( "MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));
        int handle = status;
        status = device.close(handle);
        assertTrue("Close error.", (status>=0));

    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808ReadMethod() throws IOException, ReflectiveOperationException {
        byte[] buf = new byte[2];
        int status = device.open( "MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));

        int handle = status;
        status = device.read(handle, 2, buf);
        assertEquals("Read error:", 2, status);

        logger.info("Read 2 bytes from {}: 0x{} 0x{}", device.toString(handle),
                Integer.toHexString(Byte.toUnsignedInt(buf[0])),
                Integer.toHexString(Byte.toUnsignedInt(buf[1])));

        status = device.close(handle);
        assertTrue("Close error.", (status>=0));
    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808StatusMethod() throws IOException, ReflectiveOperationException {
        byte[] buf = new byte[18];
        int status = device.open("MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));
        int handle = status;

        for (Register r : Register.values()) {
            int count = (r == Register.RESOLUTION) ? 1 : 2;

            status = device.status(handle, r.bits(), count, buf);
            assertEquals("Status error:", count, status);

            if (count == 1) {
                logger.info("Read {} status byte from reg {}: 0x{}", count, r.toString(),
                        Integer.toHexString(Byte.toUnsignedInt(buf[0])));
            } else {
                logger.info("Read {} status bytes from reg {}: 0x{} 0x{}", count, r.toString(),
                        Integer.toHexString(Byte.toUnsignedInt(buf[0])),
                        Integer.toHexString(Byte.toUnsignedInt(buf[1])));
            }

            switch (r) {
            case RESERVED:
                hopeEquals("MCP9808 Reg " + Register.RESERVED + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                hopeEquals("MCP9808 Reg " + Register.RESERVED + " (LSB)", 0x1d,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case CONFIG:
                hopeEquals("MCP9808 Reg " + Register.CONFIG + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                hopeEquals("MCP9808 Reg " + Register.CONFIG + " (LSB)", 0,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case UPPER_TEMP:
                assertEquals("MCP9808 Reg " + Register.UPPER_TEMP + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                assertEquals("MCP9808 Reg " + Register.UPPER_TEMP + " (LSB)", 0,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case LOWER_TEMP:
                assertEquals("MCP9808 Reg " + Register.LOWER_TEMP + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                assertEquals("MCP9808 Reg " + Register.LOWER_TEMP + " (LSB)", 0,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case CRIT_TEMP:
                assertEquals("MCP9808 Reg " + Register.CRIT_TEMP + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                assertEquals("MCP9808 Reg " + Register.CRIT_TEMP + " (LSB)", 0,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case AMBIENT_TEMP:
                assertEquals("MCP9808 Reg " + Register.AMBIENT_TEMP + " (MSB)", 0xc1,
                        Byte.toUnsignedInt(buf[0]));
                // assertEquals("MCP9808 Reg " + Register.AMBIENT_TEMP + " (LSB)", 0,
                // Byte.toUnsignedInt(buf[1])); // room
                break;
            case MANUF_ID:
                assertEquals("MCP9808 Reg " + Register.MANUF_ID + " (MSB)", 0,
                        Byte.toUnsignedInt(buf[0]));
                assertEquals("MCP9808 Reg " + Register.MANUF_ID + " (LSB)", 0x54,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case DEVICE_ID:
                assertEquals("MCP9808 Reg " + Register.DEVICE_ID + " (MSB)", 0x4,
                        Byte.toUnsignedInt(buf[0]));
                assertEquals("MCP9808 Reg " + Register.DEVICE_ID + " (LSB)", 0,
                        Byte.toUnsignedInt(buf[1]));
                break;
            case RESOLUTION:
                hopeEquals("MCP9808 Reg " + Register.RESOLUTION + " (MSB)", 0x0,
                        Byte.toUnsignedInt(buf[0]));
                break;
            default:
                fail("Unexpected register identifier " + r);
            }
        }
        status = device.close(handle);
        assertTrue("Close error.", (status>=0));
    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808ControlMethod() throws IOException, ReflectiveOperationException {
        byte[] statusA = new byte[2];
        byte[] statusB = new byte[2];
        byte[] statusC = new byte[2];
        byte[] control = new byte[2];

        int status = device.open("MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));

        int handle = status;

        for (Register r : Register.values()) {
            switch (r) {
            case CONFIG:
                status = device.status(handle, Register.CONFIG.bits(), 2, statusA);
                assertEquals("Status error:", 2, status);

                hopeEquals("CONFIG statusA (MSB)", 0, Byte.toUnsignedInt(statusA[0]));
                hopeEquals("CONFIG statusA (LSB)", 0, Byte.toUnsignedInt(statusA[1]));

                control[0] = (byte) (MCP9808.ConfigMask.Hysteresis.bits() & 0xFF);
                control[1] = 0;
                status = device.control(handle, Register.CONFIG.bits(), 2, control);
                assertEquals("Control error:", 2, status);

                status = device.status(handle, Register.CONFIG.bits(), 2, statusB);
                assertEquals("Status error:", 2, status);

                hopeEquals("CONFIG statusB (MSB)", 0x6, Byte.toUnsignedInt(statusB[0]));
                hopeEquals("CONFIG statusB (LSB)", 0, Byte.toUnsignedInt(statusB[1]));

                control[0] = 0;
                control[1] = 0;
                status = device.control(handle, Register.CONFIG.bits(), 2, control);
                assertEquals("Control error:", 2, status);

                status = device.status(handle, Register.CONFIG.bits(), 2, statusC);
                assertEquals("Status error:", 2, status);

                hopeEquals("CONFIG statusC (MSB)", 0, statusC[0]);
                hopeEquals("CONFIG statusC (LSB)", 0, statusC[1]);
                break;
            case RESOLUTION:
                status = device.status(handle, Register.RESOLUTION.bits(), 1, statusA);
                assertEquals("Status error:", 1, status);

                hopeEquals("RESOLUTION statusA (LSB)", 0, Byte.toUnsignedInt(statusA[0]));

                control[0] = (byte) (0x3);
                status = device.control(handle, Register.RESOLUTION.bits(), 1, control);
                assertEquals("Control error:", 1, status);

                status = device.status(handle, Register.RESOLUTION.bits(), 1, statusB);
                assertEquals("Status error:", 1, status);

                hopeEquals("RESOLUTION statusB (LSB)", 3, Byte.toUnsignedInt(statusB[0]));

                control[0] = (byte) (0);
                status = device.control(handle, Register.RESOLUTION.bits(), 1, control);
                assertEquals("Control error:", 1, status);

                status = device.status(handle, Register.RESOLUTION.bits(), 1, statusC);
                assertEquals("Status error:", 1, status);
                hopeEquals("RESOLUTION statusC (LSB)", 0, statusC[0]);
                break;

            default:
                break;
            }
        }
        status = device.close(handle);
        assertTrue("Close error.", (status>=0));
    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808LibReadTempC() throws IOException, ReflectiveOperationException {
        int status = device.open("MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));
        int handle = status;
        double tC = device.readTempC(handle);
        logger.info(String.format("Temperature: %3.1f° C, %3.1f° F", tC, (tC * 1.8) + 32));
        status = device.close(handle);
        assertTrue("Close error.", (status>=0));

    }

    // --------------------------------------------------------

    @Test
    public void testMCP9808LibPower() throws IOException, ReflectiveOperationException {
        int status = device.open("MCP9808:0",
                DeviceDriver.OpenFlag.DDO_FORCE_OPEN.bits());
        assertTrue("Open error.", (status>=0));
        int handle = status;

        assertTrue("Full power. ", device.isFullPower(handle));

        device.setFullPower(handle, false);
        assertFalse("Full power. ", device.isFullPower(handle));

        device.setFullPower(handle, true);
        assertTrue("Full power. ", device.isFullPower(handle));

        status = device.close(handle);
        assertTrue("Close error.", (status>=0));

    }

}
