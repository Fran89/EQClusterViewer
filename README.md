# EQClusterViewer
A companion app for EQCluster

### Brief:
EQCluster is designed to take output from [EQCluster](https://github.com/Fran89/EQCluster) and 
[AutoMISD](https://github.com/Fran89/AutoMISD) and graph and categorize the clusters into sequences
or swarms. Although this is done manually from visual inspection this software makes sorting big data
easier. It also has several output options that can create PDFs, PNGs, and CVSs with the separated data.
It also has a recombine where it takes the original catalog in Zmap format to see all earthquakes (+- 8hrs)
in the area in that time frame.

### Compiling:

Use either qmake or QTCreator to compile this software. Tested on the Latest QT5.

### Usage:

  0. IMPORTANT: Output from EQCluster must be edited: NO HEADERS, NO CENTERs (IDs = -1)
  1. Load the file using the menu.
  2. (Optional) Load the complete catalog in Zmap format.
  3. Use the Categorize buttons (Categorize all scrolls through ALL and cannot be cancelled) to categorize between Swarm and Sequence.
  4. Press 'Save ALL PDF' to save EVERYTHING (A typo I know).
  
### Dependencies

  * QTCreator or QMake (With QT5)

### Acknowledgments

  * Alberto Lopez and Elizabeth Vanacore, my advisors for inspiring me to do this.
  * The QTCommunity for their wonderful toolkit and libraries.
