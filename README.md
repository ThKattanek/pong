# Kleines Pong Spiel #


### Einleitung ###

Jeder Spieler hat einen Schläger den er per Tastatur nach oben und nach unten bewegen kann. Wie beim Tennis
wird ein Ball hin und her geschlagen. Kann der Ball mit dem Schläger nicht getroffen werden so bekommt der
Gegner eine Punkt. Wer zuerst 9 Punkte erreicht, hat gewonnen.

Tasten:

Spieler 1:

Hoch   - Q

Runter - A

Spieler 2:

Hoch   - CURSOR HOCH

Runter - CURSOR HOCH

Ball Start: Space
Neustart: R

Beenden: ESCAPE

### Version: 1.0.0   //   Lizenz: GPL Version 2 (http://www.gnu.org/licenses/gpl-2.0.html) ###
### Screenshot ###
![078.png](https://bitbucket.org/repo/XoRb7M/images/3447139878-078.png)
### Voraussetzung ###

* installiertes git
* installiertes qmake
* sfml lib

### Bauen der Anwendung unter Linux ###

#### Quellcode Clonen: ####
    >> git clone https://tkattanek@bitbucket.org/tkattanek/pong.git
#### Anwendung bauen: ####
    >> qmake
    >> make

### Bauen der Anwendung unter Windows (MinGW32) ###

    >> qmake
    >> make
    >> make release
    >> make release-install

#### !! Anstatt make könnte auch mingw32-make benutzt werden. ####