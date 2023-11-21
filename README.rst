=============
 Code fourni 
=============

Vous trouverez dans ce dossier un code pour vous aider à démarrer le projet.

Qu'est-ce qui est attendu concernant ce code?
=============================================

Le code est fourni pour vous aider.
Vous être libre de vous en servir ou non.
Si vous vous en servez, il sera considéré de la même façon que le reste du code rendu: vous pouvez le modifier et vous devez l'intégrer proprement à votre projet, compléter la documentation…

Quel est l'intérêt du code fourni?
==================================

Son principal intérêt est de vous donner un exemple de code bien structuré.
Nous attendons une structure de cette qualité dans votre projet.
Nous vous conseillons donc de commencer par étudier la structure du code: pour chaque classe:

#. comprenez son rôle,
#. regardez le type de ses attributs, cela vous renseigne sur ce qu'elle connaît en dehors d'elle-même et avec qui elle communique,
#. regardez ses méthodes et comprenez pourquoi elles sont implémentées dans cette classe.

Avant de vous lancer dans ce travail, voyez la suite de ce document.

Ce code vous permet aussi de commencer le projet rapidement.

Que contient le code fourni?
============================

Le code fourni contient surtout l'affichage d'une partie en cours.

- ``BoardDisplay`` définit l'interface pour les classes d'affichages *(display)* d'une partie *(board)*.
- ``BoardView`` définit l'interface que les classes d'affichages attendent des classes leur fournissant les informations sur la partie en court *(board)*.
- ``BoardControl`` définit l'interface des classes auxquelles les classes d'affichage transmettent les actions effectuées par l'utilisateur.
- ``ConsoleBoardDisplay`` implémente un affichage console. Elle hérite de ``BoardDisplay`` et attend un ``BoardView`` et un ``BoardControl`` en paramètre.
- ``DummyBoard`` implémente les interfaces ``BoardView`` et ``BoardControl``, c.-à-d., les méthodes nécessaires pour tester un minimum la vue. C'est une classe bidon.

Que fait le code fourni?
========================

.. NOTE:: L'affichage du jeu efface fréquemment la console.
   Nous vous conseillons donc de rediriger les messages d'erreurs vers un fichier ouvert dans une autre console. Par exemple::

      ./battleship 2> /tmp/battleship-err # (1)
      # dans une autre console:
      tail -f /tmp/battleship-err # (2)

   ``tail -f`` affiche la fin du fichier puis affiche toute nouvelle ligne qui s'ajouterait.

   ``make run`` lance la commande (1), vous pouvez donc utiliser ``make && make run`` dans une console et garder une autre console ouverte avec la commande (2).

Le jeu affiche une partie en cours où c'est *à votre tour*.
Vous pouvez alors taper des coordonnées (au format "a1" ou "A1").

#. Si vous n'entrez pas des coordonnées ou si elles sont en dehors du plateau, ``ConsoleBoardDisplay`` efface votre choix et attend une nouvelle entrée.
#. Si vous entrez les coordonnées d'une cellule déjà révélée, ``ConsoleBoardDisplay`` transmet le tir au ``BoardControl`` qui lui indique que le tir n'est pas valide.
   ``ConsoleBoardDisplay`` efface donc aussi le choix et attend de nouveau.
#. Si vous entrez le caractère de fin de fichier, ``ConsoleBoardDisplay`` transmet au ``BoardControl`` que le joueur quitte la partie.
#. Si vous entrez des coordonnées, ``ConsoleBoardDisplay`` transmet le tir au ``BoardControl`` qui lui indique le tir est valide.
   Le ``BoardControl`` devrait maintenant gérer le tir.
   Ici ``DummyBoard`` ne fait qu'afficher un message sur stderr, change le joueur qui doit jouer et appelle ``update`` sur le ``BoardDisplay``.
#. ``BoardDisplay`` provoque alors une erreur ``NotImplementedError``.

Que NE fait PAS le code fourni?
===============================

Notez que le code ne vous fournit pas tout ce dont vous avez besoin pour l'affichage d'une partie locale en console.
Vous devrez l'adapter selon vos préférences et ce qui vous semble le mieux pour bien s'intégrer dans l'ensemble du projet.
Voici, pour expliquer de quoi on parle, trois choix concrets qui pourraient être faits (ceci ne signifie pas que ce sont des/les bons choix):

- Changer les notions de ``my_side`` et ``their_side`` par ``player_1`` et ``player_2`` et indiquer à l'affichage quel côté il faut afficher (notez que, idéalement, le jeu en réseau ne transmettra pas au client plus que ce qu'il ne peut savoir, afin d'éviter toute triche et alléger l'utilisation du réseau),
- Instancier deux ``BoardDisplay`` et afficher celui du joueur en cours, avec un écran vide le temps de la confirmation du changement du joueur,
- Avoir deux processus et deux fenêtres, par exemple en demandant au joueur de lancer deux fois l'exécutable (potentiellement avec un launcher).

Notez aussi que le code fourni ne fait que l'affichage d'une partie (``board``) et ne s'occupe pas des autres écrans (création d'une partie…).

Glossaire
=========

Board
   Représente l'état d'une partie en cours. Un board a deux côtés, un par joueur. Le côté d'un joueur est celui qui contient ses navires.
Grid
   Dans l'affiche console, désigne la représentation d'un côté du board.
Display
   Affichage. C'est ce qui est appelée “Vue” dans le modèle MVC.
View
   Classe ou interface pour accéder à des informations (comme une std::string_view).
   Une classe / interface “View” n'aura en général que des méthodes constantes comme des getters.
   Une view peut, soit simplement retourner des informations qui ne doivent être accédées qu'en lecture, soit calculer des informations à la volée.
Dummy
   Une classe temporaire, qui n'est pas utilisée lors d'une exécution normale du programme, par exemple, une classe qui sert à en tester une autre.

.. NOTE:: N'hésitez pas à renommer les classes et méthodes fournies pour correspondre à vos conventions.

Questions
=========

Général
-------

Je n'arrive pas à compiler, pourquoi?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Quel est le message d'erreur?

La compilation nécessite une prise en charge minimale de C++20.

Avec g++, vous avez besoin de la version 10.
Vous pouvez tester votre version avec ``g++ -v |& grep version``.
Si vous utilisez la version 9 par défaut, allez dans le Makefile et ajoutez tout en haut::

   CXX=g++-10

Dans la famille Debian, vous pouvez l'installer avec ``apt install g++-10``.

Si vous n'utilisez pas ``g++`` (comme sur Mac) ou si vous n'êtes pas sur Linux, vous aurez peut-être des erreurs de type « unrecognized command-line option ». Dans ce cas allez dans le Makefile, cherchez l'option en question et supprimez-la.

Que signifient les extensions des fichiers?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``.cc``
   Fichier source C++, équivalent à ``.cpp``, mais le Makefile est prévu pour ``.cc``.
``.hh``
   Fichier d'entête C++, équivalent à ``.hpp``, mais le Makefile est prévu pour ``.hh``.
``.d``
   Fichier de dépendances généré par le compilateur. C'est un sous-Makefile créé par le compilateur à l'exécution du Makefile principal et qui indique les fichiers dont dépend la compilation d'un fichier.
``.o``
   Fichier « objet ». C'est du code compilé mais qui contient potentiellement des appels à des fonctions non définies: si le compilateur a la déclaration d'une fonction (contenu des ``.hh``), il peut écrire le code d'appel de la fonction. Une fois tous les fichiers ``.o`` créés, ils sont fusionnés par le linker qui fait le lien entre l'appel des fonctions et le code (l'adresse) des fonctions typiquement définies dans un autre ``.o``.

.. ATTENTION::
   Le Makefile demande la génération des ``.d`` seulement si les ``.o`` n'existent pas.
   Si vous supprimez les ``.d``, supprimez aussi les ``.o``.

Voyez plus bas pour d'autres remarques sur la compilation.

Qu'est-ce que le ``#pragma once`` écrit en haut des ``.hh``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

En un mot, c'est une version raccourcie des gardes::

   #ifndef _FICHIER_
   #define _FICHIER_
   ...
   #endif

Comme expliqué plus haut, pour appeler une fonction, le compilateur a besoin de sa déclaration (signature et type de retour mais sans le code).
Dans un fichier, on va donc inclure les fichiers headers contenant les fonctions utilisées.
Cependant, ``#include`` est une instruction donnée au précompilateur qui remplace l'instruction par le contenu du fichier inclut.
Ceci peut poser problème si j'inclus dans un header un header qui m'inclut.
Dans ce cas le compilateur risque de me dire que j'ai plusieurs définitions d'une même fonction ou d'une même classe, parce qu'il lira plusieurs fois le même fichier.
Les gardes, et aujourd'hui la pragma once, permettent d'éviter d'inclure plusieurs fois le même fichier dans une même unité de compilation.

``board_coordinates.hh``
------------------------

Que signifie ``constexpr``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Le mot clé ``constexpr`` signifie que, dans certaines conditions, le compilateur doit, dès la compilation, remplacer l'appel de la fonction par la valeur retournée, et vous prévenir si quelque chose l'empêchera toujours de le faire quel que soit le contexte.

Vous ne devez pas trop vous en inquiéter pour votre projet.

Que signifie ``[[nodiscard]]``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Vous ne devez pas trop vous en inquiéter non plus pour votre projet.

L'annotation ``[[nodiscard]]`` demande au compilateur d'émettre un warning si la valeur de retour est ignorée.
Son utilisation est bien expliquée sur `Stackoverflow`_.
En résumé: supposons une méthode ``date.removeOneDay()``.
Si cette méthode modifie l'objet date, alors il ne faut pas mettre ``[[nodiscard]]``.
Une bonne utilisation serait::

   Date date();
   date.removeOneDay(); // (*)
   cout << "Hier c'était le " << date;

Mais si ``date.removeOneDay()`` ne modifie pas date mais retourne un nouvel objet, alors le code précédent n'aurait pas fonctionné.
Il aurait fallu écrire::

   Date date();
   cout << "Hier c'était le " << date.removeOneDay();

Pour éviter la confusion, dans ce deuxième cas, il aurait fallu déclarer ``removeOneDay`` avec ``[[nodiscard]]``.
Alors, un bon compilateur aurait émis un warning à la ligne notée ``(*)``.

.. _Stackoverflow: https://stackoverflow.com/a/76489653

Pourquoi est-ce indiqué que la classe ``BoardCoordinates`` ne doit pas être utilisée pour stocker les coordonnées d'un pixel? Pourquoi ne pas faire une classe ``Point`` ou ``Coordinates``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

C'est le but des types. Cela sera plus clair avec un exemple. Si je n'ai qu'une seule classe ``Point`` et une fonction ``reactToUserClick(Point pos)``, la signature ne m'indique pas si ``pos`` est en pixel (et relativement à l'écran? à la fenêtre?), ou une cellule sur un tableau, ou un autre point. En cas d'erreur le programme compile mais ne se comporte pas comme souhaité.

Si j'ai des types différents, le compilateur me dira si ce n'est pas le bon type.

``BoardView``
-------------

Comment utiliser ``BoardView::CellType``
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

L'encodage de ``BoardView::CellType`` utilise une façon de faire habituelle en C et C++ pour encoder plusieurs informations sur les différents bits d'un octet. Il faut penser aux valeurs dans leur représentation binaire. On a::

   WATER     = 000
   OCEAN     = 010 // Tir loupé
   UNDAMAGED = 001 // Navire
   HIT       = 011 // Navire touché
   SUNK      = 111 // Navire touché coulé

Les trois falgs définis ``IS_SHIP``, ``IS_KNOWN`` et ``IS_SUNK`` définissent un bit. Sachant que toute valeur autre que 0 vaut vrai, on peut tester si un bit est à 1 avec ``cell_type & flag``. Par exemple ``SUNK & IS_KNOWN`` == ``0b010`` == ``true``.

Pourquoi définir ``CellType`` dans ``BoadView``? J'ai besoin du type dans ma classe stockant l'état de la partie, pas dans l'interface.
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

À priori, le modèle stockera les informations à sa façon, quelle que soit la façon dont la vue souhaite les recevoir. Il est possible qu'il faille définir une classe ``Board::CellType`` qui soit un peu ou beaucoup différente de ``BoardView::CellType``. Peut-être que vous n'en déclarerez pas du tout. Le code fourni reste utilisable dans tous les cas.

Si vous souhaitez utiliser la même structure dans d'autres classes, il peut être intéressant de placer l'enum ``CellType`` ailleurs, peut-être dans son propre fichier.

Pourquoi ``IS_SHIP`` et les autres flags font partie de l'énumération ``CellType``? N'est-ce pas autre chose et ne faut-il pas alors un autre type?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

C'est tout à fait vrai.
Idéalement, on devrait définir une énumération ``CellTypeFlag``.
Cependant, ``SUNK & IS_KNOWN`` est alors déprécié par défaut, puisque cela demande de convertir un ``CellTypeFlag`` en un ``CellType``.
On devrait contourner ce problème un implémentant l'opérateur ``bool operator&(const CellType t, const CellTypeFlag f)``.
Ce serait une bonne chose à faire et permettrait d'utiliser le warning ``-Wswitch-enum``.

``console_board_display.hh``
----------------------------

Ce fichier est long, dois-je tout comprendre?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Dans un premier temps, vous n'avez pas besoin de comprendre le corps des fonctions, juste ce qu'elles font.

Pourquoi ``ConsoleBoardDisplay::length(str)`` alors qu'il existe ``str.length()``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

En un mot, ``str.length()`` et ``str.size()`` donne le nombre d'octet (de ``char``) dans la chaîne alors que ``ConsoleBoardDisplay::length(str)`` donne le nombre de caractères de la chaîne quand on suppose que la chaîne est encodée en UTF8, comme c'est le cas des fichiers fournis.

``not_implemented_error.hh``
----------------------------

Quand lancer cette exception?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Dans deux cas:

- Vous n'avez pas encore implémenté quelque chose et vous ne souhaitez pas utiliser une valeur bidon en attendant, ou
- Votre fonction est complète pour l'état actuel (ou actuellement prévu) de votre programme, mais elle ne gère pas tous les cas qui pourraient se présenter à l'avenir / que sa signature accepte (par exemple: la fonction reçoit un ``int`` mais ne fonctionne bien que jusqu'à 10 000).

Assurez-vous de comprendre ce deuxième point: si nous avons une fonction ``foo(int i)`` qui a du sens pour tous les ``i`` mais qui ne sera appelé qu'avec des ``i`` entre 1 et 10, quelles sont les possibilités?

#. Je code tous les cas possibles, je gère bien les *corner cases*, je passe trois jours dessus, alors que le code gérant les ``i`` de 1 à 10 m'a pris deux minutes.
#. Je mets un joli commentaire ``/* Précondition: 1 <= i <= 10 */`` et, lorsqu'un collègue (ou moi dans 3 jours) appelle la méthode avec ``11``, le programme plante ou, pire, ne se comporte pas comme prévu sans qu'il ne voie d'où ça vienne.
#. Je lance une ``std::logic_error`` indiquant que la valeur doit être entre 1 et 10. C'est presque bon, mais je risque de croire que le problème est sérieux et qu'il va falloir changer beaucoup de choses. Peut-être que je commencerai par essayer d'éviter d'utiliser 11.
#. Je lance une ``NotImplementedError`` disant que le code pour ``i`` hors de [1, 10] n'est pas fait, ce qui signifie implicitement que, pour régler le problème, il suffit de gérer le 11 à l'endroit du ``throw`` et de lancer l'erreur pour ``i`` hors de [1, 11].

Quand vous codez, si vous ne gérez pas un cas, indiquez-le à l'endroit exact où vous devriez le gérer.

Faut-il retirer tous les ``throw NotImplementedError`` pour le projet final?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Non, voyez la question précédente.

``dummy_board.hh``
------------------

.. NOTE:: ``dummy_board.hh`` étant du code temporaire, la qualité de code est un peu plus basse. Par exemple, il faudrait au moins avoir le corps des fonctions dans un fichier ``.cc``.

Pourquoi ``DummyBoard`` contient un ``weak_ptr`` (vers un ``BoardDisplay``) et non un ``shared_ptr``?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``BoardDisplay`` contient un ``shared_ptr`` vers le ``DummyBoard`` (même deux).
Si ``DummyBoard`` avait un ``shared_ptr`` vers le ``BoardDisplay``, alors les destructeurs de ces deux objets ne seraient jamais appelés.

Imaginons les pointeurs partagés suivants::

   R --> A --> B --> C
         ^- E <-- D <+

Au moment où R est supprimé, on s'attend à ce que A, B, C, D et E soient supprimés.
Cependant, le pointeur vers A ne détruit pas A puisqu'il y a encore une personne qui pointe vers lui.
Donc B n'est pas supprimé.
Donc C n'est pas supprimé…
E n'étant pas supprimé, on ne supprime pas A.
On a un « cycle orphelin ».
Pour casser le cycle, on donne à E un pointeur faible vers A.

Le pointeur faible est initialisé avec un pointeur partagé.
La ressource pointée par le pointeur faible n'est pas directement accessible.
Il faut d'abord reconvertir en pointeur partagé avec ``.lock()``.
Si l'objet pointé existe toujours, le pointeur obtenu n'est pas nul et on peut s'en servir::

   if (auto p = weak_p.lock()) {
      p->foo();
   }

Compilation
-----------

Que contient le Makefile? Faut-il l'adapter au projet?
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

``make``, ``make default`` et ``make battleship`` créer le programme ``battleship``.

``make run`` lance ``./battleship 2> /tmp/battleship-err``.
Nous vous conseillons d'avoir une autre console avec ``tail -f /tmp/battleship-err`` qui tourne pour voir les messages d'erreurs.

``make clean`` supprime les fichiers ``.o`` et ``.d``.

Vous aurez aussi rapidement besoin de compiler un client et un serveur, ce qui nécessite de changer le Makefile

Pour l'interface graphique, vous aurez sûrement des options ``-l`` à ajouter dans ``LDLIBS``.
