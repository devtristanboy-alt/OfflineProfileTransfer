# Security model

- Source directories are explicitly supplied by the operator.
- Symbolic links are skipped during backup.
- Manifest paths must be relative and cannot contain `..`.
- Restore verifies hashes before copying.
- Restore does not overwrite by default.
- Use a separate destination and retain the original backup.
- Do not store passwords, tokens, private keys or account activation data.
- Do not treat a successful hash check as proof that a save is compatible with the target console/account.
- The console port must enforce user confirmation and restrict operations to authorized data.
