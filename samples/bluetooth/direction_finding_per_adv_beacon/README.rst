.. _bluetooth_direction_finding_per_adv_beacon:

Bluetooth: Direction Finding Periodic Advertising Beacon
########################################################

Overview
********

A simple application demonstrating the BLE Direction Finding CTE Broadcaster functionality by sending Constant Tone Extension with periodic advertising PDUs.


Requirements
************

* nRF52833DK board with nRF52833 SOC
* For AoD antenna matrix is needed:
* Nordic Semiconductor design 12 patch antenna matrix (optional) or
* other antenna matrix (optional)

Building and Running
********************

This sample can be found under :zephyr_file:`samples/bluetooth/df_cl_beacon` in the Zephyr tree.

There are two build configuration provided with this sample:
* df_aoa.config - enables Angle of Arrival mode
* df_aod.config - enables Angle of Departure mode (this configuration requires antenna matrix)

By default during build process df_aoa.conf configuration is used.

To use Angle of Departure mode, build this application as follows:

.. zephyr-app-commands::
   :zephyr-app: samples/bluetooth/df_cl_beacon
   :host-os: unix
   :board: nrf52833dk_nrf52833
   :gen-args: DF_MODE=AOD
   :goals: run
   :compact:

See :ref:`bluetooth samples section <bluetooth-samples>` for common information about bluetooth samples.

Antenna matrix configuration
****************************

To use this sample with Angle of Departure enabled, additional GPIOS configuration is required to enable control of the antenna array.
Example of such configuration is provided in boards/nrf52833dk_nrf52833.overlay.

nrf52833dk_nrf52833.overlay is a device tree overlay that provides information about which GPIOS should be used by Radio peripheral to switch between antenna patches during CTE transmission in AoD mode.
At least two GPIOs must be provided to enable antenna switching.
GPIOS are used by Radio peripheral in order following indices of :code:`dfegpio#-gpios` properties.
The order is important because it affects mapping of antenna switch patterns to GPIOS (see `Antenna patterns`_).

Antenna patterns
****************
The antenna switch pattern is a binary number where each bit is applied to a particular antenna GPIO pin.
For example, the pattern 0x3 means that antenna GPIOs at index 0,1 will be set, and following are left unset.
PaKa: this doesn't seem right to me. For pattern 0x03, GPIOs at 0 and 1 should be set, all others should be disabled. Or am I mistaken? You need to be able to control each pin individually. This example uses a 4 bit control but the text should maybe be made generic'ish so maybe use the rest inside of counting to 4?

This also means that, for example, when using four GPIOs, the patterns count cannot be greater than 16 and maximum allowed value is 15.

If the number of switch-sample periods is greater than the number of stored switch patterns, then the radio loops back first pattern.

The following table presents the patterns that you can use to switch antennas on the Nordic-designed antenna matrix:

+--------+--------------+
|Antenna | PATTERN[3:0] |
+========+==============+
| ANT_12 |  0 (0b0000)  |
+--------+--------------+
| ANT_10 |  1 (0b0001)  |
+--------+--------------+
| ANT_11 |  2 (0b0010)  |
+--------+--------------+
| RFU    |  3 (0b0011)  |
+--------+--------------+
| ANT_3  |  4 (0b0100)  |
+--------+--------------+
| ANT_1  |  5 (0b0101)  |
+--------+--------------+
| ANT_2  |  6 (0b0110)  |
+--------+--------------+
| RFU    |  7 (0b0111)  |
+--------+--------------+
| ANT_6  |  8 (0b1000)  |
+--------+--------------+
| ANT_4  |  9 (0b1001)  |
+--------+--------------+
| ANT_5  | 10 (0b1010)  |
+--------+--------------+
| RFU    | 11 (0b1011)  |
+--------+--------------+
| ANT_9  | 12 (0b1100)  |
+--------+--------------+
| ANT_7  | 13 (0b1101)  |
+--------+--------------+
| ANT_8  | 14 (0b1110)  |
+--------+--------------+
| RFU    | 15 (0b1111)  |
+--------+--------------+

To use antenna matrix provided by other manufacturer follow this steps:
* provide GPIO mapping in nrf52833dk_nrf52833.overlay file
* update antenna patterns in :cpp:var:`ant_patterns` array.
