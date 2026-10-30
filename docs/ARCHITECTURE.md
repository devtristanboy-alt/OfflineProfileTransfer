# Architecture

## Layers

1. **Presentation**: `GuiModel` stores screen, status, progress and log messages. A console-specific renderer is not yet supplied.
2. **Shared engine**: copies selected directories, writes a manifest, hashes files and verifies/restores them.
3. **Platform adapter**: `PlatformAdapter` defines user and removable-storage discovery. `HostPlatformAdapter` intentionally reports these operations as unsupported.
4. **Console integration**: separate PS4/PS5 targets must implement their own entrypoint, filesystem policy, UI renderer and package metadata.

## Why separate console builds?

PS4 Orbis and PS5 Prospero are different target environments. A shared C++ core does not make binaries or package metadata interchangeable. Each platform requires its own supported SDK/toolchain, imports, entrypoint, graphics/input backend and packaging configuration.

## GUI roadmap

The state model is implemented, but there is no claim of a complete native console GUI yet. Implement a renderer that maps `Screen` to:
- Home: Backup / Restore / Verify / Settings / About
- Backup: source selection, category toggles, destination selection
- Restore: backup selection, integrity check, destination and overwrite confirmation
- Verify: manifest and hash report
- Settings: language, theme, log verbosity
- About: version, licenses and credits

Use confirmation dialogs before destructive actions and show an indeterminate state when total size cannot be determined.
