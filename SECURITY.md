# Security Policy

VargaMesh Core is consensus-critical network software.

## Supported versions

Until a longer-term support schedule is published, security fixes target the
current supported VargaMesh Core release.

Users should verify release artifacts and run supported software.

## Reporting a vulnerability

Do not publish exploitable vulnerabilities in a normal public GitHub issue.

Use the private security-reporting facilities of the VargaMesh GitHub
repository when available, or use the project contact information published
at:

https://mesh.vargatech.net/

A useful report should include:

- affected release or commit
- affected component
- reproduction steps
- expected security impact
- whether exploitation has been observed
- suggested mitigation, if known

Please avoid public disclosure of a working exploit before maintainers have
had a reasonable opportunity to investigate and coordinate remediation.

## Sensitive runtime data

The public source repository must never contain production:

- private keys
- seed phrases or mnemonics
- wallet files
- RPC passwords
- RPC authentication secrets
- `.cookie` files
- SSH private keys
- production `.env` files
- server credentials

A public VargaMesh node does not require publication of any private key.
