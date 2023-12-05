======= INTRODUCTION ======= 
Ce fichier contient tous les fichiers nécessaire au SRD. 

======= STRUCTURE DES FICHIERS =======
Le fichier principal et à compiler est master.tex. Les fichiers principaux des chapitres sont contenus dans le même dossier. 
Les sections de ces chapitres sont eux dans des sous-dossiers. Un sous-fichier doit contenir l'entête \documentclass[fichier_parent]{subfiles}.
Dans le fichier parent, vous pouvez inclure un fichier enfant avec \subfile{fichier}.

Nous devrions alors avoir la structure de fichier suivante:
srd/
├─ chapitre1/
|  ├─ img/
|     ├─ mon_image.png
|  ├─ section1.tex
|  ├─ section2.tex
├─ chapitre2/
|  ├─ section1.tex
|  ├─ section2.tex
├─ chapitre1.tex
├─ chapitre2.tex
├─ master.tex
├─ master.pdf

===== COMPILATION =======
Si vous souhaiter compiler le fichier, vous pouvez utiliser TeXLive. 
Vous aurez cependant besoin du package français de TeXLive (https://packages.debian.org/fr/sid/texlive-lang-french pour Debian/Ubuntu/Mint). 
Si l'un d'entre vous n'arrive pas a compiler, je regarderai régulièrement le repo pour voir si des modifications ont été faites qui auraient besoin 
d'être compilées.

En bref, si, comme moi vous utilisez Debian ou tout autre distro qui s'en dérive (Ubuntu/Mint), 
entrez ça dans le répertoire srd, ca devrait compiler et ouvrir le fichier PDF :)

> sudo apt-get install texlive-latex-base texlive-latex-extra texlive-latex-recommended texlive-lang-french
> pdflatex master.tex
> evince master.pdf

