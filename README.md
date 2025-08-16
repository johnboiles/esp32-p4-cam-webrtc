# ESP32-P4 WebRTC Camera

Code to turn a ESP32-P4 with a MIPI camera into a network camera using WebRTC.

## Getting started (build with IDF.py)

After cloning this repo from GitHub you'll need to pull the most recent submodules. You'll need to rerun this command periodically to get the latest changes whenever the submodules change.

```bash
git submodule update --init --recursive
```

The primary way to build the firmware itself is with Espressif's official [esp-idf toolchain](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/linux-macos-setup.html). In some other directory (not in this repo), run:

```sh
mkdir ~/esp
cd ~/esp
git clone -b v5.5 --recursive https://github.com/espressif/esp-idf.git
cd esp-idf
./install.sh esp32s3
```

If you get errors about `cmake` you might need to install `cmake` first (`brew install cmake` on macOS / `sudo apt install cmake` on Ubuntu).

Now from the root of this repo, you can run (replace `/YOUR/PATH/TO` with the path where you put `esp-idf`):

```sh
. ~/esp/esp-idf/export.sh
idf.py build flash monitor
```

This will build the latest firmware, flash it to the device, and open a serial monitor. You can also use `idf.py menuconfig` to configure the build if you need to change build options.

See also the official [esp-idf installation instructions](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/get-started/index.html) for more details.
