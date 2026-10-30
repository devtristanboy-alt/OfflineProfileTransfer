# Backup format v1

A backup directory contains:

```text
manifest.json
backup.log
savedata/
trophies/
av_contents/
user_selected/
```

`manifest.json` stores:
- format and format version
- app version
- source console label
- firmware label (not automatically detected in v0.1)
- user ID label (not automatically detected in v0.1)
- UTC timestamp
- each copied file's category, relative path, byte size and SHA-256

Paths are relative to the backup root. The engine rejects absolute paths and `..` components. Symbolic links are skipped.

The v0.1 verifier supports manifests emitted by this program. Before parsing manifests from arbitrary third parties, replace the minimal parser with a maintained JSON library and add fuzz tests.

No account secrets, authentication tokens, NPUSER files or account activation files belong in this format.
