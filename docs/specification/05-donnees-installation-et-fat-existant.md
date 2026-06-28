# Donnees d'entree de l'installation et FAT existant

## Vue d'ensemble

Le projet porte sur un filtre a tambour (FAT) deja partiellement construit et integre dans une ligne de filtration de bassin. Les dimensions et interfaces ci-dessous sont considerees comme des caracteristiques d'entree stables pour la conception mecanique, hydraulique et de pilotage.

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

## Chaine de filtration autour du FAT

| Element | Valeur | Notes |
| --- | --- | --- |
| Bassin | 186 cm x 411 cm x 108 cm | Volume geometrique theorique: 8,26 m3 |
| Evacuation bassin | 1 bonde de fond 110 mm + 1 skimmer 110 mm | Fonctionnement en gravitaire vers le FAT |
| Filtration biologique | cuve IBC 1000 L avec tapis japonais | En sortie du FAT |
| Pompe de filtration principale | retour bassin en 63 mm | Fonctionnement continu vise |
| Pompe decoration | usage ponctuel | Cascade, mur d'eau, lame d'eau, etc. |
| UV | emplacement non defini | A integrer plus tard dans la chaine |

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
- Les capteurs de niveau doivent etre positionnes cote eau propre sur le tube de report en 32 mm, et non directement dans la zone sale.
- Les capteurs de niveau retenus sont des CR18-8DN en M18, sortie NPN, alimentation 12-24 VDC, cablage 3 fils.
- Le support du FAT devra fixer la cote altimetrique du trop-plein par rapport au niveau hydraulique du bassin.
- Les diametres d'interconnexion existants 110 mm, 100 mm, 63 mm et 32 mm doivent etre preserves dans les choix d'implantation et de pilotage.

## Reperes de niveau a definir

| Repere | Description | Etat |
| --- | --- | --- |
| Niveau normal cote sale | Niveau nominal observe dans la chambre eau sale en fonctionnement etabli | Hypothese de travail: environ 20 cm au-dessus du bas du tambour, soit environ 27 cm depuis le fond du filtre |
| Niveau normal cote propre | Niveau nominal observe dans la chambre eau propre et sur le report de niveau | A definir |
| Niveau de declenchement du lavage | Niveau a partir duquel le FAT doit lancer un cycle de lavage | A definir |
| Niveau bas de securite | Niveau a partir duquel l'installation passe en etat de securite | A definir |

### Contraintes sur ces reperes

- Le niveau bas de securite doit rester au-dessus du niveau minimal acceptable pour proteger pompes, UV et hydraulique.
- Le niveau de declenchement du lavage doit rester suffisamment sous le trop-plein physique a 30,5 cm pour laisser une marge de reaction.
- Les niveaux normaux cote sale et cote propre serviront de reference pour comprendre le comportement hydraulique du filtre propre.
- Ces quatre reperes devront etre convertis en cotes physiques mesurables depuis le fond du filtre et, si utile, en ecarts entre cote sale et cote propre.
- La surface de filtration utile finale dependra de la geometrie des ouvertures du tambour sous la toile inox 74 microns.

## Politique de securite en cas de niveau bas

### Equipements a couper ou inhiber

| Equipement | Action attendue | Justification |
| --- | --- | --- |
| Pompe principale de filtration | Coupure | Eviter de vider le bassin et proteger la pompe contre la marche a sec |
| Pompe decoration | Coupure | Eviter de vider le bassin et proteger la pompe contre la marche a sec |
| UV | Coupure | Eviter un fonctionnement hors d'eau sans refroidissement correct |
| Rotation du tambour | Inhibition | Eviter un cycle FAT en situation d'eau insuffisante |
| Pompe de rincage | Inhibition | Eviter un lavage inutile ou degradant en situation anormale |
| Mise a niveau automatique du bassin | Coupure | Eviter de remplir indefiniment un bassin en cas de fuite |

### Equipements a maintenir alimentes

| Equipement | Action attendue | Justification |
| --- | --- | --- |
| Bulleur cuve bio | Maintien alimente | Preserver les bacteries de filtration biologique |
| Bulleur bassin | Maintien alimente | Maintenir l'oxygenation des poissons et limiter la glace en hiver |

## Travaux restants sur le FAT existant

### Mecanique et hydraulique

- Realiser les ouvertures dans le tuyau qui sert de tambour.
- Poser la maille inox autour du tambour.
- Raccorder la rampe d'aspersion a la pompe de rincage.
- Percer une paroi du filtre pour la prise d'eau de la pompe de rincage.
- Percer le trou de vidange du filtre.
- Installer le joint a levre autour du tambour pour separer eau propre et eau sale.
- Fabriquer le support du FAT afin de positionner correctement le trop-plein par rapport au bassin.

### Instrumentation et automatisme

- Fixer les capteurs de niveau sur le tube de report en 32 mm.
- Concevoir et integrer toute la partie intelligence et pilotage du FAT.
- Ajouter un capot de fermeture avec capteur de detection d'ouverture.

## Questions techniques encore ouvertes

- Cotes exactes du niveau normal cote sale, du niveau normal cote propre, du seuil de lavage et du niveau bas de securite.
- Nombre exact de capteurs CR18-8DN a installer et implantation precise de chacun.
- Caracteristiques electriques du moteur de tambour.
- Caracteristiques electriques et debit de la pompe de rincage.
- Forme, nombre et surface totale des ouvertures du tambour pour atteindre la surface de filtration utile visee.
- Architecture de puissance pour separer clairement les sorties a couper des sorties a maintenir alimentees.
- Implantation finale de l'UV dans la chaine de filtration.
- Strategie de securite a appliquer si le capot est ouvert pendant un cycle.
