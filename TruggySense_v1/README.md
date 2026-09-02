# TruggySense

The foundation for a new off-road era of (autonomous) RC-vehicles!

Developed by Robbe Elsermans through Cosys-Lab at University of Antwerp.

Welcome to the readme! A list of what this repo includes

- all files related to the end product,
- source code,
- manuals of the final system,
- a future perspective (inside the paper).

## cad
Everything related to specialized mounts, holders, and cases to build upon the Team Corally Kagama 1/8 RC vehicle chassis. 
Furthermore, it contains also schematics and circuits drawn in KiCAD.

## code
Build in Platform IO (extension of VS-Code)

- **LowLevelController** containing the code for the LLC developed for a Teensy4.1 MCU,
- **base_station** contains code for the base station telemetry receiver,
- **matlab** holds some scripts to review the converted offline data,
- **nodered** describes the used flow and needed pallets to set things up locally.
- **acquired_data** has captured data of several runs during experiments. Both the binary as the converted JSON are present there.
- **examples** contains sketches used to test modules and sensors out during development. They served as a starting point to develop the drivers so don't expect them to be fully updated!

## docs
- the portfolio of the whole project with a nice *How To* guide at the end,
- the paper, also available on arXiv: [https://arxiv.org/abs/2505.07399](https://arxiv.org/abs/2505.07399).

Note that inside **code/LowLevelController/docs**, a doxygen is present giving insights into the code.

## Future perspectives
Right now, the TruggySense framework is at the beginning of its era. A great number of work and tinkering is done already. Nevertheless, there is still a long path ahead where opportunities can emerge. Some future work listed below.

Future work for TruggySense
- **Modularity**
- **Reproducability**
- **Code Independency**

Future work that is build upon TruggySense
- **Advanced Driver Assistance System (ADAS)** -> TruggyAssist
- **Autonomous Agent** -> TruggyDrive

## licence
BY-SA 4.0 (Attribution-ShareAlike 4.0 International)
See licence.txt for the disclaimer
