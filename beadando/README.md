# A Compton-szórást implementáltam

## Példa:

Egy vas csövön történő (vissza szórás) energia spektrumát próbáltam reprodukálni.

A referencia a spectrum_of_pipe.jpg (https://sci-hub.tw/10.1007/s10967-015-4579-2), a saját eredményem a my_spectrum_of_pipe.pdf

A görbék nem teljesen egyeznek, mivel a fotoeffektussal nem foglalkoztam, és vasban már kb. 200 keV alatt nem elhanyagolható.
(Ez a iron_cross_section.png ábrán látható.) (https://i.stack.imgur.com/NFbG6.png, https://astronomy.stackexchange.com/questions/19492/compton-scattering-of-high-energy-radiation)

Illetve a kollimátorban, és a detektor burkolatában történő szóródással sem foglalkoztam.
(És a levegőn történő szóródást is elhanyagoltam, bár az valószínűleg tényleg elhanyagolható.)

## Nem sikerült:

Egy szilícium (és germánium) rétegben az egyszeres és kétszeres szórások arányának a meghatározása. (https://sci-hub.tw/https://www.sciencedirect.com/science/article/pii/016890029090832Q)

Erről nem csináltam külön ábrát, a kódot változtatva lehet nézegetni adott energiánál és vastagságnál.

Amit nem egészen értek, hogy mért nagyon más eredmények jönnek ki, pedig 500 keV lényegében csak Compton effektus van a szilíciumban.
(silicon_cross_section.jpg - https://www.researchgate.net/publication/30861511_Statistical_Performance_Evaluation_System_Modeling_Distributed_Computation_and_Signal_Pattern_Matching_for_a_Compton_Medical_Imaging_System)

### Egyéb fájlok: 
resolution_of_scintillator.jpg - https://inis.iaea.org/collection/NCLCollectionStore/_Public/32/042/32042470.pdf
