# ElessarOS

risc-v OS inspired by xv6. Initially this will be pretty much me building xv6 bit by bit, but I do hope to eventually diverge and implement some more advanced OS features like users, permissions, etc.

## Docker toolchain

Installing the toolchain

```bash
docker pull ghcr.io/francisrstokes/rv-toolchain-docker:main
```

## Building the kernel

```bash
docker run --rm -v $(pwd):/project -w /project -it francisrstokes/rv-toolchain:latest make
```

## Debugging

Build the kernel, then run

```bash
make qemu-gdb
```

(note: QEMU is not part of the riscv toolchain and needs to be separately installed)

In another console, run

```bash
docker run --rm -v $(pwd):/project --net="host" -w /project -it francisrstokes/rv-toolchain:latest riscv64-unknown-linux-gnu-gdb -ix .gdbinit
```

The program should now be debuggable inside GDB.

### VSCode debugging

A gdb-multiarch configuration exists inside `.vscode/launch.json`. If the port assigned by make differs from `26000`, you'll have to manually change it. Otherwise, debugging inside VSCode should also work just fine.