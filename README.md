# QMK userspace for Keebio Nyquist LM

## One-time setup

Clone `qmk_firmware` and this repo as siblings:

```bash
git clone https://github.com/qmk/qmk_firmware.git
cd qmk_firmware && git submodule update --init --recursive && cd ..
git clone git@github.com:zakj/qmk_userspace.git
```

## Build

Start a container with both trees mounted:

```bash
docker run --rm -it \
  -v "$PWD/qmk_firmware":/qmk_firmware \
  -v "$PWD/qmk_userspace":/qmk_userspace \
  -e QMK_USERSPACE=/qmk_userspace \
  -w /qmk_firmware \
  ghcr.io/qmk/qmk_cli bash
```

Then, inside:

```bash
qmk config user.qmk_home=/qmk_firmware
qmk config user.overlay_dir=/qmk_userspace
qmk compile -kb keebio/nyquist_lm/rev1 -km zakj
```

## Formatting

Grid alignment via [qmk-layout-fmt](https://github.com/OneDeadKey/qmk-layout-fmt):

```bash
qmk-layout-fmt keyboards/keebio/nyquist_lm/keymaps/zakj/keymap.c --uniform
```

Note that it leaves some invalid trailing commas.
