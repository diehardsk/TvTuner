# WBFM audio from TV Tuner
Multistandard LG TV tuner was removed from an old PCI tuner card (SKYTV HD6PCI).
Inside of it there's TUA6039 MIXER and TDA9885 DEMODULATOR which we can control by I2C from ATMEGA8L microcontroller to receive demodulated audio. Example code selects LOW band and tunes to 93.3MHz WBFM analog radio station.
