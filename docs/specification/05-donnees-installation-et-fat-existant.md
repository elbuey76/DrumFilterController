# Donnees d'entree de l'installation et FAT existant

## Vue d'ensemble

Le projet porte sur un filtre a tambour (FAT) deja partiellement construit et integre dans une ligne de filtration de bassin. Les dimensions et interfaces ci-dessous sont considerees comme des caracteristiques d'entree stables pour la conception mecanique, hydraulique et de pilotage.

## Environnement d'installation

Le FAT sera installe dans un local de filtration maconne, isole avec 5 cm de XPS. Un capot transparent relevable sera present au-dessus du petit batiment afin de voir le tambour tourner sans ouvrir le FAT. Le capot et le couvercle transparent designent la meme piece physique. Sa matiere et son niveau d'isolation restent a definir.

Le FAT ne sera donc pas expose a la pluie directe. L'environnement reste toutefois un local de filtration humide, avec risque de condensation, projections ponctuelles et contraintes de maintenance.

Le capteur de temperature ambiante local doit mesurer l'air du local de filtration. Son implantation exacte doit eviter une mesure trop influencee par le coffret electrique, un equipement chaud, le soleil ou le volume sous capot si celui-ci cree une stratification thermique.

## Geometrie immuable du FAT

### Enveloppe generale

| Element | Valeur | Notes |
| --- | --- | --- |
| Surface totale eau propre + eau sale | 78 cm x 47 cm | Surface interne totale du filtre |
| Hauteur d'eau maximale avant debordement | 30,5 cm | Au-dela, l'eau deborde dans la goutiere d'evacuation |
| Volume geometrique theorique total jusqu'au trop-plein | 111,8 L | Calcul: 78 x 47 x 30,5 cm |

### Partie eau propre

| Element | Valeur | Notes |
| --- | --- | --- |
| Compartiment eau propre | 62 cm x 47 cm x 30,5 cm max | Volume geometrique theorique: 88,9 L |
| Tambour | diametre 31 cm x longueur 57 cm | Longueur utile cote eau propre |
| Volume geometrique du cylindre de tambour | 42,9 L | Valeur utile pour l'encombrement, pas pour estimer a elle seule le volume d'eau libre |

### Partie eau sale

| Element | Valeur | Notes |
| --- | --- | --- |
| Largeur theorique residuelle | 16 cm | Difference entre 78 cm et 62 cm |
| Surface theorique residuelle | 16 cm x 47 cm | A confirmer sur plan si des cloisons locales modifient cette section |
| Volume geometrique theorique jusqu'au trop-plein | 22,9 L | Calcul sur la section residuelle |

## Hypothese hydraulique de travail sur le niveau cote sale

| Element | Valeur | Notes |
| --- | --- | --- |
| Niveau d'eau sale nominal vise | environ 20 cm au-dessus du bas du tambour | Hypothese de travail a confirmer sur installation |
| Position du bas du tambour | 7 cm au-dessus du fond du filtre | Donnee fournie |
| Niveau d'eau correspondant depuis le fond du filtre | environ 27 cm | 7 cm + 20 cm |
| Marge sous le trop-plein | environ 3,5 cm | Avec trop-plein fixe a 30,5 cm |

### Consequences estimees sur l'immersion du tambour

| Element | Valeur | Notes |
| --- | --- | --- |
| Hauteur d'eau sur le tambour | environ 20 cm | Soit environ 64,5 % du diametre 31 cm en hauteur verticale |
| Circonference totale du tambour | environ 97,4 cm | Calcul a partir du diametre 31 cm |
| Surface totale de toile du tambour | environ 0,55 m2 | Circonference x longueur 57 cm |
| Part de circonference immergee | environ 59 % | Soit une longueur developpee immergee d'environ 58 cm |
| Surface de toile immergee brute | environ 0,33 m2 | Sur environ 0,55 m2 de toile totale |
| Surface filtrante utile visee | environ 0,20 a 0,23 m2 | Selon la geometrie reelle des ouvertures et le taux de passage utile |

## Interfaces hydrauliques connues

| Interface | Valeur | Notes |
| --- | --- | --- |
| Arrivee eau sale | 2 entrees de 110 mm | Bonde de fond + skimmer en gravitaire |
| Sortie eau propre | 2 sorties de 110 mm | Maintien du flux hydraulique vers la filtration bio |
| Goutiere d'evacuation | trop-plein a 30,5 cm | Goutiere fixe avec legere pente |
| Evacuation des dechets rinces | tuyau de 100 mm | Place en sortie de goutiere |
| Report de niveau eau propre | tuyau de 32 mm | Support de fixation des capteurs de niveau |
| Mise a l'air du report de niveau | trou de 1 mm dans le bouchon | Le bouchon haut reste demontable pour nettoyage |
| Rampe d'aspersion | tuyau de 32 mm + buses | A raccorder a la pompe de rincage |

## Composants deja retenus

### Filtration mecanique

| Element | Valeur | Notes |
| --- | --- | --- |
| Toile de filtration | inox 74 microns | Maille de filtration retenue pour le tambour |

### Capteurs de niveau

| Element | Valeur | Notes |
| --- | --- | --- |
| Reference capteur | CR18-8DN | Capteur de niveau retenu |
| Format mecanique | M18 | A prendre en compte pour le support sur tube de report |
| Distance de detection ajustable | 8 mm | Valeur fournie |
| Sortie | NPN | Interface controleur a adapter en consequence |
| Alimentation | 12-24 VDC | Compatible avec plusieurs architectures basse tension |
| Consommation | 10 mA | Valeur unitaire fournie |
| Cablage | DC 3 fils | A integrer au schema de cablage |

### Motorisation tambour

| Element | Valeur | Notes |
| --- | --- | --- |
| Moteur candidat | Moteur d'essuie-glace avant Peugeot 106 phase 2 | Moteur avec reducteur integre et tringlerie |
| Fabricant / reference | SWF 403.835 / 403835 | Reference longue observee : 7905562055 |
| Reference PSA probable | 640583 / 6405.83 | Equivalence probable, a ne pas considerer comme preuve directe |
| Connecteur | 5 broches | Brochage exact a identifier avant cablage |
| Tension moteur | 12 V DC | Hypothese de conception |
| Vitesse retenue | Petite vitesse uniquement | Limite vitesse tambour, bruit, a-coups et consommation |
| Puissance estimee | environ 40 W | Hypothese de dimensionnement, non confirmee par fiche constructeur |
| Courant de fonctionnement a prevoir | 4 a 8 A | A mesurer sur montage reel |
| Courant de demarrage ou blocage | 10 a 20 A possible | Dimensionnant pour alimentation, fusible et protection |
| Alimentation recommandee | 12 V DC / 15 A | Minimum prudent : 12 V DC / 10 A |
| Fusible initial | 10 a 15 A | A ajuster apres essais |
| Vitesse sortie moteur estimee | 40 a 60 tr/min a vide | Valeur typique a valider experimentalement |
| Couple moteur | Inconnu | Donnee critique a valider par essais |
| Usage retenu | Intermittent pendant les cycles de lavage | Usage continu 24/24 non recommande |

La transmission envisagee utilise un pignon moteur de 10 cm vers un engrenage tambour de 30 cm, soit une reduction 3:1. La vitesse tambour attendue serait donc d'environ 13 a 20 tr/min si la vitesse moteur reelle est bien comprise entre 40 et 60 tr/min.

Le moteur doit rester hors eau et protege des projections. Le brochage exact, la vitesse, le courant en charge et le comportement en blocage doivent etre valides avant integration definitive. Le detail du dimensionnement est trace dans [../calculs/NC-0002-dimensionnement-motorisation-tambour.md](../calculs/NC-0002-dimensionnement-motorisation-tambour.md).

### Pompe de rincage

| Element | Valeur | Notes |
| --- | --- | --- |
| Pompe retenue | VEVOR / Leo EKJ-802S | Garden Jet Pump de surface auto-amorcante |
| Fabricant OEM | Leo Group Pump (Zhejiang) Co., Ltd. | Information fournisseur |
| Numero de serie | 221228693910166 | Donnee plaque |
| Alimentation | 220-240 VAC, 50 Hz | Pompe secteur monophase |
| Puissance | 800 W indique projet ; 0,6 kW / 0,8 HP sur fiche Leo | Divergence a garder visible pour dimensionnement prudent |
| Courant nominal fiche | 3,5 A | Le courant d'appel moteur reste a prendre en compte |
| Debit maximal | 3,6 a 3,7 m3/h, soit environ 60 L/min | Valeur de bout de courbe, pas disponible en meme temps que la hauteur maximale |
| Hauteur maximale | 40 a 43 m | Selon source ; la courbe Leo indique 43 m a debit nul |
| Raccords aspiration / refoulement | 1 pouce / 1 pouce | A adapter vers la prise d'eau et la rampe 32 mm |
| Temperature liquide maximale | 35 degC projet ; 40 degC fiche | Retenir 35 degC par prudence tant que la source exacte n'est pas arbitree |
| Temperature ambiante maximale | 40 degC | Donnee fiche technique |
| Protection | IPX4, classe I, isolation F | Pompe a mettre a la terre, non immergeable |
| Corps de pompe | AISI 304 | Donnee fiche technique |
| Dimensions / poids | environ 38 x 24 x 30 cm, 9 kg | Fiche detaillee : L 369,5 mm, W 243,5 mm, H 284 mm |

La courbe disponible donne environ 20 L/min a 34 m, 30 L/min a 28 m, 40 L/min a 21 m, 50 L/min a 12,5 m et 60 L/min a 3 m. La courbe sert seulement d'estimation provisoire. Le debit de rincage de reference V1 devra etre mesure aux buses apres montage reel de la pompe, de la rampe et des pertes de charge. Le detail du dimensionnement est trace dans [../calculs/NC-0003-dimensionnement-pompe-rincage.md](../calculs/NC-0003-dimensionnement-pompe-rincage.md).

### Hypothese de nommage cote eau propre

| Nom | Role | Notes |
| --- | --- | --- |
| EP_LAVAGE | Demande de lavage | Niveau eau propre abaisse mais pas encore critique |
| EP_CRITIQUE | Danger hydraulique et arret de securite | Niveau eau propre tres bas, coeur de la protection V1 |

## Chaine de filtration autour du FAT

| Element | Valeur | Notes |
| --- | --- | --- |
| Bassin | 186 cm x 411 cm x 108 cm | Volume geometrique theorique: 8,26 m3 |
| Evacuation bassin | 1 bonde de fond 110 mm + 1 skimmer 110 mm | Fonctionnement en gravitaire vers le FAT |
| Filtration biologique | cuve IBC 1000 L avec tapis japonais | En sortie du FAT |
| Pompe de filtration principale | retour bassin en 63 mm | Fonctionnement continu vise |
| Pompe decoration | usage ponctuel | Cascade, mur d'eau, lame d'eau, etc. |
| UV | hors tambour, dans la filtration, apres la pompe principale | Asservi a filtration autorisee plus absence EP_CRITIQUE ; ne pas couper sur defaut FAT non critique si la filtration reste autorisee |
| Temperature bassin | mesure a ajouter | Valeur utile pour alertes et exploitation saisonniere |
| Temperature ambiante local | mesure a ajouter | Valeur utile pour alertes environnementales et suivi du local technique |

## Hypotheses hydrauliques de dimensionnement de la filtration

### Objectif de renouvellement du bassin

Le renouvellement cible de l'eau du bassin est fixe entre 1 h et 1 h 30. Avec un volume geometrique theorique de 8,26 m3, cela conduit a une plage de debit nominal visee d'environ 8 a 10 m3/h pour la filtration principale.

### Repartition cible entre bonde de fond et skimmer

| Circuit | Debit cible | Part relative | Justification |
| --- | --- | --- | --- |
| Bonde de fond | 6 a 7 m3/h | environ 75 % | Garantir au moins 6 m3/h dans la conduite de bonde de fond pour conserver une vitesse d'entrainement d'environ 0,18 m/s |
| Skimmer | 2 a 3 m3/h | environ 25 % | Completer l'aspiration de surface sans penaliser la bonde de fond |
| Total filtration principale | 8 a 10 m3/h | 100 % | Compromis entre renouvellement du bassin, capacite du FAT et temps de passage en bio |

### Contraintes de debit sur le FAT et la biofiltration

- Le FAT existant est considere comme potentiellement sous-dimensionne en surface filtrante au-dela de 10 m3/h.
- La cuve de biofiltration ne doit pas etre traversee a un debit trop eleve afin de conserver un temps de contact acceptable avec les bacteries.
- Les temps de passage theoriques actuellement retenus dans la bio sont d'environ 7,5 minutes a 8 m3/h et 6 minutes a 10 m3/h.
- La presence d'une deuxieme pompe de decoration ou d'usage ponctuel est susceptible de modifier les equilibres hydrauliques globaux et devra etre prise en compte lors des reglages finaux.

### Consequence pour le dimensionnement de la pompe principale

Le point de fonctionnement nominal recherche reste 8 a 10 m3/h, mais la pompe principale devra conserver une marge suffisante pour absorber les pertes de charge reelles de l'installation, rester reglable finement et eviter un fonctionnement permanent a pleine vitesse. Les calculs et la decision associes sont traces dans [../calculs/NC-0001-dimensionnement-pompe-filtration.md](../calculs/NC-0001-dimensionnement-pompe-filtration.md) et [../decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md](../decisions/ADR-0002-dimensionnement-pompe-filtration-principale.md).

## Hypotheses de conception a retenir

- Le seuil physique de debordement du filtre est impose par la goutiere a 30,5 cm au-dessus du fond du filtre.
- La logique de pilotage V1 repose sur une cote simple cote eau propre, avec deux niveaux fonctionnels : EP_LAVAGE et EP_CRITIQUE.
- Les capteurs de niveau doivent etre positionnes cote eau propre sur le tube de report en 32 mm, et non directement dans la zone sale.
- Les capteurs de niveau retenus sont des CR18-8DN en M18, sortie NPN, alimentation 12-24 VDC, cablage 3 fils.
- Le moteur tambour candidat est un moteur d'essuie-glace avant SWF 403.835 en 12 V DC, a utiliser en petite vitesse et en fonctionnement intermittent.
- L'alimentation moteur doit etre dimensionnee sur les appels de courant de demarrage et de blocage, pas seulement sur la puissance estimee de 40 W.
- Une protection contre blocage ou surintensite du moteur tambour doit etre prevue ou fortement justifiee si elle est absente.
- La pompe de rincage retenue est une pompe de surface VEVOR / Leo EKJ-802S en 220-240 VAC ; elle doit etre commandee comme une charge moteur secteur.
- Le debit de rincage de reference ne doit pas etre confondu avec le debit maximal de 60 L/min, car le point de fonctionnement dependra des buses et des pertes de charge.
- Le support du FAT devra fixer la cote altimetrique du trop-plein par rapport au niveau hydraulique du bassin. Cette cote doit etre mesuree sur site avant fabrication du support ; elle ne doit pas etre inventee en specification.
- La pompe decoration aspirant au meme endroit que la pompe principale, elle doit suivre les memes regles de securite hydraulique.
- Les diametres d'interconnexion existants 110 mm, 100 mm, 63 mm et 32 mm doivent etre preserves dans les choix d'implantation et de pilotage.

## Reperes de niveau a definir

| Repere | Description | Etat |
| --- | --- | --- |
| Niveau normal cote sale | Niveau nominal observe dans la chambre eau sale en fonctionnement etabli | Hypothese de travail: environ 20 cm au-dessus du bas du tambour, soit environ 27 cm depuis le fond du filtre |
| Niveau normal cote propre | Niveau nominal observe dans la chambre eau propre et sur le report de niveau | A definir |
| Niveau de declenchement du lavage | Niveau cote eau propre a partir duquel le FAT doit lancer un cycle de lavage | A definir |
| Niveau bas de securite | Niveau cote eau propre a partir duquel l'installation passe en etat de securite | A definir |

### Contraintes sur ces reperes

- Le niveau bas de securite doit rester au-dessus du niveau minimal acceptable pour proteger pompes, UV et hydraulique.
- Le niveau de declenchement du lavage et le niveau bas de securite sont mesures cote eau propre sur le report de niveau.
- Le niveau de declenchement du lavage doit rester suffisamment sous le trop-plein physique a 30,5 cm pour laisser une marge de reaction.
- Les niveaux normaux cote sale et cote propre serviront de reference pour comprendre le comportement hydraulique du filtre propre.
- Les ecarts eau sale / eau propre peuvent rester utiles pour les observations de mise au point, mais ils ne constituent pas la logique de pilotage V1.
- Ces reperes devront etre convertis en cotes physiques mesurables depuis le fond du filtre.
- La surface de filtration utile finale dependra de la geometrie des ouvertures du tambour sous la toile inox 74 microns. L'objectif V1 est d'obtenir environ 0,20 a 0,23 m2 de surface filtrante utile ; le calcul doit etre valide avant decoupe ou percage.

## Politique de securite en cas de niveau bas

### Equipements a couper ou inhiber

| Equipement | Action attendue | Justification |
| --- | --- | --- |
| Pompe principale de filtration | Coupure | Eviter de vider le bassin et proteger la pompe contre la marche a sec |
| Pompe decoration | Coupure | Meme point d'aspiration que la pompe principale : meme protection contre vidange du bassin et marche a sec |
| UV | Coupure | Eviter un fonctionnement hors d'eau sans refroidissement correct |
| Rotation du tambour | Inhibition | Eviter un cycle FAT en situation d'eau insuffisante |
| Pompe de rincage | Inhibition | Eviter un lavage inutile ou degradant en situation anormale |
| Mise a niveau automatique du bassin | Coupure | Eviter de remplir indefiniment un bassin en cas de fuite |

### Equipements hors sorties controlees

| Equipement | Action attendue | Justification |
| --- | --- | --- |
| Bulleur cuve bio | Branchement direct 220 V, hors controleur | Preserver les bacteries de filtration biologique |
| Bulleur bassin | Branchement direct 220 V, hors controleur | Maintenir l'oxygenation des poissons et limiter la glace en hiver |

## Travaux restants sur le FAT existant

### Mecanique et hydraulique

- Realiser les ouvertures dans le tuyau qui sert de tambour.
- Poser la maille inox autour du tambour.
- Raccorder la rampe d'aspersion a la pompe de rincage.
- Percer une paroi du filtre pour la prise d'eau de la pompe de rincage.
- Mesurer le debit reel de rincage et verifier la pression ou qualite de jet aux buses.
- Percer le trou de vidange du filtre.
- Installer le joint a levre autour du tambour pour separer eau propre et eau sale.
- Fabriquer le support du FAT afin de positionner correctement le trop-plein par rapport au bassin.

### Instrumentation et automatisme

- Fixer les capteurs de niveau sur le tube de report en 32 mm.
- Identifier le brochage exact du moteur d'essuie-glace et valider son sens de rotation.
- Installer une alimentation 12 V DC et une protection adaptees au courant de demarrage du moteur tambour.
- Concevoir et integrer toute la partie intelligence et pilotage du FAT.
- Ajouter un capot transparent de fermeture avec capteur de detection d'ouverture.
- Ajouter une sonde de temperature d'eau avec implantation maintenable et representative.
- Ajouter une sonde de temperature ambiante du local avec implantation representative.
- Definir une IHM locale permettant de remonter clairement le statut du systeme.
- Prevoir une extension Wi-Fi V2 de remontee a distance de l'etat et des alarmes sans remplacement de la plateforme principale.
- Prevoir pour une V2 les notifications immediates actionnables et leur politique anti-repetition : apparition, rappel rare et retour a la normale.
- Prevoir pour une V2 une synthese quotidienne de fonctionnement et la possibilite de la desactiver.
- La pompe decoration pourra etre pilotee en V1.1/V2 par deux plages horaires maximum par jour, identiques tous les jours, avec interrupteur logiciel actif/inactif pour les longues periodes d'arret dont l'hiver.
- Definir la liste finale des alarmes indirectes et leur formulation utilisateur.
- Definir pour une V1.1 un test journalier automatique avec diagnostic.
- Definir pour une V1.1 une strategie d'indexation du tambour pour repartir l'immersion.
- Definir pour une V1.1 les statistiques de lavage pertinentes pour suivre l'etat du filtre.
- Definir pour une V1.1 ou V2 un indice simple d'encrassement exploitable dans le temps.
- Definir pour une V1.1 ou V2 une estimation empirique de la consommation d'eau du rincage et du besoin d'appoint associe.
- Definir pour une V1.1 les temps de fonctionnement cumules des organes principaux.

## Questions techniques encore ouvertes

- Cotes exactes du niveau normal cote sale, du niveau normal cote propre, du seuil de lavage et du niveau bas de securite.
- Nombre exact de capteurs CR18-8DN a installer et implantation precise de chacun.
- Couple reel, vitesse reelle, courant en charge et seuil de protection du moteur de tambour.
- Brochage exact du moteur de tambour, y compris fonctions de parking a isoler ou ignorer proprement en V1.
- Debit reel, pression utile aux buses, amorcage et courant reel de la pompe de rincage.
- Forme, nombre et surface totale des ouvertures du tambour a calculer avant decoupe pour atteindre environ 0,20 a 0,23 m2 de surface filtrante utile.
- Architecture de puissance pour separer clairement les sorties a couper des equipements hors controleur.
- Strategie de securite a appliquer si le capot est ouvert pendant un cycle.
- Position finale du capteur de temperature bassin et technologie retenue.
- Seuils d'alerte temperature eau V1 : basse < 4 deg C, haute > 28 deg C, configurables.
- Position finale du capteur de temperature ambiante local et technologie retenue, avec mesure representative de l'air du local de filtration.
- Seuils d'alerte temperature ambiante local V1 : basse < 2 deg C, haute > 40 deg C, configurables.
- Niveau d'IHM locale a retenir et informations a afficher localement.
- Nombre, couleur et signification des voyants si une signalisation lumineuse est retenue.
- Liste exacte des informations visibles en permanence sur l'IHM locale et de celles accessibles en detail.
- Canal de notification a distance V2 retenu : Wi-Fi, avec materiel MVP compatible.
- Liste des evenements a notifier et comportement en cas de perte reseau pour une V2, sur la base des evenements actionnables deja retenus.
- Activation par defaut, horaire et contenu exact de la synthese quotidienne pour une V2.
- Ne pas creer d'information distante "bassin niveau bas" sans capteur bassin distinct ; avec l'instrumentation retenue, EP_CRITIQUE doit etre presente comme `NIVEAU FAT CRITIQUE`.
- Programmation pompe decoration retenue pour V1.1/V2 : deux plages maximum par jour, identiques tous les jours, et interrupteur logiciel actif/inactif sans automatisme hiver au depart.
- Priorite pompe decoration retenue : securites hydrauliques et defauts bloquants, commande manuelle locale, commande distante, puis programmation horaire.
- Validation physique de l'ordre et de la logique des capteurs EP_LAVAGE et EP_CRITIQUE.
- Choix de rester en diagnostic indirect V1 confirme ; pressostat, debitmetre, retour courant rincage, rotation, fuite ou niveau eau sale sont reportes V1.1/V2 si les essais montrent un besoin.
- Regles de lancement, de verdict et de report du test journalier automatique en V1.1.
- Strategie retenue en V1.1 pour ne pas laisser toujours la meme zone du tambour immergee.
- Regles de calcul et de restitution des statistiques de lavage en V1.1.
- Formule retenue pour l'indice d'encrassement et usage associe en V1.1 ou V2.
- Methode empirique d'estimation de la consommation d'eau et des pertes vers evacuation en V1.1 ou V2.
- Regles de cumul et d'affichage des temps de fonctionnement en V1.1 ; remises a zero maintenance et seuils de rappel reportes V2.
