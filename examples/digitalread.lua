local littlewire = require "littlewire"
local LED_PIN = PIN2

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect()

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

littlewire.pinMode(lw_dev, LED_PIN, INPUT)

while true do
  print("Pin value is: ", littlewire.digitalRead(lw_dev, LED_PIN))
  littlewire.delay(1000);
end
