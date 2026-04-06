# Practice 5

1. Install a serial terminal emulator (`screen`).
```sh
sudo dnf install screen
```

2. Add your user to the serial access group
 ```sh
sudo usermod -aG dialout $USER
newgrp dialout
```

3. Identify the correct serial device.
```sh
ls -l /dev/ttyACM*
```

4. Connect to the Raspberry Pi Pico (replace `/dev/ttyACM0` if needed)
```sh
sudo screen /dev/ttyACM0 115200
```

1. Interact with the system: press keys 1, 2, 3, 4 or 5 to enqueue tasks

2. To exit the terminal press `Ctrl + A`, then `K`.