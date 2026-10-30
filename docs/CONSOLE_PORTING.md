# Console porting checklist

## PS4 (Orbis)

Candidate toolchain: OpenOrbis PS4 Toolchain. Read its current upstream setup guide and sample projects before creating the target.
- [ ] Confirm target firmware and homebrew runtime compatibility.
- [ ] Add Orbis application entrypoint and lifecycle handling.
- [ ] Add native graphics/input renderer for `GuiModel`.
- [ ] Implement removable USB enumeration and mount handling.
- [ ] Implement user selection through supported homebrew/runtime interfaces.
- [ ] Restrict reads to explicitly selected and accessible paths.
- [ ] Add Orbis package metadata and build pipeline.
- [ ] Test on a non-critical user and disposable data first.

## PS5 (Prospero)

Candidate project: SharpProspero. Confirm current upstream compatibility matrix and application-vs-payload model.
- [ ] Confirm exact firmware support; do not infer that a jailbreak for a firmware means every SDK sample works there.
- [ ] Add Prospero application entrypoint and lifecycle.
- [ ] Add native UI renderer and input.
- [ ] Implement USB enumeration/mount handling.
- [ ] Implement supported user selection.
- [ ] Implement safe restore to user-selected, authorized destinations.
- [ ] Add package metadata and build pipeline.
- [ ] Test with disposable data and official backup available.

## Deliberately excluded

- Account credential cloning or activation bypass.
- NPUSER patching to impersonate another account.
- Rebuilding/injecting internal trophy databases.
- Blind overwrites of protected system directories.
