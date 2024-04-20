# Battleship - Groupe 9
## Build le projet
### Dependencies
Pour faire fonctionner le projet, vous aurez besoin des libraires suivantes:
 - Make · `sudo apt install make`
 - CMake · `sudo apt install cmake`
 - CppRestSDK · `sudo apt install libcpprest-dev`
 - libSQLite3-dev · `sudo apt install libsqlite3-dev`
 - libsodium-dev · `sudo apt install libsodium-dev`
 - qt6 · `sudo apt install qt6-base-dev`

Pour vous aider à installer les dépendances, un script est là pour simplifier la tache si vous êtes sur un système basé sur Debian:
```sh
> chmod +x dependencies.sh
> ./dependencies.sh
```

### Compilation
Pour build le projet, vous avez deux options, soit vous compilez le projet en mode "Release" et pour cela il suffit de faire la commande suivante:
```sh
> make
```
Soit vous le compilez en mode "Debug" et il suffit alors de faire cette commande ci:
```sh
> make debug
```

Si pour l'une ou l'autre raison les commandes au dessus ne fonctionnent pas, vous pouvez essayer de le compiler en faisant la manipulation suivante:
```sh
> mkdir build && cd build
> cmake ..
> make
```

## Membres du projet
- Islam BENAIM SAHTANE RGHIF (@ibenaims)
- Romain CROUGHS (@rcro0001)
- Noah DEBUS (@ndeb0013)
- Alessandro DORIGO (@adorigo)
- Gabriel GOLDSZTAJN (@ggoldszt)
- Manon GORTZ (@mgor0004)
- Pawel JADCZUK (@pjadczuk)
- Philippe NYABYENDA (@pnya0002)
- Lucas VAN PRAAG (@lvan0086)