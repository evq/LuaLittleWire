local littlewire = require "littlewire"

print("Dumping littlewire:")
for k, _ in pairs(littlewire) do
  print(k)
end
print()

local littlewire_devices = littlewire.search()
print(#littlewire_devices, " Device(s): ")
for i=1, #littlewire_devices, 1 do
  v = littlewire_devices[i]
  lw_dev = littlewire.connect_bySerialNum(v)
  print("Dev: ", i, " SN: ", v, " FW: ", littlewire.readFirmwareVersion(lw_dev))
  print()
end
