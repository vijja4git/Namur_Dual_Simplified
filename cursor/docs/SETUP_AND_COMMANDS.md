# Setup and Commands (MS51 repo)

## Required tools
- SDCC
- packihx
- make
- clang/gcc (for host tests)
- Optional: Nu-Link tooling for flashing

## macOS setup
```bash
xcode-select --install
brew update
brew install sdcc make
sdcc --version
packihx --help