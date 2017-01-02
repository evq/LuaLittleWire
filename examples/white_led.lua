local littlewire = require "littlewire"
local LED_PIN = PIN3

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect()

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

littlewire.softPWM_state(lw_dev, ENABLE)
--littlewire.softPWM_write(lw_dev, 205, 255, 95)
littlewire.softPWM_write(lw_dev, 0, 0, 0)

--littlewire.pinMode(lw_dev, 0, OUTPUT)
--littlewire.digitalWrite(lw_dev, 0, LOW)

--littlewire.pinMode(lw_dev, 1, OUTPUT)
--littlewire.digitalWrite(lw_dev, 1, LOW)

--littlewire.pinMode(lw_dev, 2, OUTPUT)
--littlewire.digitalWrite(lw_dev, 2, LOW)

littlewire.pinMode(lw_dev, LED_PIN, OUTPUT)
--littlewire.digitalWrite(lw_dev, LED_PIN, HIGH)
littlewire.digitalWrite(lw_dev, LED_PIN, LOW)
