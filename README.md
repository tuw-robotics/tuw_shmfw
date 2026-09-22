# tuw_shmfw

A very small C++ library for creating shared memory variables that let a
running program's internal state be inspected and changed from the outside, without stopping or recompiling it.

Wrap a variable or `std::vector` in a `ShmFw::Var<T>` / `ShmFw::Vector<T>`, and it becomes visible in a shared memory segment under a name of your choosing. Any other process — including the bundled `shmfw_admin` and `shmfw_editor` tools — can then list, read, and modify that value at runtime.

## Features

- Header-only style wrappers (`ShmFw::Var<T>`, `ShmFw::Vector<T>`) around
  values living in a Boost.Interprocess managed shared memory segment
- Automatic timestamping and optional locking per variable
- Type introspection (type name/hash, container kind) so generic tools can
  read and print variables without knowing their C++ type at compile time
- `shmfw_admin` — lists all variables in a shared memory segment, with their
  type, lock state, timestamp, and value
- `shmfw_editor` — an ncurses TUI for live-viewing and editing variables in a
  running segment
- `std::chrono::system_clock::time_point` supported directly as a variable
  type

## Requirements

- CMake >= 3.10
- A C++17 compiler
- Boost (`date_time`, `thread`, `program_options`)
- ncurses (only needed for `shmfw_editor`)

On Ubuntu, install everything needed with:

```bash
sudo apt install build-essential cmake libboost-date-time-dev libboost-thread-dev libboost-program-options-dev libncurses-dev
```

## Building

```bash
mkdir build && cd build
cmake ..
make
```

This builds the `tuw_shmfw` library, the `shmfw_admin` and `shmfw_editor`
apps, and the example programs.

## Usage

### Writing a shared variable

See [`common/examples/usage_var.cpp`](common/examples/usage_var.cpp):

```cpp
#include <tuw_shmfw/variable.hpp>

ShmFw::HandlerPtr shmHdl = ShmFw::Handler::create("shared_memory_name", 65536);

ShmFw::Var<double> a("a", shmHdl);
a.set(5.4);
std::cout << a << std::endl;
*a.get() = 1.2;
```

### Writing a shared vector

See [`common/examples/usage_vector.cpp`](common/examples/usage_vector.cpp):

```cpp
#include <tuw_shmfw/vector.hpp>

ShmFw::HandlerPtr shmHdl = ShmFw::Handler::create("shared_memory_name", 65536);

ShmFw::Vector<double> a("vector_a", shmHdl);
a.clear();
a->push_back(1.23);
```

Run the examples with `shmfw_usage_var` / `shmfw_usage_vector` (built to
`build/common/examples/`).

### Inspecting a segment: shmfw_admin

```bash
shmfw_admin -m shared_memory_name --context --type --timestamp
```

Lists every variable in the segment along with its lock state and, with the
flags above, its value, type, and last-update time. Pass `-c` / `--clear` to
remove the segment entirely.

### Editing variables live: shmfw_editor

```bash
shmfw_editor -m shared_memory_name
```

Opens an ncurses view of the segment's variables (or a specific subset given
via `-n`). Navigate with the arrow keys, press `Enter` to edit a value, `l`/`r`
to lock/unlock, `t` to trigger a change notification, and `h` for the full key
list.

## Project layout

```
common/
├── include/tuw_shmfw/   # public headers (Var, Vector, Handler, Header, ...)
├── src/                 # library implementation
├── apps/
│   ├── admin/           # shmfw_admin CLI
│   └── editor/          # shmfw_editor ncurses TUI
└── examples/            # shmfw_usage_var, shmfw_usage_vector, ...
```

## License

BSD 3-Clause, see [LICENSE](LICENSE).
