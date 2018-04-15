from evdev import InputDevice, list_devices

devices = [InputDevice(fn) for fn in list_devices()]
for device in devices:
    print(device.fn, device.name, device.phys)
