# Bonsai-Modellcomputer
Der Bonsai-Modellcomputer wurde von Klaus Merkert für den Einsatz im Informatikunterricht entwickelt. Auf folgender Seite findet man unter anderem Schaltpläne und Platinenlayouts von Klaus Merkert zum Bau eines Bonsai-Modellcomputers:
http://www.hsg-kl.de/faecher/inf/material/bonsai/

Es gibt auch eine Simulation des Lerncomputers (https://bonsai.pinyto.de/), die im digitalen Informatik-Schulbuch http://inf-schule.de verwendet wird.

Viele Bauteile, die in den Plänen von Klaus Merkert verwendet werden, wurden abgekündigt und sind heue (Stand 2021) nicht mehr verfügbar.
Ziel dieses Projektes ist es deshalb, eine Version des Bonsai-Modellcomputer zu entwickeln, die möglichst leicht nachgebaut werden kann.

# Projekstatus
Der Prototyp funktioniert und wurde im Unterricht eingesetzt. In den Github-Issues sind einige kleinere Probleme beschrieben, die vor einem weiteren Nachbau behoben werden könnten.

![Prototyp des Bonsai-Modellcomputers](Prototyp.jpg)

# Änderungen gegenüber den Plänen von Klaus Merkert
- Die Namen und Reihenfolge der Steuersignale wurden an den Simulator angepasst (vgl. https://bonsai.pinyto.de/documentation/micro/)
- Alle Schaltpläne und Platinen wurden mit der Software KiCad gezeichnet (http://kicad.org).
- Für die Platinen wurden neue Layouts entwickelt, damit die Möglichkeiten eines doppelseitigen Layouts besser genutzt werden können.
- Die Steckverbinder zur Verbindung der Steuersignale sowie der Spannungsversorgung mit den Platinen wurden ersetzt durch Molex KK (oder kompatible Nachbauten).
- Alle Bauteile der Familie 74LS wurden durch Bauteile der Familien 74HC oder 74HCT ersetzt.
- Zur Nullentdeckung auf der Platine Befehle wurde eine Wired-And-Verknüpfung mit Open-Collector-Ausgängen verwendet. Diese Wired-And-Verknüpfung wurde durch Gatter in einem 74HC08 ersetzt.
- Zur Erzeugung eines automatischen Taktsignals wird ein 74HC4046 verwendet.