# Changelog

All notable changes to kingOS will be documented in this file.

## Unreleased

### Added

- Added GitHub Actions CI to build and validate the ISO on pushes and pull requests.
- Added release automation that creates missing `v*` tag releases and attaches `kingos.iso`.
- Added manual CI runs through GitHub Actions workflow dispatch.
- Documented the LLVM-backed CI build path.

### Changed

- Made linker libraries configurable through `LDLIBS` so CI can build with Clang/LLD while the default GCC cross-toolchain still links with `-lgcc`.
- Updated the linker script section alignment syntax to work with both GNU ld and LLD.
- Made the kernel build explicitly non-PIC and non-PIE.

## v0.1.0 - 2026-07-22

### Added

- Added colored kernel status output and a small boot banner.
- Added terminal support for line wrapping, scrolling, carriage returns, tabs, and backspace.
- Added decimal number output for simple kernel diagnostics.
- Added `make check` to validate that the kernel image is Multiboot-compatible when `grub-file` is installed.
- Added this changelog.

### Changed

- Updated the default `make` target to build the ISO without launching QEMU.
- Added explicit Makefile dependencies for source, linker, and GRUB configuration files.
- Updated README with build requirements, usage, and project structure.

### Fixed

- Fixed VGA terminal scrolling, which previously used transposed row and column indexes.
- Fixed terminal row bounds checks that compared rows against screen width.
- Fixed line wrapping so output scrolls instead of jumping back to the top of the screen.
- Fixed several comments and documentation typos.
