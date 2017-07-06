local littlewire = require "littlewire"
local LED_PIN = PIN1

local littlewire_devices = littlewire.search()

print("Found ", #littlewire_devices, " littlewire devices.\n")

local lw_dev = littlewire.connect()

print("Connected to first device.")
print("FW Version: ", littlewire.readFirmwareVersion(lw_dev), "\n")

littlewire.quadPWM_state(lw_dev, 1)

delayTime = 25

while true do
  redVal = 255
  blueVal = 0
  greenVal = 0
  i = 0
  while i < 255 do
      greenVal = greenVal + 1
      redVal = redVal - 1
      littlewire.quadPWM_write(lw_dev, 255 - redVal, 255 - greenVal, 0, 0)
      littlewire.delay(delayTime)
      i = i + 1
  end
    
  redVal = 0
  blueVal = 255
  greenVal = 0
  i = 0
  while i < 255 do
      redVal = redVal + 1
      blueVal = blueVal - 1
      littlewire.quadPWM_write(lw_dev, 255-redVal, 0, 255 - blueVal, 0)
      littlewire.delay(delayTime)
      i = i + 1
  end  

  redVal = 0
  blueVal = 0
  greenVal = 255
  i = 0
  while i < 255 do
      blueVal = blueVal + 1
      greenVal = greenVal - 1
      littlewire.quadPWM_write(lw_dev, 0, 255 - greenVal, 255 - blueVal, 0)
      littlewire.delay(delayTime)
      i = i + 1
  end
end
