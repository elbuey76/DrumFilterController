# Checklist go/no-go materiel V1

## Objet

Cette checklist sert de verrou avant cablage final, mise en eau reelle puis exploitation autonome du controleur de filtre a tambour.

Un point non valide peut etre accepte uniquement s'il est marque comme non applicable, reporte explicitement hors V1, ou couvert par une action corrective documentee. Les points de securite critiques doivent etre passes avant exploitation sans presence humaine.

## References finales

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Plateforme controle | KC868-A32 reference finale disponible et inspectee | A faire | |
| Alimentation 12 VDC | Mean Well NDR-120-12 disponible, entree/sortie reperees | A faire | |
| Capteurs niveau | 2 x CR18-8DN disponibles, brochage confirme | A faire | |
| Pompe rincage | VEVOR / Leo EKJ-802S conservee comme reference V1 | A faire | |
| Moteur tambour | Fyearfly 12 VDC 10 rpm disponible, fixation definie | A faire | |
| Relais tambour | HELLA 4RD 933 332-551 disponible, support rail DIN defini | A faire | |
| Contacteurs auxiliaires | TOMZN TOCT1-25Z 12 VDC valides ou alternative definie | A faire | |
| Contacteur rincage | Schneider TeSys LC1D18P7 avec bobine 230 VAC valide au schema | A faire | Remplace le LC1D12P7 ; confirmer encombrement, bornes et reperage avant cablage. |
| Capot | OMRCH ME-8104 disponible, came/support definis | A faire | |
| IHM | LCD 2004, boutons, voyants et etiquettes facade definis | A faire | |
| RTC / temperature | DS3231 et 2 x DS18B20 disponibles, adresses/brochages verifies | A faire | |
| Coffret | Reference coffret choisie, IP65 cible ou IP55 justifie | A faire | |
| Borniers / presse-etoupes | References et quantites suffisantes | A faire | |
| Pieces de rechange | Fusibles ATO, au moins un capteur niveau, une sonde, un bouton, un voyant, et si possible un contacteur spare | A faire | |

## Banc electrique basse tension

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Boot securise | Toutes les sorties controlees restent OFF au boot avant autorisation | A faire | |
| Perte 12 VDC | Contacteurs et sorties controlees retombent OFF par conception | A faire | |
| Distribution 12 VDC | Fusibles 5 A moteur, 3 A automate, 1 A capteurs/boutons, 1 A IHM/accessoires coherents apres mesure | A faire | |
| CR18-8DN repos/detection | `EP_LAVAGE` et `EP_CRITIQUE` lus correctement sur KC868-A32 | A faire | |
| Rupture fils niveau | Fils marron, bleu et noir de chaque capteur testes et comportement documente | A faire | |
| Incoherence niveau | Incoherence critique declenche defaut bloquant hydraulique | A faire | |
| Contact capot | Capot ferme = boucle fermee ; capot ouvert, fil coupe ou debranche = capot ouvert | A faire | |
| Boutons / selecteur | AUTO et MAINTENANCE exclusifs ; reset, test, manuel tambour et manuel rincage lus sans confusion | A faire | |
| Voyants | Marche, lavage et alarme visibles et conformes a l'affectation V1 | A faire | |
| LCD | Adresse, contraste, lisibilite, reboot et pull-up I2C 3,3 V verifies | A faire | |
| RTC | Adresse `0x68`, conservation heure apres coupure, pull-up 3,3 V verifies | A faire | |
| I2C partage | LCD + RTC stables ensemble sur `GPIO32` / `GPIO33` pendant essai prolonge | A faire | |
| DS18B20 | Brochage reel, pull-up 4,7 kOhm, identification eau/local et perte de sonde verifies | A faire | |

## Puissance 230 VAC et coffret

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Revue externe | Schema 230 VAC relu par personne competente avant mise sous tension | A faire | |
| Protection amont | Depart maison 16 A confirme et coherent avec la liaison existante | A faire | |
| Tete tableau | Sectionneur local, differentiel 30 mA type A et disjoncteurs aval conformes au schema | A faire | |
| Separation BT/secteur | Cheminements, borniers, reperes et distances propres dans le coffret | A faire | |
| Terre | Pompe de rincage, coffret/equipements classe I et masses raccordes selon schema | A faire | |
| Contacteur rincage | Bobine 230 VAC commandee proprement par relais automate, avec reperage clair du 230 VAC de commande | A faire | |
| Contacteurs TOMZN | Consommation bobine, echauffement, bruit et coupure 12 VDC observes | A faire | |
| Pompe filtration prioritaire | Depart filtration separe du depart UV / decoration / mise a niveau | A faire | |
| Bulleurs | Bulleurs hors sorties controlees et clairement identifies comme alimentation permanente locale | A faire | |
| Condensation | Entrees cables, boucles goutte, presse-etoupes et traitement passif definis | A faire | |
| Reserve coffret | Place libre rail DIN et borniers suffisante pour maintenance et V1.1/V2 | A faire | |

## Mesures mecaniques et hydrauliques

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Courant moteur tambour | Courant a vide, en charge, au demarrage et blocage controle mesure | A faire | |
| Fusible moteur | Fusible 5 A confirme ou ajuste apres mesure | A faire | |
| Rotation tambour | Sens, vitesse, couple et absence de blocage valides en conditions reelles | A faire | |
| Transmission | Fixation et ratio reel documentes si transmission ajoutee | A faire | |
| Rincage | Jet suffisant observe sur rampe et buses reelles | A faire | |
| Debit rincage | Debit aux buses mesure ; estimation L/cycle documentee | A faire | |
| Seuils niveau | `EP_LAVAGE` et `EP_CRITIQUE` regles apres observation terrain | A faire | |
| Supports capteurs | Supports reglables, accessibles, nettoyables et reperes | A faire | |
| Tube report niveau | Tube 32 mm et event 1 mm inspectables et inclus dans maintenance | A faire | |
| Reserve troisieme capteur | Emplacement et passage cable prevus sans cablage V1 obligatoire | A faire | |

## Essais fonctionnels bloquants

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Lavage nominal | Cycle auto execute, duree mini respectee, retour niveau arrete le cycle | A faire | |
| Duree maximale | Le lavage s'arrete a la duree max configuree | A faire | |
| Tentatives max | Apres 3 tentatives inefficaces, defaut lavage et inhibition auto | A faire | |
| `EP_CRITIQUE` | Coupe ou inhibe filtration, UV, decoration, mise a niveau, tambour et rincage selon schema | A faire | |
| Reprise apres critique | Retour niveau stable puis reset local obligatoire avant reprise | A faire | |
| Capot ouvert | Tambour et rincage interdits ou coupes ; affichage explicite | A faire | |
| Commandes manuelles | Tambour et rincage manuels maintenus seulement si securites OK | A faire | |
| Test lavage | Test borne autorise sous preconditions, refuse avec cause sinon | A faire | |
| Reset refuse | Reset refuse et cause affichee si defaut encore actif | A faire | |
| Reprise secteur | Apres coupure, sorties OFF puis retour selon selecteur et securites | A faire | |
| LCD absent | Fonction automate maintenue, diagnostic degrade signale si possible | A faire | |
| RTC absente | Fonction automate maintenue avec horodatage inconnu | A faire | |
| Sonde temperature absente | Alerte informative sans inhibition lavage/filtration V1 | A faire | |

## Mise en service progressive

| Etape | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| 1. Simulation table | Entrees/sorties et IHM valides sans puissance dangereuse | A faire | |
| 2. Coffret sans charges | Alimentation, protections, boot et commandes testes charges deconnectees | A faire | |
| 3. Contacteurs | Commandes contacteurs validees sans comportement inattendu | A faire | |
| 4. Manuel terrain | Tambour et rincage testes en manuel sous presence humaine | A faire | |
| 5. Auto surveille | Plusieurs cycles auto observes sans lavage intempestif | A faire | |
| 6. Exploitation normale | Fonctionnement prolonge observe, alarmes et reprise secteur testees | A faire | |

## Exploitation et maintenance

| Point | Critere go/no-go | Statut | Preuve / commentaire |
| --- | --- | --- | --- |
| Angles morts V1 | Defauts non detectes directement documentes | A faire | |
| Fiche plastifiee | Fiche coffret avec voyants, reset, maintenance, coupure generale et alarmes critiques | A faire | |
| Procedure tiers | Une personne autre que le concepteur peut lire l'etat, couper, mettre en maintenance et demander aide | A faire | |
| Inspection periodique | Nettoyage tube niveau, event, buses, toile, capot et condensation planifie | A faire | |
| Documentation mesures | Courant, debit, seuils, essais et revue 230 VAC archives dans le dossier projet | A faire | |
