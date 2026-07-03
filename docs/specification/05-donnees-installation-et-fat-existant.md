# Données d'entrée de l'installation et FAT existant

## Vue d'ensemble

Le projet porte sur un filtre à tambour (FAT) deja partiellement construit et intégré dans une ligne de filtration de bassin. Les dimensions et interfaces ci-dessous sont considerees comme des caractéristiques d'entrée stables pour la conception mécanique, hydraulique et de pilotage.

## Environnement d'installation

Le FAT sera installe dans un local de filtration maconne, isole avec 5 cm de XPS. Un capot transparent relevable sera present au-dessus du petit batiment afin de voir le tambour tourner sans ouvrir le FAT. Le capot et le couvercle transparent désignent la même pièce physique. Sa matiere et son niveau d'isolation restent à définir.

Le FAT ne sera donc pas expose à la pluie directe. L'environnement reste toutefois un local de filtration humide, avec risque de condensation, projections ponctuelles et contraintes de maintenance.

Le capteur de température ambiante local doit mesurer l'air du local de filtration. Son implantation exacte doit éviter une mesure trop influencée par le coffret électrique, un équipement chaud, le soleil ou le volume sous capot si celui-ci créé une stratification thermique.

## Géométrie immuable du FAT

### Enveloppe générale

| Element | Valeur | Notes |
| --- | --- | --- |
| Surface totale eau propre + eau sale | 78 cm x 47 cm | Surface interne totale du filtre |
| Hauteur d'eau maximale avant debordement | 30,5 cm | Au-delà, l'eau deborde dans la goutiere d'évacuation |
| Volume géométrique théorique total jusqu'au trop-plein | 111,8 L | Calcul: 78 x 47 x 30,5 cm |

### Partie eau propre

| Element | Valeur | Notes |
| --- | --- | --- |
| Compartiment eau propre | 62 cm x 47 cm x 30,5 cm max | Volume géométrique théorique: 88,9 L |
| Tambour | diamètre 31 cm x longueur 57 cm | Longueur utile côté eau propre |
| Volume géométrique du cylindre de tambour | 42,9 L | Valeur utile pour l'encombrement, pas pour estimer a elle seule le volume d'eau libre |

### Partie eau sale

| Element | Valeur | Notes |
| --- | --- | --- |
| Largeur théorique résiduelle | 16 cm | Difference entre 78 cm et 62 cm |
| Surface théorique résiduelle | 16 cm x 47 cm | À confirmer sur plan si des cloisons locales modifient cette section |
| Volume géométrique théorique jusqu'au trop-plein | 22,9 L | Calcul sur la section résiduelle |

## Hypothèse hydraulique de travail sur le niveau côté sale

| Element | Valeur | Notes |
| --- | --- | --- |
| Niveau d'eau sale nominal vise | environ 20 cm au-dessus du bas du tambour | Hypothèse de travail à confirmer sur installation |
| Position du bas du tambour | 7 cm au-dessus du fond du filtre | Donnée fournie |
| Niveau d'eau correspondant depuis le fond du filtre | environ 27 cm | 7 cm + 20 cm |
| Marge sous le trop-plein | environ 3,5 cm | Avec trop-plein fixe à 30,5 cm |

### Conséquences estimées sur l'immersion du tambour

| Element | Valeur | Notes |
| --- | --- | --- |
| Hauteur d'eau sur le tambour | environ 20 cm | Soit environ 64,5 % du diamètre 31 cm en hauteur verticale |
| Circonference totale du tambour | environ 97,4 cm | Calcul à partir du diamètre 31 cm |
| Surface totale de toile du tambour | environ 0,55 m2 | Circonference x longueur 57 cm |
| Part de circonference immergée | environ 59 % | Soit une longueur developpee immergée d'environ 58 cm |
| Surface de toile immergée brute | environ 0,33 m2 | Sur environ 0,55 m2 de toile totale |
| Surface filtrante utile visee | environ 0,20 à 0,23 m2 | Selon la geometrie réelle des ouvertures et le taux de passage utile |

## Interfaces hydrauliques connues

| Interface | Valeur | Notes |
| --- | --- | --- |
| Arrivee eau sale | 2 entrées de 110 mm | Bonde de fond + skimmer en gravitaire |
| Sortie eau propre | 2 sorties de 110 mm | Maintien du flux hydraulique vers la filtration bio |
| Goutiere d'évacuation | trop-plein à 30,5 cm | Goutiere fixe avec legere pente |
| Évacuation des dechets rinces | tuyau de 100 mm | Place en sortie de goutiere |
| Report de niveau eau propre | tuyau de 32 mm | Support de fixation des capteurs de niveau |
| Mise à l'air du report de niveau | trou de 1 mm dans le bouchon | Le bouchon haut reste demontable pour nettoyage |
| Rampe d'aspersion | tuyau de 32 mm + buses | A raccorder à la pompe de rinçage |

## Composants deja retenus

### Filtration mécanique

| Element | Valeur | Notes |
| --- | --- | --- |
| Toile de filtration | inox 74 microns | Maille de filtration retenue pour le tambour |

### Capteurs de niveau

| Element | Valeur | Notes |
| --- | --- | --- |
| Référence capteur | CR18-8DN | Capteur de niveau retenu |
| Format mécanique | M18 | A prendre en compte pour le support sur tube de report |
| Distance de détection ajustable | 8 mm | Valeur fournie |
| Sortie | NPN | Interface contrôleur a adaptér en conséquence |
| Alimentation | 12-24 VDC | Compatible avec plusieurs architectures basse tension |
| Consommation | 10 mA | Valeur unitaire fournie |
| Câblage | DC 3 fils | À intégrer au schéma de câblage |

### Motorisation tambour

| Element | Valeur | Notes |
| --- | --- | --- |
| Moteur candidat | Moteur d'essuie-glace avant Peugeot 106 phase 2 | Moteur avec reducteur intégré et tringlerie |
| Fabricant / référence | SWF 403.835 / 403835 | Référence longue observée : 7905562055 |
| Référence PSA probable | 640583 / 6405.83 | Équivalence probable, a ne pas considerer comme preuve directe |
| Connecteur | 5 broches | Brochage exact à identifier avant câblage |
| Tension moteur | 12 V DC | Hypothèse de conception |
| Vitesse retenue | Petite vitesse uniquement | Limité vitesse tambour, bruit, a-coups et consommation |
| Puissance estimée | environ 40 W | Hypothèse de dimensionnement, non confirmée par fiche constructeur |
| Courant de fonctionnement à prévoir | 4 à 8 A | À mesurer sur montage réel |
| Courant de démarrage ou blocage | 10 à 20 A possible | Dimensionnant pour alimentation, fusible et protection |
| Alimentation recommandée | 12 V DC / 15 A | Minimum prudent : 12 V DC / 10 A |
| Fusible initial | 10 à 15 A | A ajuster après essais |
| Vitesse sortie moteur estimée | 40 à 60 tr/min a vide | Valeur typique à valider expérimentalement |
| Couple moteur | Inconnu | Donnée critique à valider par essais |
| Usage retenu | Intermittent pendant les cycles de lavage | Usage continu 24/24 non recommandé |

La transmission envisagée utilisé un pignon moteur de 10 cm vers un engrenage tambour de 30 cm, soit une réduction 3:1. La vitesse tambour attendue serait donc d'environ 13 à 20 tr/min si la vitesse moteur réelle est bien comprise entre 40 et 60 tr/min.

Le moteur doit rester hors eau et protège des projections. Le brochage exact, la vitesse, le courant en charge et le comportement en blocage doivent être valides avant intégration definitive. Le détail du dimensionnement est tracé dans [../calculs/NC-0002-dimensionnement-motorisation-tambour.md](../calculs/NC-0002-dimensionnement-motorisation-tambour.md).

### Pompe de rinçage

| Element | Valeur | Notes |
| --- | --- | --- |
| Pompe retenue | VEVOR / Leo EKJ-802S | Garden Jet Pump de surface auto-amorcante |
| Fabricant OEM | Leo Group Pump (Zhejiang) Co., Ltd. | Information fournisseur |
| Numéro de serie | 221228693910166 | Donnée plaque |
| Alimentation | 220-240 VAC, 50 Hz | Pompe secteur monophase |
| Puissance | 800 W indique projet ; 0,6 kW / 0,8 HP sur fiche Leo | Divergence a garder visible pour dimensionnement prudent |
| Courant nominal fiche | 3,5 A | Le courant d'appel moteur reste à prendre en compte |
| Débit maximal | 3,6 à 3,7 m3/h, soit environ 60 L/min | Valeur de bout de courbe, pas disponible en même temps que la hauteur maximale |
| Hauteur maximale | 40 à 43 m | Selon source ; la courbe Leo indique 43 m a débit nul |
| Raccords aspiration / refoulement | 1 pouce / 1 pouce | A adaptér vers la prise d'eau et la rampe 32 mm |
| Température liquide maximale | 35 degC projet ; 40 degC fiche | Retenir 35 degC par prudence tant que la source exacte n'est pas arbitree |
| Température ambiante maximale | 40 degC | Donnée fiche technique |
| Protection | IPX4, classe I, isolation F | Pompe a mettre à la terre, non immergeable |
| Corps de pompe | AISI 304 | Donnée fiche technique |
| Dimensions / poids | environ 38 x 24 x 30 cm, 9 kg | Fiche détaillée : L 369,5 mm, W 243,5 mm, H 284 mm |

La courbe disponible donne environ 20 L/min à 34 m, 30 L/min à 28 m, 40 L/min à 21 m, 50 L/min à 12,5 m et 60 L/min à 3 m. La courbe sert seulement d'estimation provisoire. Le débit de rinçage de référence V1 devra être mesure aux buses après montage réel de la pompe, de la rampe et des pertes de charge. Le détail du dimensionnement est tracé dans [../calculs/NC-0003-dimensionnement-pompe-rincage.md](../calculs/NC-0003-dimensionnement-pompe-rincage.md).

### Hypothèse de nommage côté eau propre

| Nom | Rôle | Notes |
| --- | --- | --- |
| EP_LAVAGE | Demande de lavage | Niveau eau propre abaisse mais pas encore critique |
| EP_CRITIQUE | Danger hydraulique et arrêt de sécurité | Niveau eau propre très bas, coeur de la protection V1 |

## Chaine de filtration autour du FAT

| Element | Valeur | Notes |
| --- | --- | --- |
| Bassin | 186 cm x 411 cm x 108 cm | Volume géométrique théorique: 8,26 m3 |
| Évacuation bassin | 1 bonde de fond 110 mm + 1 skimmer 110 mm | Fonctionnement en gravitaire vers le FAT |
| Filtration biologique | cuve IBC 1000 L avec tapis japonais | En sortie du FAT |
| Pompe de filtration principale | retour bassin en 63 mm | Fonctionnement continu vise |
| Pompe décoration | usage ponctuel | Cascade, mur d'eau, lame d'eau, etc. |
| UV | hors tambour, dans la filtration, après la pompe principale | Asservi a filtration autorisée plus absence EP_CRITIQUE ; ne pas couper sur défaut FAT non critique si la filtration reste autorisée |
| Température bassin | mesure à ajouter | Valeur utile pour alertes et exploitation saisonniere |
| Température ambiante local | mesure à ajouter | Valeur utile pour alertes environnementales et suivi du local technique |

## Hypothèses hydrauliques de dimensionnement de la filtration

### Objectif de renouvellement du bassin

Le renouvellement cible de l'eau du bassin est fixe entre 1 h et 1 h 30. Avec un volume géométrique théorique de 8,26 m3, cela conduit a une plage de débit nominal visee d'environ 8 à 10 m3/h pour la filtration principale.

### Repartition cible entre bonde de fond et skimmer

| Circuit | Débit cible | Part relative | Justification |
| --- | --- | --- | --- |
| Bonde de fond | 6 à 7 m3/h | environ 75 % | Garantir au moins 6 m3/h dans la conduite de bonde de fond pour conserver une vitesse d'entrainement d'environ 0,18 m/s |
| Skimmer | 2 à 3 m3/h | environ 25 % | Compléter l'aspiration de surface sans penaliser la bonde de fond |
| Total filtration principale | 8 à 10 m3/h | 100 % | Compromis entre renouvellement du bassin, capacité du FAT et temps de passage en bio |

### Contraintes de débit sur le FAT et la biofiltration

- Le FAT existant est considere comme potentiellement sous-dimensionne en surface filtrante au-delà de 10 m3/h.
- La cuve de biofiltration ne doit pas être traversee a un débit trop eleve afin de conserver un temps de contact acceptable avec les bactéries.
- Les temps de passage théoriques actuellement retenus dans la bio sont d'environ 7,5 minutes à 8 m3/h et 6 minutes à 10 m3/h.
- La presence d'une deuxième pompe de décoration ou d'usage ponctuel est susceptible de modifier les equilibres hydrauliques globaux et devra être prise en compte lors des reglages finaux.

### Conséquence pour le dimensionnement de la pompe principale

Le point de fonctionnement nominal recherche reste 8 à 10 m3/h, mais la pompe principale devra conserver une marge suffisante pour absorber les pertes de charge réelles de l'installation, rester réglable finement et éviter un fonctionnement permanent à pleine vitesse. Les calculs et la decision associés sont tracés dans [../calculs/NC-0001-dimensionnement-pompe-filtration.md](../calculs/NC-0001-dimensionnement-pompe-filtration.md) et [../decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md](../decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md).

## Hypothèses de conception à retenir

- Le seuil physique de debordement du filtre est impose par la goutiere à 30,5 cm au-dessus du fond du filtre.
- La logique de pilotage V1 repose sur une cote simple côté eau propre, avec deux niveaux fonctionnels : EP_LAVAGE et EP_CRITIQUE.
- Les capteurs de niveau doivent être positionnes côté eau propre sur le tube de report en 32 mm, et non directement dans la zone sale.
- Les capteurs de niveau retenus sont des CR18-8DN en M18, sortie NPN, alimentation 12-24 VDC, câblage 3 fils.
- Le moteur tambour candidat est un moteur d'essuie-glace avant SWF 403.835 en 12 V DC, a utiliser en petite vitesse et en fonctionnement intermittent.
- L'alimentation moteur doit être dimensionnée sur les appels de courant de démarrage et de blocage, pas seulement sur la puissance estimée de 40 W.
- Une protection contre blocage ou surintensité du moteur tambour doit être prévue ou fortement justifiée si elle est absente.
- La pompe de rinçage retenue est une pompe de surface VEVOR / Leo EKJ-802S en 220-240 VAC ; elle doit être commandée comme une charge moteur secteur.
- Le débit de rinçage de référence ne doit pas être confondu avec le débit maximal de 60 L/min, car le point de fonctionnement dépendra des buses et des pertes de charge.
- Le support du FAT devra fixer la cote altimetrique du trop-plein par rapport au niveau hydraulique du bassin. Cette cote doit être mesurée sur site avant fabrication du support ; elle ne doit pas être inventee en spécification.
- La pompe décoration aspirant au même endroit que la pompe principale, elle doit suivre les mêmes règles de sécurité hydraulique.
- Les diametres d'interconnexion existants 110 mm, 100 mm, 63 mm et 32 mm doivent être preserves dans les choix d'implantation et de pilotage.

## Reperes de niveau à définir

| Repere | Description | État |
| --- | --- | --- |
| Niveau normal côté sale | Niveau nominal observé dans la chambre eau sale en fonctionnement etabli | Hypothèse de travail: environ 20 cm au-dessus du bas du tambour, soit environ 27 cm depuis le fond du filtre |
| Niveau normal côté propre | Niveau nominal observé dans la chambre eau propre et sur le report de niveau | À définir |
| Niveau de declenchement du lavage | Niveau côté eau propre à partir duquel le FAT doit lancer un cycle de lavage | À définir |
| Niveau bas de sécurité | Niveau côté eau propre à partir duquel l'installation passe en état de sécurité | À définir |

### Contraintes sur ces reperes

- Le niveau bas de sécurité doit rester au-dessus du niveau minimal acceptable pour protéger pompes, UV et hydraulique.
- Le niveau de declenchement du lavage et le niveau bas de sécurité sont mesures côté eau propre sur le report de niveau.
- Le niveau de declenchement du lavage doit rester suffisamment sous le trop-plein physique à 30,5 cm pour laisser une marge de reaction.
- Les niveaux normaux côté sale et côté propre serviront de référence pour comprendre le comportement hydraulique du filtre propre.
- Les ecarts eau sale / eau propre peuvent rester utiles pour les observations de mise au point, mais ils ne constituent pas la logique de pilotage V1.
- Ces reperes devront être convertis en cotes physiques mesurables depuis le fond du filtre.
- La surface de filtration utile finale dépendra de la geometrie des ouvertures du tambour sous la toile inox 74 microns. L'objectif V1 est d'obtenir environ 0,20 à 0,23 m2 de surface filtrante utile ; le calcul doit être valide avant découpe ou perçage.

## Politique de sécurité en cas de niveau bas

### Équipements à couper ou inhiber

| Équipement | Action attendue | Justification |
| --- | --- | --- |
| Pompe principale de filtration | Coupure | Éviter de vider le bassin et protéger la pompe contre la marche à sec |
| Pompe décoration | Coupure | Même point d'aspiration que la pompe principale : même protection contre vidange du bassin et marche à sec |
| UV | Coupure | Éviter un fonctionnement hors d'eau sans refroidissement correct |
| Rotation du tambour | Inhibition | Éviter un cycle FAT en situation d'eau insuffisante |
| Pompe de rinçage | Inhibition | Éviter un lavage inutile ou degradant en situation anormale |
| Mise à niveau automatique du bassin | Coupure | Éviter de remplir indefiniment un bassin en cas de fuite |

### Équipements hors sorties contrôlées

| Équipement | Action attendue | Justification |
| --- | --- | --- |
| Bulleur cuve bio | Branchement direct 220 V, hors contrôleur | Préserver les bactéries de filtration biologique |
| Bulleur bassin | Branchement direct 220 V, hors contrôleur | Maintenir l'oxygénation des poissons et limiter la glace en hiver |

## Travaux restants sur le FAT existant

### Mécanique et hydraulique

- Réaliser les ouvertures dans le tuyau qui sert de tambour.
- Poser la maille inox autour du tambour.
- Raccorder la rampe d'aspersion à la pompe de rinçage.
- Percer une paroi du filtre pour la prise d'eau de la pompe de rinçage.
- Mesurer le débit réel de rinçage et vérifier la pression ou qualité de jet aux buses.
- Percer le trou de vidange du filtre.
- Installer le joint a levre autour du tambour pour separer eau propre et eau sale.
- Fabriquer le support du FAT afin de positionner correctement le trop-plein par rapport au bassin.

### Instrumentation et automatisme

- Fixer les capteurs de niveau sur le tube de report en 32 mm.
- Identifier le brochage exact du moteur d'essuie-glace et valider son sens de rotation.
- Installer une alimentation 12 V DC et une protection adaptées au courant de démarrage du moteur tambour.
- Concevoir et integrer toute la partie intelligence et pilotage du FAT.
- Ajouter un capot transparent de fermeture avec capteur de détection d'ouverture.
- Ajouter une sonde de température d'eau avec implantation maintenable et représentative.
- Ajouter une sonde de température ambiante du local avec implantation représentative.
- Définir une IHM locale permettant de remonter clairement le statut du système.
- Prévoir une extension Wi-Fi V2 de remontée à distance de l'état et des alarmes sans remplacement de la plateforme principale.
- Prévoir pour une V2 les notifications immédiates actionnables et leur politique anti-repetition : apparition, rappel rare et retour à la normale.
- Prévoir pour une V2 une synthèse quotidienne de fonctionnement et la possibilité de la désactiver.
- La pompe décoration pourra être pilotée en V1.1/V2 par deux plages horaires maximum par jour, identiques tous les jours, avec interrupteur logiciel actif/inactif pour les longues périodes d'arrêt dont l'hiver.
- Définir la liste finale des alarmes indirectes et leur formulation utilisateur.
- Définir pour une V1.1 un test journalier automatique avec diagnostic.
- Définir pour une V1.1 une stratégie d'indexation du tambour pour repartir l'immersion.
- Définir pour une V1.1 les statistiques de lavage pertinentes pour suivre l'état du filtre.
- Définir pour une V1.1 ou V2 un indice simple d'encrassement exploitable dans le temps.
- Définir pour une V1.1 ou V2 une estimation empirique de la consommation d'eau du rinçage et du besoin d'appoint associé.
- Définir pour une V1.1 les temps de fonctionnement cumulés des organes principaux.

## Questions techniques encore ouvertes

- Cotes exactes du niveau normal côté sale, du niveau normal côté propre, du seuil de lavage et du niveau bas de sécurité.
- Nombre exact de capteurs CR18-8DN à installer et implantation précise de chacun.
- Couple réel, vitesse réelle, courant en charge et seuil de protection du moteur de tambour.
- Brochage exact du moteur de tambour, y compris fonctions de parking a isoler ou ignorer proprement en V1.
- Débit réel, pression utile aux buses, amorçage et courant réel de la pompe de rinçage.
- Forme, nombre et surface totale des ouvertures du tambour a calculer avant découpe pour atteindre environ 0,20 à 0,23 m2 de surface filtrante utile.
- Architecture de puissance pour separer clairement les sorties à couper des équipements hors contrôleur.
- Stratégie de sécurité a appliquer si le capot est ouvert pendant un cycle.
- Position finale du capteur de température bassin et technologie retenue.
- Seuils d'alerte température eau V1 : basse < 4 deg C, haute > 28 deg C, configurables.
- Position finale du capteur de température ambiante local et technologie retenue, avec mesure représentative de l'air du local de filtration.
- Seuils d'alerte température ambiante local V1 : basse < 2 deg C, haute > 40 deg C, configurables.
- Niveau d'IHM locale à retenir et informations a afficher localement.
- Nombre, couleur et signification des voyants si une signalisation lumineuse est retenue.
- Liste exacte des informations visibles en permanence sur l'IHM locale et de celles accessibles en détail.
- Canal de notification à distance V2 retenu : Wi-Fi, avec matériel MVP compatible.
- Liste des événements a notifier et comportement en cas de perte réseau pour une V2, sur la base des événements actionnables deja retenus.
- Activation par défaut, horaire et contenu exact de la synthèse quotidienne pour une V2.
- Ne pas créer d'information distante "bassin niveau bas" sans capteur bassin distinct ; avec l'instrumentation retenue, EP_CRITIQUE doit être présente comme `NIVEAU FAT CRITIQUE`.
- Programmation pompe décoration retenue pour V1.1/V2 : deux plages maximum par jour, identiques tous les jours, et interrupteur logiciel actif/inactif sans automatisme hiver au départ.
- Priorité pompe décoration retenue : sécurités hydrauliques et défauts bloquants, commande manuelle locale, commande distante, puis programmation horaire.
- Validation physique de l'ordre et de la logique des capteurs EP_LAVAGE et EP_CRITIQUE.
- Choix de rester en diagnostic indirect V1 confirmé ; pressostat, débitmètre, retour courant rinçage, rotation, fuite ou niveau eau sale sont reportés V1.1/V2 si les essais montrent un besoin.
- Règles de lancement, de verdict et de report du test journalier automatique en V1.1.
- Stratégie retenue en V1.1 pour ne pas laisser toujours la même zone du tambour immergée.
- Règles de calcul et de restitution des statistiques de lavage en V1.1.
- Formule retenue pour l'indice d'encrassement et usage associé en V1.1 ou V2.
- Methode empirique d'estimation de la consommation d'eau et des pertes vers évacuation en V1.1 ou V2.
- Règles de cumul et d'affichage des temps de fonctionnement en V1.1 ; remises à zéro maintenance et seuils de rappel reportés V2.
