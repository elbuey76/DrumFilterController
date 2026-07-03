# Architecture logicielle

## Modules pressentis

| Module | Responsabilite |
| --- | --- |
| Entrees | Lire les capteurs, la temperature bassin, la temperature ambiante et les boutons, appliquer anti-rebond et filtrage. |
| Temporisations | Centraliser les delais, durees et timeouts. |
| Machine a etats | Decider des transitions, du mode courant et des verrouillages. |
| Diagnostics | Evaluer auto-tests, coherence EP_LAVAGE et EP_CRITIQUE, consequences hydrauliques observables, temperatures et criteres de passage en degrade ou defaut. |
| Communication distante | Option V2 : publier et notifier les etats et evenements importants vers l'exterieur. |
| Temps systeme | Option V2 : fournir une heure fiable pour horodatage, historiques, notifications et syntheses. |
| Sorties | Piloter relais, voyants, ecran local et autres actionneurs. |
| Configuration | Stocker les parametres modifiables et la politique de reprise. |
| Journalisation | Enregistrer cycles, alarmes et evenements importants. |
| Statistiques | Calculer, consolider et exposer les indicateurs de lavage a court et moyen terme. |

## Machine a etats cible

```mermaid
stateDiagram-v2
    [*] --> DEMARRAGE
    DEMARRAGE --> AUTO_ATTENTE: auto pret
    DEMARRAGE --> MAINTENANCE: capot ouvert ou demande maintenance
    DEMARRAGE --> DEGRADE: defaut degradable detecte
    DEMARRAGE --> DEFAUT: defaut critique detecte

    AUTO_ATTENTE --> LAVAGE_AUTO: demande lavage valide
    LAVAGE_AUTO --> POST_LAVAGE: niveau normal apres duree mini
    LAVAGE_AUTO --> RETRY_PAUSE: duree maxi atteinte et tentative restante
    LAVAGE_AUTO --> DEGRADE: tentatives max atteintes sans EP_CRITIQUE
    LAVAGE_AUTO --> DEFAUT: EP_CRITIQUE ou defaut critique
    POST_LAVAGE --> PAUSE_SECURITE: rotation residuelle terminee
    RETRY_PAUSE --> LAVAGE_AUTO: pause ecoulee et nouvelle tentative
    PAUSE_SECURITE --> AUTO_ATTENTE: delai ecoule

    AUTO_ATTENTE --> MANUEL: selection mode manuel
    MANUEL --> AUTO_ATTENTE: retour auto

    AUTO_ATTENTE --> TEST_LAVAGE: demande test
    TEST_LAVAGE --> AUTO_ATTENTE: test valide
    TEST_LAVAGE --> DEFAUT: test en echec
    TEST_LAVAGE --> MAINTENANCE: capot ouvert
    TEST_LAVAGE --> DEFAUT: EP_CRITIQUE ou defaut critique

    AUTO_ATTENTE --> MAINTENANCE: capot ouvert ou demande maintenance
    MANUEL --> MAINTENANCE: capot ouvert ou demande maintenance
    MAINTENANCE --> AUTO_ATTENTE: fin maintenance et tempo ecoulee

    AUTO_ATTENTE --> DEGRADE: defaut degradable
    MANUEL --> DEGRADE: defaut degradable
    DEGRADE --> AUTO_ATTENTE: defaut leve et acquitte

    AUTO_ATTENTE --> DEFAUT: defaut critique
    MANUEL --> DEFAUT: defaut critique
    MAINTENANCE --> DEFAUT: defaut critique
    DEGRADE --> DEFAUT: aggravation
    DEFAUT --> AUTO_ATTENTE: acquittement et conditions OK
```

## Regles de reprise apres coupure d'alimentation

Au demarrage, le logiciel doit :

1. initialiser les sorties dans un etat sur ;
2. relire capteurs, capot, commandes operateur et eventuels etats memorises ;
3. executer les auto-diagnostics de base ;
4. choisir le mode cible selon priorite securite puis exploitation.

Priorite de decision recommandee :

- `EP_CRITIQUE` actif ou defaut critique actif : rester en DEFAUT avec sorties protegees coupees, jusqu'au retour normal puis acquittement valide ;
- capot ouvert ou demande maintenance : entrer en MAINTENANCE ;
- `EP_LAVAGE` actif sans `EP_CRITIQUE` : entrer en DEGRADE controle, autoriser la filtration et l'UV si la filtration est autorisee, puis lancer un lavage immediat si les preconditions sont OK ;
- defaut degradable detecte : entrer en DEGRADE ;
- sinon : entrer en AUTO_ATTENTE sans attente operateur supplementaire.

Dans le cas `EP_LAVAGE` actif au demarrage, si le lavage de reprise ne ramene pas le niveau a l'etat normal, le logiciel doit inhiber les nouveaux lavages automatiques, maintenir l'alarme et conserver filtration + UV tant que `EP_CRITIQUE` reste absent et que le bypass passif assure le passage vers la biofiltration.

Apres un EP_CRITIQUE confirme, la reprise doit se faire en deux temps : retour niveau normal stable et acquittement local valide, puis redemarrage de la filtration, puis reautorisation de l'UV apres une courte temporisation de stabilisation hydraulique.

En fonctionnement, `CAPOT_OUVERT` reste prioritaire sur le selecteur `AUTO / MAINTENANCE` : un capot ouvert force l'etat maintenance ou securite meme si le selecteur est physiquement sur AUTO. Le capot est le couvercle transparent unique du FAT, cable en normalement ferme : capot ferme = contact ferme ; fil coupe, connecteur debranche ou capot ouvert = `CAPOT_OUVERT`.

Le filtrage du capot doit etre asymetrique : ouverture confirmee rapidement apres anti-rebond court de 100 a 500 ms, fermeture stable pendant 1 a 2 s avant reautorisation des commandes dangereuses ou reprise automatique.

Capot ouvert hors action dangereuse, l'IHM doit afficher l'etat informatif permanent `MAINTENANCE - CAPOT OUVERT`. Cet etat ne demande pas d'acquittement. Apres fermeture stable, le controleur revient automatiquement au mode demande par le selecteur si aucune alarme bloquante capot dangereux n'a ete creee. Si le capot s'est ouvert pendant une action dangereuse, la fermeture ne suffit pas : l'acquittement reste requis.

Si le capot reste ouvert au-dela d'une temporisation configurable, valeur initiale 10 minutes, l'IHM doit afficher `A15 - CAPOT OUVERT LONG` et allumer `VOYANT_ALARME` rouge fixe, sans clignotement en V1. Cette alerte signale un oubli probable et rappelle que le lavage tambour est indisponible tant que le capot reste ouvert. Elle n'ajoute pas de blocage supplementaire : le blocage provient deja de l'etat `CAPOT_OUVERT`. Elle disparait automatiquement apres fermeture stable du capot, sans acquittement ni maintien artificiel du voyant rouge.

Le logiciel conserve une trace minimale persistante et non bloquante de A15. Cette trace est ecrite quand A15 survient et doit rester presente apres coupure d'alimentation. La V1 conserve au minimum un compteur persistant et, si c'est simple, le dernier evenement. Si une horloge fiable existe facilement en MVP, le dernier evenement est horodate ; sinon le compteur persistant suffit. Une memoire circulaire courte de quelques evenements recents est acceptable, mais un historique long n'est pas requis en V1. Au redemarrage, le logiciel relit le capot : si A15 etait actif avant coupure et que le capot est encore ouvert, A15 est reaffiche apres lecture stable ; si le capot est ouvert mais qu'A15 n'etait pas encore actif, l'etat informatif `MAINTENANCE - CAPOT OUVERT` est affiche et la temporisation A15 repart.

L'arret total n'apparait pas comme un etat logiciel de la machine a etats ci-dessus. Il correspond a une consignation ou a une coupure electrique maitrisee, explicite pour l'exploitation, et geree hors du cycle logiciel nominal.

## Capteurs de reference cote eau propre

La logique V1 repose sur une cote simple cote eau propre, sans comparaison automatisee avec un niveau cote eau sale.

| Capteur | Role logique |
| --- | --- |
| EP_LAVAGE | Demande de lavage par niveau eau propre abaisse |
| EP_CRITIQUE | Danger hydraulique, risque pompe a sec et arret de securite |

## Limites de diagnostic en V1

Sans capteur supplementaire, la V1 ne prouve pas directement :

- un tambour bloque ;
- une pompe de rincage HS ;
- une pression ou un debit reel de rincage absent ;
- une pompe filtration reellement branchee ou debiteuse ;
- une pompe decoration reellement branchee ou debiteuse ;
- un UV reellement allume ;
- une fuite local ;
- un niveau haut cote eau sale ;
- une pompe a air HS.

Le logiciel diagnostique donc surtout les consequences visibles cote eau propre et les incoherences de commande.

## Principe de diagnostic indirect

La logique de diagnostic devrait suivre cette regle simple :

- nommer d'abord l'effet observe ;
- associer ensuite des causes probables a verifier ;
- ne conclure a une panne d'organe que si un retour d'etat dedie est ajoute plus tard.

Exemples de libelles preferes :

- niveau eau propre anormal ;
- lavage inefficace ;
- risque pompe a sec ;
- cycle de lavage incoherent ;
- temperature anormale ;
- capot ouvert ;
- commande incoherente.

Les auto-diagnostics indirects obligatoires V1 sont : EP_CRITIQUE, incoherence EP_CRITIQUE actif avec EP_LAVAGE inactif, lavage inefficace apres 3 tentatives, capot ouvert dangereux, capot ouvert trop longtemps A15, commande UV incoherente, perte sonde temperature eau/local. Les diagnostics absence anormale de lavage, lavage trop frequent, moteur tambour bloque, pompe de rincage HS, pression absente, fuite local et niveau eau sale sont reportes V1.1 ou V2.

La V1 n'ajoute pas de capteur dedie pour diagnostiquer directement rotation tambour, courant mesure, fuite local ou niveau eau sale. Les protections materielles restent obligatoires ; un simple contact defaut fourni par un module de protection peut etre lu s'il existe naturellement, sans devenir une mesure detaillee de cause. Les fonctions de parking du moteur d'essuie-glace sont ignorees ou isolees proprement en V1, sauf contrainte de brochage simple ; leur usage pour l'indexation est reporte V1.1.

## Gestion des modes

| Mode | Autorisations principales | Interdictions principales |
| --- | --- | --- |
| AUTO_ATTENTE | Surveillance et lavage automatique | Commandes manuelles directes |
| MANUEL | Commande individuelle des sorties | Contournement des verrouillages critiques |
| MAINTENANCE | Arret propre, intervention humaine, tests limites | Demarrage automatique tambour et lavage auto |
| DEGRADE | Fonctionnement restreint pour maintien de vie du bassin | Retour silencieux au nominal sans acquittement |
| TEST_LAVAGE | Cycle complet automatique borne sous supervision | Relances multiples ou usage si preconditions non remplies |
| DEFAUT | Affichage, alarme, acquittement | Actionnement non securise des organes |

## Commandes manuelles et test V1

Les commandes `MANU_TAMBOUR` et `MANU_RINCAGE` sont des commandes a action maintenue. La sortie correspondante n'est active que tant que le bouton est maintenu et que les interverrouillages restent valides. Les deux commandes sont refusees capot ouvert ; ce refus preventif affiche un message local simple sans creer d'alarme bloquante si aucune sortie dangereuse n'a demarre.

Le bouton `TEST_LAVAGE` lance un seul cycle complet autonome et borne apres appui bref si les preconditions sont satisfaites. Il est autorise en AUTO et en MAINTENANCE si le capot est ferme, EP_CRITIQUE absent, les capteurs de niveau coherents et aucun defaut critique bloquant actif. Ce cycle est interrompu immediatement par capot ouvert, EP_CRITIQUE ou defaut critique.

Le test ne reprend pas la strategie de relances multiples du lavage automatique. Si EP_LAVAGE est inactif au depart, le verdict cible est `TEST OK - CYCLE EXECUTE` si le cycle s'execute sans securite critique ; il ne faut pas afficher `lavage efficace`. Si EP_LAVAGE est actif au depart, le verdict cible est `TEST OK - NIVEAU OK` si EP_LAVAGE revient normal, sinon `TEST ECHEC - EP_LAVAGE ACTIF` ou `TEST ECHEC - LAVAGE INEFFICACE`. Le test seul ne declare pas un defaut lavage maintenu.

Une demande de test avec capot ouvert est refusee sans mouvement avec `A13 - TEST REFUSE CAPOT`. Une demande avec EP_CRITIQUE actif, capteurs de niveau incoherents ou defaut critique bloquant est refusee avec `A14 - TEST REFUSE SECURITE`.

## Statuts a remonter localement

L'IHM locale V1 retient un ecran texte ou petit afficheur. Elle doit presenter au minimum :

- mode actif : auto, manuel, maintenance, degrade ou defaut ;
- etat niveau : OK, lavage ou critique ;
- etat lavage : repos, cycle ou inhibe ;
- presence d'une alarme active ;
- temperature eau ;
- message ou code de defaut si l'ecran le permet.

La priorite d'affichage des alarmes V1 est :

1. EP_CRITIQUE ou capteurs niveau incoherents ;
2. capot ouvert pendant action dangereuse ;
3. defaut lavage maintenu ;
4. capot ouvert trop longtemps (`A15`) ;
5. alertes temperature ;
6. informations de fonctionnement.

En cas de reset refuse, l'ecran doit afficher une cause courte et explicite, par exemple `RESET REFUSE - EP_CRITIQUE ACTIF`, `RESET REFUSE - EP_LAVAGE ACTIF` ou `RESET REFUSE - CAPTEURS INCOHERENTS`.

Le bypass passif n'est pas instrumente en V1. L'IHM ne doit donc pas afficher un etat mesure `BYPASS ACTIF`; elle doit preferer `MODE DEGRADE - BYPASS SUPPOSE` lorsque le lavage est inefficace sans EP_CRITIQUE et que la filtration reste maintenue.

Les alarmes V1 doivent utiliser le format `Axx - MESSAGE COURT`. Liste minimale initiale :

| Code | Message |
| --- | --- |
| A01 | NIVEAU CRITIQUE |
| A02 | CAPTEURS NIVEAU INCOHERENTS |
| A03 | CAPOT OUVERT DANGER |
| A04 | LAVAGE INEFFICACE |
| A05 | RESET REFUSE |
| A06 | TEMP EAU BASSE |
| A07 | TEMP EAU HAUTE |
| A08 | TEMP LOCAL BASSE |
| A09 | TEMP LOCAL HAUTE |
| A10 | MODE DEGRADE - BYPASS SUPPOSE |
| A11 | SONDE EAU ABSENTE |
| A12 | SONDE LOCAL ABSENTE |
| A13 | TEST REFUSE CAPOT |
| A14 | TEST REFUSE SECURITE |
| A15 | CAPOT OUVERT LONG |

Les voyants physiques restent des complements de lecture rapide : `VOYANT_MARCHE` vert et `VOYANT_ALARME` rouge sont retenus en V1. `VOYANT_ALARME` s'allume fixe pour les alarmes actives et pour `A15 - CAPOT OUVERT LONG`. Aucun clignotement n'est retenu en V1. `VOYANT_LAVAGE` jaune ou ambre reste optionnel si le cablage est simple ; l'ecran reste la source du detail.

## Donnees utiles a presenter localement

L'IHM locale devrait idealement pouvoir presenter ou rendre accessibles :

- mode actuel ;
- etat lavage, repos ou defaut ;
- niveau eau propre : OK, bas ou critique ;
- heure du dernier lavage ;
- nombre de lavages aujourd'hui ;
- defaut actif ;
- temperature eau ;
- temperature local ;
- etat pompe principale ;
- etat pompe decoration ;
- etat UV.

## Statistiques de lavage V1.1 a consolider

Les statistiques avancees sont reportees en V1.1. Le logiciel devrait alors consolider au minimum :

- nombre de lavages par heure ;
- nombre de lavages par jour ;
- duree moyenne d'un lavage ;
- duree totale de lavage par jour ;
- intervalle moyen entre lavages ;
- intervalle minimum entre lavages ;
- tendance glissante sur 7 jours ;
- tendance glissante sur 30 jours.

## Indicateurs de consommation d'eau V1.1 ou V2

Ces indicateurs sont reportes en V1.1 ou V2. Le logiciel pourrait consolider au minimum :

- litres par lavage ;
- litres par jour ;
- litres par semaine ;
- litres perdus vers evacuation ;
- estimation du remplissage necessaire.

Ces indicateurs sont estimes empiriquement a partir des essais : `volume estime = debit mesure aux buses x duree de rincage cumulee`. Les pertes vers evacuation et le besoin de remplissage restent indicatifs tant qu'aucun compteur d'eau dedie n'est ajoute.

## Temps de fonctionnement V1.1 a consolider

Les compteurs detailles sont reportes en V1.1. La cible V1.1 est un compteur cumule simple par organe principal, sans remise a zero maintenance ni seuil de rappel. Le logiciel devrait alors cumuler au minimum :

- heures moteur tambour ;
- heures pompe rincage ;
- heures pompe decoration ;
- heures pompe principale ;
- heures UV.

## Indicateur derive d'encrassement V1.1 ou V2

L'indice d'encrassement est reporte en V1.1 ou V2. Un indicateur derive simple candidat est :

`Indice encrassement = nombre de lavages par heure x duree moyenne lavage`

Cette formule est retenue comme indicateur experimental stable afin de comparer les periodes. Elle ne declenche aucune action automatique. Les alertes futures devront reposer sur une derive relative apres observation, par exemple un doublement par rapport a la mediane recente ou une hausse continue sur plusieurs jours.

Cet indicateur peut aider a suivre :

- la charge du bassin ;
- le colmatage de la toile ;
- la baisse d'efficacite du rincage ;
- les effets de debit ou de saison.

## Sequencement cible du lavage

Le moteur tambour et la pompe de rincage sont commandes ensemble au debut de chaque tentative. Le logiciel doit ensuite :

1. confirmer la demande de lavage par un retard EP_LAVAGE configurable, cible initiale 5 a 15 s ;
2. lancer une tentative et imposer une duree mini ;
3. verifier le retour au niveau normal ;
4. soit conclure avec rotation residuelle 2 a 5 s puis anti-redemarrage 30 a 120 s ;
5. soit poursuivre jusqu'a duree maxi ;
6. soit relancer apres une courte pause si des tentatives restent ;
7. soit declarer un defaut lavage, inhiber le lavage automatique et maintenir la pompe principale si EP_CRITIQUE est absent et qu'un bypass hydraulique maintient la biofiltration.

Si EP_LAVAGE redevient actif pendant l'anti-redemarrage apres lavage reussi, le logiciel ne doit pas relancer immediatement. Il attend la fin de l'anti-redemarrage, puis relit EP_LAVAGE avec le retard configure avant de lancer un nouveau lavage si la demande persiste.

## Logique de defaut hydraulique recommandee

La logique d'etat observable recommandee est la suivante :

1. EP_LAVAGE = 0 et EP_CRITIQUE = 0 : fonctionnement normal ;
2. EP_LAVAGE = 1 et EP_CRITIQUE = 0 : demande de lavage ;
3. apres lavage, EP_LAVAGE retourne a 0 : lavage reussi ;
4. apres lavage, EP_LAVAGE reste a 1 : lavage douteux puis relance selon tentatives restantes ; apres tentatives max, defaut lavage maintenu, lavages automatiques inhibes et reset refuse tant que EP_LAVAGE reste actif ;
5. EP_CRITIQUE = 1 apres anti-rebond tres court, cible initiale 0,5 a 2 s : defaut critique, arret filtration et UV ;
6. EP_CRITIQUE = 1 alors que EP_LAVAGE = 0 : capteurs incoherents, defaut bloquant hydraulique et mise en securite complete des sorties protegees.

La philosophie capteur niveau est prudente : une perte de confiance dans EP_CRITIQUE rend le systeme bloquant hydraulique, alors qu'un doute limite a EP_LAVAGE interdit les lavages automatiques mais peut maintenir filtration et UV si EP_CRITIQUE reste sain.

## Fonctions periodiques recommandees apres V1

Ces fonctions sont hors V1 stricte. Le logiciel devrait pouvoir gerer plus tard deux fonctions periodiques :

1. un test journalier automatique du lavage avec verdict diagnostique, en V1.1 ;
2. une indexation du tambour pour modifier la zone immergee au repos, en V1.1.

Ces fonctions doivent rester secondaires par rapport aux verrouillages de securite et au mode courant.

## Gestion programmee de la pompe decoration V1.1 ou V2

Cette fonction est hors V1 stricte. Le logiciel peut aussi gerer plus tard une programmation horaire simple de la pompe decoration. Cette fonction devrait :

- verifier si la pompe decoration est globalement autorisee ;
- verifier si l'heure courante est dans une des deux plages actives maximum, identiques tous les jours ;
- appliquer ensuite les securites globales avant autorisation de sortie ;
- appliquer les memes securites hydrauliques que la pompe principale, la pompe decoration aspirant au meme endroit ;
- permettre une inhibition simple actif/inactif pour l'hiver ou une longue periode d'arret, sans automatisme saisonnier au depart.

La priorite de commande retenue est : securites hydrauliques et defauts bloquants, commande manuelle locale, commande distante, puis programmation horaire.

## Evenements candidats a remonter a distance en V2

La supervision distante devrait au minimum pouvoir traiter :

- entree en defaut critique ;
- passage en degrade ;
- niveau eau propre critique ;
- capteurs niveau incoherents ;
- lavage inefficace critique ;
- absence anormale de lavage ;
- commande UV incoherente ;
- alarme temperature eau ou temperature ambiante ;
- echec du test journalier automatique, a partir de la V1.1 ;
- redemarrages frequents de l'automate ;
- retour a un etat nominal apres incident ;
- perte puis retour de la connectivite distante si cette information est disponible.

## Notifications immediates candidates en V2

Une premiere version simple peut envoyer immediatement au minimum :

- `EP_CRITIQUE` ;
- capteurs niveau incoherents ;
- capot ouvert en situation dangereuse ;
- capot ouvert trop longtemps (`A15`) ;
- lavage inefficace ;
- retour courant apres coupure ;
- perte sonde temperature persistante.

Le canal cible V2 est le Wi-Fi. Le MVP ne doit pas implementer la connectivite active, mais la plateforme retenue doit pouvoir recevoir cette fonction sans remplacement materiel principal. Ethernet n'est pas disponible sur site, BLE seul est insuffisant en portee et SMS n'est pas retenu par defaut.

La consultation distante V2 doit commencer par un etat simple : etat courant, derniere alarme, dernier lavage, temperature eau/local et dernier redemarrage. L'historique detaille est reporte V2.1.

## Synthese quotidienne candidate en V2

Si la fonction est activee, le logiciel devrait pouvoir generer une synthese quotidienne contenant au minimum :

- statut global du filtre ;
- nombre de lavages du jour ;
- duree moyenne de lavage ;
- volume d'eau estime ou mesure consomme ;
- dernier defaut connu ;
- temperature eau.

Cette synthese doit rester optionnelle et configurable independamment des notifications immediates.

La synthese quotidienne V2 est desactivee par defaut. Si elle est activee, l'horaire par defaut est 18h00 et le canal est le meme que celui des notifications Wi-Fi V2. Les notifications immediates restent independantes.

L'etat distant ne doit pas exposer un champ `BASSIN NIVEAU BAS` sans capteur bassin distinct. Avec l'instrumentation retenue, `EP_CRITIQUE` est affiche comme `NIVEAU FAT CRITIQUE`.

## Parametres configurables

- duree de lavage ;
- duree de lavage mini ;
- duree de lavage mini, cible initiale 10 s ;
- duree de lavage maxi, cible initiale 45 s ;
- temps rotation apres rincage, cible initiale 2 a 5 s ;
- delai minimal entre cycles / anti-redemarrage, cible initiale 30 a 120 s ;
- duree maximale de marche continue ;
- nombre maximal de cycles dans une fenetre de temps ;
- nombre maximum de tentatives de lavage, cible initiale 3 ;
- pause entre tentatives, cible initiale 30 a 120 s ;
- retard EP_LAVAGE, cible initiale 5 a 15 s ;
- confirmation EP_CRITIQUE, cible initiale 0,5 a 2 s ;
- temps de confirmation defaut non critique ;
- tempo redemarrage pompe principale ;
- tempo stabilisation UV apres reprise filtration ;
- temps capot ouvert avant alarme ;
- duree anormale sans lavage ;
- seuil redemarrages automate sur une periode ;
- duree maximale de commande continue par sortie ;
- regles de calcul des statistiques, V1.1 ;
- regle de calcul stable de l'indice d'encrassement, V1.1 ou V2 ;
- regle empirique de calcul de la consommation d'eau, debit mesure aux buses x duree de rincage cumulee, V1.1 ou V2 ;
- debit de rincage de reference mesure aux buses apres montage reel ;
- eventuel retour pressostat, debitmetre ou courant pompe rincage reporte V1.1/V2 si les essais montrent trop d'ambiguite ;
- regles de cumul des temps de fonctionnement ;
- profondeur historique 7 jours et 30 jours, V1.1 ;
- heure ou fenetre du test journalier, V1.1, par defaut 11h-15h ;
- timeout du test journalier, V1.1 ;
- duree courte d'indexation tambour tant qu'aucun capteur de position n'est retenu ;
- frequence d'indexation du tambour ;
- logique de declenchement ;
- tempo de reprise apres maintenance ;
- politique de reprise apres coupure d'alimentation ;
- activation ou non des extensions de journalisation ;
- criteres de passage en degrade ;
- liste des alarmes indirectes actives ;
- seuils d'alerte temperature eau basse et haute, cibles initiales < 4 deg C et > 28 deg C ;
- seuils d'alerte temperature local basse et haute, cibles initiales < 2 deg C et > 40 deg C ;
- comportement des voyants, couleurs et clignotements ;
- politique d'envoi des notifications distantes ;
- temporisation anti-repetition des notifications ;
- activation ou non de la synthese quotidienne, desactivee par defaut en V2 ;
- heure d'envoi de la synthese quotidienne, defaut 18h00 si activee ;
- contenu exact de la synthese quotidienne ;
- liste des notifications immediates actives ;
- activation globale pompe decoration programmee, interrupteur actif/inactif ;
- liste des tranches horaires pompe decoration, deux plages maximum par jour identiques tous les jours ;
- inhibition saisonniere ou hivernale pompe decoration par interrupteur logiciel, sans automatisme au depart ;
- activation ou non du mode hiver.

## Points d'attention pour le firmware

- Distinguer clairement les defauts critiques des defauts degradables.
- Eviter qu'une coupure secteur ne remette les sorties en marche sans reevaluation des securites.
- Conserver les logs persistants deja ecrits avant coupure, notamment A15.
- Implementer en V1 un mini-journal persistant court : compteurs par code d'evenement plus dernier evenement global, ou memoire circulaire 8/16 evenements si simple.
- Persister au minimum A15, EP_CRITIQUE, capot ouvert dangereux, lavage inefficace, redemarrage apres coupure et acquittements reussis des alarmes bloquantes.
- Ne pas journaliser un par un les refus repetitifs d'acquittement en V1 ; afficher seulement le dernier refus localement avec sa cause.
- Au redemarrage, redetecter l'etat courant du capot et reafficher A15 si l'alerte etait deja active avant coupure et que le capot est encore ouvert.
- Centraliser les interverrouillages pour qu'ils s'appliquent de la meme facon en auto, manuel et test.
- Garder le mode test separe du mode manuel afin de pouvoir valider un cycle complet borne avec verdict automatique.
- Traiter le capot ouvert comme prioritaire sur le selecteur AUTO / MAINTENANCE.
- Coder `CAPOT_OUVERT` comme entree normalement fermee fail-safe avec temporisation asymetrique ouverture/fermeture.
- Afficher `MAINTENANCE - CAPOT OUVERT` tant que le capot est ouvert hors action dangereuse, puis revenir au mode demande apres fermeture stable si aucune alarme bloquante n'existe.
- Declencher `A15 - CAPOT OUVERT LONG` si le capot reste ouvert au-dela de la temporisation configuree, valeur initiale 10 minutes, allumer `VOYANT_ALARME` rouge fixe, puis effacer A15 automatiquement apres fermeture stable sans maintien artificiel du voyant.
- Conserver une trace minimale persistante et non bloquante de A15, sous forme de compteur persistant plus dernier evenement si simple ; horodater le dernier evenement seulement si une horloge fiable existe facilement en MVP.
- Ne pas imposer l'implementation d'une horloge fiable en MVP, mais choisir une plateforme qui permet cette fonction en V2 sans remplacement materiel principal et sans dependance exclusive a Internet.
- Commander le tambour et le rincage manuel uniquement tant que les boutons dedies restent maintenus.
- Refuser `MANU_TAMBOUR` et `MANU_RINCAGE` capot ouvert sans alarme bloquante si aucune sortie dangereuse n'a demarre.
- Autoriser le test lavage en AUTO et MAINTENANCE sous preconditions, puis l'interrompre immediatement sur capot ouvert, EP_CRITIQUE ou defaut critique.
- Ne pas emettre de diagnostic du type tambour bloque ou pompe HS sans capteur ou retour de marche dedie.
- Traiter la perte de mesure temperature eau comme une alerte explicite `A11 - SONDE EAU ABSENTE`, distincte d'une temperature simplement hors plage.
- Traiter la perte de mesure temperature ambiante comme une alerte explicite `A12 - SONDE LOCAL ABSENTE`, distincte d'une ambiance simplement hors plage.
- Concevoir la supervision distante comme une fonction additionnelle qui ne bloque jamais le fonctionnement local.
- Distinguer dans la configuration les notifications immediates et la synthese quotidienne.
- Journaliser si une notification immediate a ete emise ou supprimee par anti-repetition pour aider au diagnostic.
- Appliquer la priorite pompe decoration retenue : securites hydrauliques et defauts bloquants, commande manuelle locale, commande distante, puis programmation horaire.
- Conditionner le comportement de la pompe decoration a son point reel d'aspiration si elle peut contribuer a vider une zone sensible.
- Memoriser le compteur de tentatives et le compteur de lavages par heure et par jour pour les diagnostics.
- Memoriser le temps de retour EP_LAVAGE a l'etat normal, le nombre d'activations de EP_CRITIQUE, les ouvertures capot et leur duree.
- Garder une definition stable des statistiques pour pouvoir comparer les tendances dans le temps.
- Garder une definition stable de l'indice d'encrassement pour que sa derive reste interpretable.
- Indiquer explicitement que la consommation d'eau de rincage est estimee empiriquement.
- Garantir que les compteurs de temps de fonctionnement restent coherents apres redemarrage ou coupure.
- Empiler proprement les planifications periodiques apres V1 pour que test journalier et indexation ne perturbent pas la logique de lavage nominale.
- Pour le test journalier V1.1, distinguer un cycle execute OK d'un retour hydraulique OK lorsque EP_LAVAGE etait actif au depart.
- Pour l'indexation V1.1 sans capteur de position, raisonner en duree courte configurable et ne pas afficher d'angle atteint.
- Separer les statistiques nominales des tests, echecs et interruptions afin de garder des moyennes exploitables.
