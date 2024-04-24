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

Pour vous aider à installer les dépendances, un script est là pour simplifier la tache si vous êtes sur un système basé sur Debian: **(à noter que l'installation version ordinateur de l'université installe uniquement les librairies qui ne sont pas encore présentes sur ceux-ci)**

```sh
> chmod +x install_dep
> ./install_dep
```

Ou, a condition que make soit deja installé:

```sh
> make install
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

Vous avez de plus les commandes suivantes a votre disposition:

```sh
# Execute le script d'installation des dépendences
> make install

# Nettoie l'environnement de travail en effacant le dossier "build"
> make clean

# Nettoie le dossier lib en ne laissant que les script d'installation des librairies
> make clean_dep

# Nettoie le dossier du projet en appelant les commandes clean, clean_dep et en effaçant les executables
> make fclean

# Nettoie et recompile le projet en mode Release
> make re

# Nettoie et recompile le projet en mode Deubg
> make re_debug
```

Si pour l'une ou l'autre raison les commandes de compilation au dessus ne fonctionnent pas, vous pouvez essayer de le compiler en faisant la manipulation suivante:

```sh
> mkdir build && cd build
> cmake ..
> make
```

## Execution du logiciel

### Serveur

Une fois la compilation finie, il faut d'abord lancer le serveur :

```sh
./server
```

Celui-ci se mets à écouter les connections entrantes a l'adresse `http://localhost:8080`

### Client

Une fois le serveur lancé, vous pouvez lancer le client :

```sh
./battleship [gui] [server_address]
```

Par défaut le client essayera de se connecter au serveur a l'addresse `http://localhost:8080`,
il est neamoins possible de passer une autre adresse en argument pour que le client se connecte a une autre addresse

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
