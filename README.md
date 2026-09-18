# Qt calculator · C++17 desktop application

An educational desktop calculator built with Qt Widgets and C++17. The application demonstrates signal/slot-driven arithmetic, keyboard input, memory, selected scientific functions, and limited power-rule calculus. It is not a general symbolic mathematics engine or a high-precision scientific calculator.

## Build and run

Install a C++17 compiler, CMake 3.16+, and Qt Widgets development files (Qt 5.15+ or Qt 6). For example, with Qt 5 on Debian/Ubuntu:

```bash
sudo apt-get install cmake g++ qtbase5-dev
git clone https://github.com/omarsaqr12/calculator.git
cd calculator
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/QtAdvancedCalculator
```

On Windows and macOS, use the CMake generator and executable path associated with your Qt kit. The original [`calculator.pro`](calculator.pro) is retained for qmake. Only Ubuntu/Qt 5 builds are checked in CI; cross-platform release binaries have not been certified.

## Using the calculator

The UI provides digits `0–9`, four basic operators, decimal, `=`, backspace, clear entry (`CE`), and clear all (`AC`). Keyboard digits, `+`, `-`, `*`, `/`, `.`, Enter/`=`, Backspace, Delete (`CE`), and Escape (`AC`) are routed through their corresponding buttons. The display is read-only. Memory buttons (`MS`, `MR`, `MC`, `M+`, `M-`) store, recall, clear, add and subtract a value.

Scientific controls implement square root, percent (`x / 100`), factorial for nonnegative integer arguments through 170, sine/cosine/tangent in **degrees**, base-10 and natural logarithms, and `e^x`. Floating-point precision, overflow, and domain restrictions apply. The incomplete `x^y` handler is deliberately **not exposed**. An in-memory history is retained for debug logging, but there is no visible history browser.

The `d/dx` and `integrate` buttons demonstrate only powers: enter an evaluation point or upper bound `x`, press the operation, enter exponent `n`, then `=`. The first computes the derivative of `x^n` at `x`; the second uses `x^(n+1)/(n+1)` as the integral of `t^n` from 0 to `x` where defined. The implementation rejects `n = -1` and does not parse an arbitrary mathematical expression. Other domains and boundary cases have not all been validated.

## Architecture and verification

- [`src/calculator.ui`](src/calculator.ui) defines the original window and core controls; [`src/calculator.h`](src/calculator.h) and [`src/calculator.cpp`](src/calculator.cpp) contain the original arithmetic, memory state, and numerical helpers.
- [`src/calculator_app.h`](src/calculator_app.h) adds missing controls and a keyboard-to-button event filter shared by [`src/main.cpp`](src/main.cpp) and the expanded GUI test.
- [`tests/calculator_smoke.cpp`](tests/calculator_smoke.cpp) asserts `1 + 2 = 3`, then `3 × 2 = 6` using original buttons.
- [`tests/calculator_controls.cpp`](tests/calculator_controls.cpp) exercises the restored controls, button placement, memory, representative scientific functions, two power-rule examples, and actual keyboard events.
- [GitHub Actions](.github/workflows/ci.yml) builds the program and runs both tests under Qt 5's offscreen platform. Locally, run `ctest --test-dir build --output-on-failure` after building.

## Known limitations and license

The original calculation state is stored in globals, so multiple independent calculator windows could interfere. The original `calculator::keyPressEvent` retains its sender-dependent path; the application-level event filter handles supported keys for the normal executable, but the original class itself is not completely refactored. Repeated equals, negative-number entry, operator chaining, all scientific/calculus edge cases, and platforms beyond the tested Ubuntu Qt 5 environment require further regression testing. No screenshots or release binaries have been verified, so none are represented as published artifacts.

The original [MIT license](LICENSE) is unchanged.
