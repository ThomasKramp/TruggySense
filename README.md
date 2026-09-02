# TruggySense

This repository contains two generations of the project:

| Folder | Description |
|---|---|
| [`TruggySense_v1`](./TruggySense_v1) | Original system, developed by **[Robbe Elsermans](https://github.com/RobbeElsermans)**. Companion paper: [arXiv:2505.07399](https://arxiv.org/abs/2505.07399) |
| [`TruggySense_v2`](./TruggySense_v2) | Hardware and firmware redesign for reproducibility, developed by **Thomas Kramp**, building on v1 |

Each subfolder has its own README with details on `cad/`, `code/`, and `docs/`.

---

## License

Unless stated otherwise, **original content created for this project** — documentation,
CAD/schematic designs, and project-authored source code — is licensed under
**[CC BY-SA 4.0 (Attribution-ShareAlike 4.0 International)](https://creativecommons.org/licenses/by-sa/4.0/)**.
The full legal text is in [`licence.txt`](./TruggySense_v1/licence.txt) (identical copy also in `TruggySense_v2/`).

In short, if you reuse or build on this work you must:
- **Attribute** the original creator(s) (Robbe Elsermans for v1, Thomas Kramp for v2) and link back to this repository,
- **Indicate any changes** you made,
- **Share any adapted/derivative version under the same license** (CC BY-SA 4.0 or a compatible license).

### What this license does *not* cover

CC BY-SA is intended for creative/documentation works, not for reused third-party software or hardware libraries. The following are **excluded** from the CC BY-SA grant above and remain under their own original licenses/terms:

- **Third-party code libraries** vendored under `code/**/lib/` (e.g. encoder libraries, `TimerInterrupt_Generic`, and similar dependencies pulled in via PlatformIO or copied manually). Check each library's own header/license file or upstream repository before reuse.
- **Vendor-exported component/footprint libraries** under `cad/KiCAD/**` (e.g. `Capture/`, `Quadcept/`, manufacturer `.zip` libraries). These originate from component manufacturers or distributors and are subject to their own terms.
- **Any datasheet, manual, or reference material** included for convenience (e.g. under `docs/`), which remains the property of its original publisher.
- **Trademarks and product names** referenced in this project — including *Team Corally Kagama*, *Teensy*, and other component/brand names — belong to their respective owners. Their use here is purely descriptive (identifying compatible hardware) and does **not** imply affiliation with, sponsorship by, or endorsement from those companies.

If you plan to redistribute or build a product on top of this repository, you are responsible for independently verifying the license terms of any third-party component you rely on.

### Hardware & safety disclaimer

This project involves custom electronics (battery connections, power protection, motor control) built for an RC vehicle platform. It is shared **as-is, for research and educational purposes**:

- No warranty is made regarding the safety, reliability, or fitness of any design for a particular purpose — see the full disclaimer in Section 5 of [`licence.txt`](./TruggySense_v1/licence.txt).
- Building or operating hardware based on this repository (batteries, power electronics, motorized vehicles) carries inherent risk. Do so at your own risk and with appropriate care.
- The authors and Cosys-Lab / University of Antwerp accept no liability for damages, injury, or losses arising from use of this material.

### Note on university/thesis materials

This repository is the output of Master's thesis work supervised by Cosys-Lab. Any university-internal documents (guidelines, templates) are included for context only and remain the property of the University of Antwerp; they are not licensed for reuse under the terms above.

---

## Questions about reuse or licensing

Open an issue on this repository, or reach out via Cosys-Lab, University of Antwerp.
