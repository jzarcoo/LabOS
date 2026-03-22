# Practice 4

1. Install a serial terminal emulator (`screen`).
```sh
sudo dnf install screen
```

2. Add your user to the serial access group
 ```sh
sudo usermod -aG dialout $USER
newgrp dialout
```

3. Connect to the Raspberry Pi Pico
```sh
sudo screen /dev/ttyACM0 115200
```


4. Interact with the system:  Press keys 1, 2, 3, or 4 to enqueue tasks

5. To exit the terminal press `Ctrl + A`, then `K`.