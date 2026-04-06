# Tests

## v0.1

Manual procedures only; follow [docs/test-plan.md](../docs/test-plan.md) (**BOOT**, **CLI**, **stub `TLM`** at ~**10 Hz** in **RUN**). No committed automated harness.

## Host-side (optional, later)

Compile small pure-C helpers (parsers, format checks) on the PC for fast iteration. Keep them under a subfolder such as **`tests/host/`** when you add them—not required for v0.1.

## Target-side (optional, later)

On-device checks (manual or a lightweight framework) still map to [docs/test-plan.md](../docs/test-plan.md). Place extras under **`tests/target/`** if needed.

## Policy

Do not commit large UART captures; use **`logs/`** (gitignored).
