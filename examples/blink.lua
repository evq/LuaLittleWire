local littlewire = require "littlewire"
local LED_PIN = PIN1

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect()

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

littlewire.pinMode(lw_dev, LED_PIN, OUTPUT)

while true do
  littlewire.digitalWrite(lw_dev, LED_PIN, LOW)
  print("LED OFF")
  littlewire.delay(500);
  littlewire.digitalWrite(lw_dev, LED_PIN, HIGH)
  print("LED ON")
  littlewire.delay(500);
end
