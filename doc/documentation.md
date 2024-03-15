# Documentation des nouvelles classes pour les parties :
Ce document sert a documenter les modifications qui ont eu lieu dans le code, lié a la partie bataille navale du projet. Un effort a été fait pour essayer, tant que possible, de rendre le nouveau code compatible avec l'implementation précédente du jeu.
## dossier Common :
### Classe Player :
#### Instances de classe :
##### Privées :
- `_faction: Faction`
    - instance indiquant la faction du joueur, lors d'une partie cet instance est accedée pour determiner ce qui doit etre affiché lors du choix d'un coup.
- `_fleet: std::vector<ship&>`
    - un vecteur contenant des références vers tout les bateaux que le joueur a posé sur son coté du plateau de jeu.
- `_is_turn: bool`
    - un booléen indiquant si c'est le tour du joueur, était précédemment stocké directement dans le (local_)board
- `_energy_points: int`
    - instance permettant de stocké localement la quantité de points d'énergie disponible au joueur. Permet au `GameController` de s'assurer que le coup que le joueur veut faire est possible.

#### Méthodes de classe :
##### Publiques :
1. Constructeurs :
    1. par defaut :
        - `Player();`
            - Crée un joueur ayant la faction "classique". Utilisé pour le mode de jeu Classique.
    2. Avec comme argument une Faction:
        - `Player(Faction faction);`
            - Crée un joueur ayant la faction précisée. Utilisé pour le mode de jeu Commandant.
    3. Constructeur par copie :
    - `Player(const Player& other);`
        - permet de copier une instance de Player
2. Getters :
    - `Faction getFaction() const;`
        - renvoie un objet Faction, representant la Faction a laquelle appartient le joueur.
    - `std::vector<Ship> getFleet() const;`
        - renvoie le vecteur de bateaux possédés par un joueur.
    - `bool isTurn() const;`
        - renvoie la valeur de la variable d'instance isTurn.
    - `int getEnergyPoints() const;`
        - renvoie les points d'énergie que le joueur possède actuellement.
3. Setters :
    - `void setFaction(Faction faction);`
        - Permet de modifier la faction d'un joueur (inutilisée pour l'instant).
    - `void setFleet(std::vector<Ship> fleet);`
        - Permet d'ajouter tout les bateaux choisi par le joueur en une seule fois.
    - `void addShip(Ship ship);`
        - Permet d'ajouter un bateau au vecteur de bateaux deja possédés par le joueur.
    - `void setTurn(bool is_turn);`
        - Permet de modifier la valeur de is_turn, lorsque le joueur a fait un coup.
    - `void removeEnergyPoints(int to_remove);`
        - permets d'enlever `to_remove` points d'energie au joueur.

### Classe Faction :
#### Typedef's :
- `typedef int Amount;`
- `typedef int Size;`
- `typedef std::pair<Amount, Size> PossibleShip;`
- `typedef std::vector<PossibleShip> PossibleShips;`
- `typedef std::vector<SpecialAbility> SpecialAbilities;`

#### Instances de classe :
##### Privées :
- `std::string _name;`
    - Nom de la faction
- `PossibleShips _possible_ships;`
    - Vecteur contenant des paires d'`int` representant le nombre et la taille de chaque bateau possible
- `SpecialAbilities _special_abilities;`
    - Vecteur contenant les capacités speciales accessibles a la faction
#### Méthodes de classe :
##### Protégées :
- `virtual void setName(std::string name);`
    - Permet de modifier le nom de la faction à une classe heritant de celle-ci.
- `virtual void setPossibleShips(PossibleShips possible_ships);`
    - Permet de modifier le vecteur de bateaux accessibles à la faction à une classe heritant de celle-ci.
- `virtual void addSpecialAbility(SpecialAbility new_ability);`
    - Permet d'ajouter une capacité spéciale accessible à la faction à une classe heritant de celle-ci.
- `virtual void setSpecialAbilities(SpecialAbilities abilities);`
    - Permet de modifier le vecteur de capacités spéciales accessibles à la faction à une classe héritant de celle-ci.
##### Publiques :
1. Constructeurs :
    - Par défaut :
        - `Faction() = default;`
            - Constructeur par defaut initialisant la classe avec les valeur par defaut des differentes instances a l'interieur de celle ci.
    - Avec arguments :
        - `Faction(std::string name, PossibleShips possible_ships, SpecialAbilities special_abilities)`
            - initialise un objet Faction avec le nom, le vecteur de bateaux possibles, et de capacités speciales possibles
2. Getters :
    - `virtual std::string getName();`
        - Renvoie le nom de la faction.
    - `virtual std::vector<PossibleShip> getPossibleShips();`
        - Renvoie le vecteur avec les quantités et tailles de bateaux.
    - `virtual PossibleShips getSpecialAbilities();`
        - Renvoie le vecteur de capacités speciales.
#### Classes spécialisantes de Faction:
Les classes spécialisantes heritent toutes de Faction, et ont toutes un constructeur par defaut initialisant directement les pouvoirs et bateaux disponibles au joueur pour une certaine faction donnée.
##### FactionClassique
##### FactionBombardement
##### FactionMines
##### FactionSonar

### Classe SpecialAbility :
#### typedef's :
- `typedef enum SpecialAbilityType`
    - défini les differents types de capacité possibles, pour avoir une gestion "simplifiée" au niveau du controlleur pour savoir quoi faire en fonction de l'action entreprise.
#### Instances de classe :
##### Privées :
- std::string name_;
    - Nom de la capacité speciale.
- std::string description_;
    - Description de la capacité spéciale.
- int energy_cost_;
    - Cout de la capacité spéciale.
- SpecialAbilityType type_;
    - Type de la capacité speciale.
#### Méthodes de classe :
##### Protégées :
- `void    setName(std::string new_name);`
    - Modifie le nom de la capacité spéciale.
- `void    setDescription(std::string new_description);`
    - Modifie la déscription de la capacité spéciale.
- `void    setEnergyCost(int new_cost);`
    - Modifie le cout de la capacité spéciale.
- `void    setType(SpecialAbilityType new_type);`
    - modifie le type de la capacité spéciale.
##### Publiques :
1. Constructeurs :
    - `SpecialAbility(std::string name, std::string description="", int energy_cost=0, SpecialAbilityType type=NONE)`
        - Construit un objet de la classe SpecialAbility avec un nom, description, cout et type.
2. Getters :
    - `std::string getName();`
        - Renvoie le nom de la capacité.
    - `std::string getDescription();`
        - Renvoie la description de la capacité.
    - `int getEnergyCost();`
        - Renvoie le cout de la capacité.
    - `SpecialAbilityType getType();`
        - Renvoie le type de la capacité.

#### Classes spécialisantes de SpecialAbility:
Les classes spécialisantes heritent toutes de SpecialAbility, et ont toutes un constructeur initialisant directement la capacité spéciale avec le nom, description, cout et type adapté. le seul parametre a précisé est le cout, car il peut varier pour une meme capacité d'une faction a une autre.
##### Torpedo
##### PiercingTorpedo
##### BigTorpedo
##### AerialStrike
##### Sonar
##### Probe
##### Mines

## dossier Client :
### Modification apportées :