local littlewire = require "littlewire"
local FOWARD_PIN = PIN1
local BACKWARD_PIN = PIN2

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect_bySerialNum(100)

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

littlewire.pinMode(lw_dev, FOWARD_PIN, OUTPUT)
littlewire.pinMode(lw_dev, BACKWARD_PIN, OUTPUT)

while true do
  littlewire.digitalWrite(lw_dev, FOWARD_PIN, LOW)
  littlewire.digitalWrite(lw_dev, BACKWARD_PIN, LOW)
  print("MOTOR OFF")
  littlewire.delay(1000);
  littlewire.digitalWrite(lw_dev, FOWARD_PIN, HIGH)
  print("MOTOR FORWARD")
  littlewire.delay(1000);
  littlewire.digitalWrite(lw_dev, FOWARD_PIN, LOW)
  print("MOTOR OFF")
  littlewire.delay(1000);
  littlewire.digitalWrite(lw_dev, BACKWARD_PIN, HIGH)
  print("MOTOR BACKWARD")
  littlewire.delay(1000);
end
