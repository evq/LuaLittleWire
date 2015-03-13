local littlewire = require "littlewire"
local LED_PIN = PIN1

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect()

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

while true do
  --humid, temp = littlewire.dht_read(lw_dev, DHT11)
  humid, temp = littlewire.dht_read(lw_dev, DHT22)
  print("H: " .. humid/10.0 .. " T: " .. temp/10.0)
  littlewire.delay(5000);
end
