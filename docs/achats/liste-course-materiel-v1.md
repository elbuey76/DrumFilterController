# Liste de course materiel V1

## Objet

Cette liste consolide les achats a prevoir pour commander le materiel du controleur FAT V1. Elle est basee sur l'architecture materielle, la table d'entrees/sorties, les ADR V1 et la checklist go/no-go.

Elle distingue :

- `A commander` : materiel necessaire si non deja en stock ;
- `A verifier avant commande` : reference, longueur ou dimension a confirmer ;
- `Spare recommande` : piece de rechange utile pour depannage et essais.
- `Commande 2026-07-06` : article commande, attente de reception puis controle banc ou inspection ;
- `Deja disponible` : article annonce en stock, inspection et validation encore necessaires avant montage.

Les circuits 230 VAC doivent etre finalises sur schema et relus par une personne competente avant mise sous tension.

Le journal de commande detaille est dans [commandes-2026-07-06.md](commandes-2026-07-06.md).

## 1. Controle, alimentation 12 VDC et distribution

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Carte de controle cible | KC868-A16 ESP32 classique REV.1.6.3 a sorties MOSFET | Deja disponible, validation banc en cours | Carte recue le 2026-07-22 ; scan interne `0x21/0x22/0x24/0x25` et demarrage safe documentes. Bornes physiques : entrees `X1-X16`, sorties `Y1-Y16`. |
| 1 | Ancienne carte de controle | KC868-A32 / variante A32G | Abandonnee pour le FAT | Commandee le 2026-07-06 pour 121,99 EUR puis retiree de la conception cible car surdimensionnee ; retour, revente ou reutilisation a decider. |
| 1 | Alimentation rail DIN | Mean Well NDR-120-12, 12 VDC, 120 W, 10 A | Commande 2026-07-06 | Alimente automate, capteurs, IHM, contacteurs 12 VDC et moteur via fusibles. |
| 1 | Porte-fusibles DC 4 departs | ATO 4 emplacements, idealement rail DIN natif | Commande 2026-07-06 | Variante 6 voies commandee ; si modele non DIN, prevoir adaptateur imprime 3D. |
| 5 | Fusibles ATO 5 A | Depart moteur tambour | Commande 2026-07-06 | Quantites et calibres reels a verifier a reception. |
| 5 | Fusibles ATO 3 A | Depart automate | Commande 2026-07-06 | Quantites et calibres reels a verifier a reception. |
| 10 | Fusibles ATO 1 A | Departs capteurs/boutons et IHM/accessoires | Commande 2026-07-06 | Quantites et calibres reels a verifier a reception. |

## 2. Capteurs et instrumentation

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 3 | Capteurs de niveau | CR18-8DN, NPN, 12-24 VDC, M18, 3 fils | Deja disponible | 2 installes : `EP_LAVAGE`, `EP_CRITIQUE`. Le 3e est le spare recommande ; valider l'interface KC868-A16 avant cablage definitif. |
| 2 | Supports reglables capteurs M18 | Equerres, colliers ou support maison reglable en hauteur | A verifier avant commande | Pour tube de report niveau 32 mm ; prevoir nettoyage et reglage accessibles. |
| 1 | Reserve support 3e capteur | Support mecanique ou emplacement libre | A verifier avant commande | Reserve V1.1/V2, pas de cablage V1 obligatoire. |
| 1 | Fin de course capot | OMRCH ME-8104, momentane, contact 1NO1NC | Commande 2026-07-06 | Montage fail-safe : capot ferme = boucle fermee ; quantite et bornes a verifier. |
| 1 | Came / patte de capot | Patte reglable pour actionner le ME-8104 | A verifier avant commande | Peut etre fabriquee localement. |
| 3 | Sondes temperature | DS18B20 etanche inox, 3 fils, longueur cible 3 m | Deja disponible | 2 installees : eau + local. Le 3e est le spare recommande ; verifier le brochage et la longueur de cable. |
| 10 | Resistances | 4,7 kOhm, 1/4 W | Deja disponible | Pull-up bus 1-Wire vers 3,3 V. |
| 1 | Module RTC | DS3231 I2C compatible 3,3 V, batterie rechargeable incluse | Commande 2026-07-06 | Adresse attendue `0x68`, pull-up I2C 3,3 V a verifier. |

## 3. Interface locale en facade

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Ecran LCD | LCD 2004 / 20x4 I2C, compatible 3,3 V, fond bleu | Commande 2026-07-06 | Adresse probable `0x27` ou `0x3F`; verifier absence de pull-up 5 V. |
| 1 | Selecteur AUTO / MAINTENANCE | Type XB2-BD25 ou equivalent, 22 mm, 2 positions maintenues, 1NO + 1NC | Commande 2026-07-06 | Deux entrees : `MODE_AUTO` et `MODE_MAINTENANCE`. |
| 1 | Bouton reset | 22 mm momentane, bleu, 1NO1NC | Commande 2026-07-06 | `RESET_ALARME`. |
| 1 | Bouton test | 22 mm momentane, jaune, 1NO1NC | Commande 2026-07-06 | `TEST_LAVAGE`. |
| 2 | Boutons manuel | 22 mm momentanes, noirs, 1NO1NC | Commande partielle 2026-07-06 | `MANU_TAMBOUR` et `MANU_RINCAGE`, etiquettes obligatoires ; verifier si un second noir est inclus ou a commander. |
| 1 | Bouton de rechange | 22 mm momentane, 1NO1NC, couleur au choix hors rouge/vert si possible | Spare recommande | Piece de depannage. |
| 1 | Voyant marche | LED etanche 16 mm, vert, 12 VDC | Commande 2026-07-06 | `VOYANT_MARCHE`. |
| 1 | Voyant alarme | LED etanche 16 mm, rouge, 12 VDC | Commande 2026-07-06 | `VOYANT_ALARME`. |
| 1 | Voyant lavage | LED etanche 16 mm, jaune, 12 VDC | Commande 2026-07-06 | `VOYANT_LAVAGE`. |
| 1 | Voyant de rechange | LED etanche 16 mm, 12 VDC, rouge ou jaune | Spare recommande | Choisir une couleur utile au depannage. |
| 1 lot | Etiquettes facade | AUTO, MAINTENANCE, RESET ALARME, TEST LAVAGE, MANU TAMBOUR, MANU RINCAGE, MARCHE, ALARME, LAVAGE | A commander | Preferer etiquettes gravees/plastifiees durables. |

## 4. Commande de puissance et actionneurs

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Motorreducteur tambour | Fyearfly 12 VDC, 10 rpm | Commande 2026-07-06 | Mesurer courant a vide, en charge, demarrage et blocage. |
| 1 | Relais moteur tambour | HELLA 4RD 933 332-551, 12 V, charge inductive 15 A | Commande 2026-07-06 | Support rail DIN imprime 3D realise et fonctionnel. |
| 1 | Support rail DIN relais HELLA | Piece imprimee 3D ou support compatible | Fabrique et fonctionnel | Maintien mecanique, acces cosses et reperage valides. |
| 1 | Pompe de rincage | VEVOR / Leo EKJ-802S, 220-240 VAC | Deja disponible | Pompe classe I, terre obligatoire, debit reel a mesurer sur rampe. |
| 1 | Relais d'interface pompe rincage | Bobine 12 VDC < 500 mA, support rail DIN, contact cible AC-15 >= 1 A sous 230 VAC, suppression de bobine | A choisir avant commande | La sortie MOSFET A16 commande la bobine 12 VDC ; le contact du relais commande la bobine 230 VAC du LC1D18P7. |
| 1 | Contacteur pompe rincage | Schneider Electric TeSys LC1D18P7, 3P, AC-3 18 A, bobine 230 VAC | Commande 2026-07-06 | Remplace le LC1D12P7 a prix equivalent : meme bobine 230 VAC, marge moteur superieure. Commande via relais d'interface 12 VDC / 230 VAC ; separation BT/secteur a soigner. |
| 4 | Contacteurs auxiliaires 12 VDC | TOMZN TOCT1-25Z, 25 A, bobine 12 VDC | Commande 2026-07-06 | Filtration, UV, decoration, mise a niveau. |
| 1 | Contacteur auxiliaire de rechange | TOMZN TOCT1-25Z 12 VDC ou alternative validee | Spare recommande | Si budget/place disponible. |

## 5. Protections et tableau 230 VAC

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Coupure generale locale | Schneider Electric TeSys VCDN20, interrupteur-sectionneur 3P, 690 V, 20 A, poignee rouge cadenassable | Deja disponible | Sectionneur annonce en stock ; verifier reference exacte, etat, calibre, poles, verrouillage et compatibilite coffret. |
| 1 | Interrupteur differentiel | 2P, 30 mA, 40 A, type A | A commander | Reference finale a choisir dans la gamme tableau retenue. |
| 1 | Disjoncteur alimentation 12 V | 4 A courbe C | A commander | Depart Mean Well NDR-120-12. |
| 1 | Disjoncteur pompe rincage | 6 A courbe C | A commander | Pompe EKJ-802S de courant nominal 3,5 A, commandee via contacteur Schneider. Le 6 A C laisse la marge necessaire au demarrage et protege plus finement le depart qu'un 10 A. |
| 1 | Disjoncteur prises local | 16 A courbe C | A commander | Bulleurs et prises maintenance, hors controleur. |
| 1 | Disjoncteur pompe filtration | 6 A courbe C | A commander | Depart separe organe essentiel. |
| 1 | Disjoncteur UV / decoration / mise a niveau | 6 A courbe C | A commander | Depart separe de la filtration. |
| 1 | Disjoncteur eclairage exterieur | 6 A courbe C | A commander | Depart dedie, hors automatisme FAT. |
| 8 | Prises 16 A 2P+T | Adaptees au local humide, IP44 minimum et IP55 prefere | A commander | 4 prises commandees : rincage, filtration, decoration, UV. 4 prises permanentes : 2 bulleurs et 2 maintenance. Les appareils a fiche europeenne ne sont pas recables. |
| 1 | Disjoncteur amont maison | 16 A dedie, compatible avec le tableau maison et sa gamme | A commander et faire installer | Le cable maison-local est present en attente et confirme en 3G2,5 mm2, mais aucun depart n'est actuellement protege au tableau maison. Ajouter ce disjoncteur avant tout raccordement du coffret FAT. |
| 1 lot | Peignes / pontages / obturateurs | Compatibles avec la gamme de disjoncteurs choisie | A commander | Garder une gamme coherente pour le tableau. |

## 6. Coffret, rail DIN et implantation

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Coffret electrique plastique | IP65 minimum ; cible environ 800 x 600 x 300 mm, polyester renforce ou polycarbonate industriel, porte compatible facade IHM | A verifier avant commande | Enveloppe non metallique imperative pour le Wi-Fi. Capacite minimale calculee 58 emplacements equivalents, sectionneur de facade exclu ; quatre rails de 450 a 500 mm. La taille est imposee aussi par les goulottes, les profondeurs et le Wi-Fi. Candidat : Schneider NSYPLM86BG, IP66/IK10, 847 x 636 x 300 mm, platine bakelite. Voir [NC-0005](../calculs/NC-0005-dimensionnement-coffret-electrique.md). |
| 1 | Platine de fond | Compatible coffret | A commander si non incluse | Pour fixer rail DIN, goulottes, borniers et supports. |
| 4 | Rails DIN | 35 mm, longueur utile 450 a 500 mm | A verifier avant commande | Implantation cible : protections, puissance, controle/12 VDC et borniers. |
| 2 a 4 | Goulottes cable | Avec couvercles, dimensions selon coffret | A verifier avant commande | Separation cheminements BT/secteur. |
| 15 | Presse-etoupes IP68 | 5 M16, 9 M20 et 1 M25, avec contre-ecrous | A commander | 13 sorties actives, 1 arrivee maison 3G2,5 mm2 et 1 reserve. Verifier les diametres reels de cable avant commande. |
| 1 lot | Bouchons de presse-etoupe | Compatibles diametres non utilises | A commander | Pour reserve propre et etanche. |
| 1 | Event / traitement condensation | Event respirant ou solution passive adaptee au coffret | A verifier avant commande | Local humide, condensation a traiter. |

## 7. Borniers, raccordement et reperage

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 8 | Borniers passage gris | Rail DIN, 2,5 mm2, 230 VAC phase | A commander | 1 arrivee maison et 7 departs puissance sortants. |
| 8 | Borniers neutre bleus | Rail DIN, 2,5 mm2 | A commander | 1 arrivee maison et 7 departs puissance sortants. |
| 8 | Borniers terre vert/jaune | Rail DIN, 2,5 mm2 | A commander | 1 arrivee maison et 7 departs puissance sortants. |
| 16 | Borniers passage blancs | Rail DIN, 2,5 mm2, TBT | A commander | Moteur tambour 12 VDC, capteurs niveau, capot et sondes temperature. |
| 1 lot | Pontages borniers | Compatibles borniers choisis | A commander | Distribution propre +12 V, 0 V, phase/neutre selon schema. |
| 1 lot | Reperes borniers et fils | Numerotation, etiquettes ou bagues | A commander | Indispensable pour maintenance et validation. |
| 1 lot | Embouts de cablage | 0,5 / 0,75 / 1,5 / 2,5 mm2 | A commander | Pour fils souples H07V-K. |
| 1 lot | Cosses plates isolees | 6,3 mm et formats necessaires au relais HELLA / accessoires | A commander | A confirmer avec les bornes reelles. |
| 1 lot | Cosses oeillet / fourche | Pour terre et raccordements a vis | A commander | Dimensions a adapter aux bornes finales. |
| 1 lot | Gaine thermoretractable | Plusieurs diametres | A commander | Finitions et isolations locales. |
| 1 lot | Colliers, bases autocollantes, visserie | Nylon/inox selon environnement | A commander | Maintien des cables et petites fixations. |

## 8. Cables et fils

Les longueurs exactes doivent etre mesurees sur site avant commande. Les sections ci-dessous reprennent l'intention V1 documentee ; elles restent a verifier avec le schema final, les longueurs et les protections.

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| A mesurer | Fil interne 230 VAC | H07V-K 1,5 mm2, marron/noir, bleu, vert-jaune | A commander | Cablage interne armoire et departs 230 VAC. |
| A mesurer | Fil interne basse tension | H07V-K souple : 0,5 mm2 pour TBT/signaux, 0,75 mm2 pour alimentation A16 et bobines 12 VDC | A commander | Couleurs : rouge `+12 V`, noir `0 V`, blanc signaux TBT. Ne pas utiliser le bleu, reserve au neutre 230 VAC. |
| A mesurer | Cable souple de porte | 16 x 0,25 ou 0,34 mm2, cable de commande multiconducteur | A commander | 13 ames utiles : LCD 3,3 V/I2C, selecteur, 4 boutons, 3 voyants et 0 V commun ; 3 ames de reserve. |
| A mesurer | Cable moteur tambour | 2 x 2,5 mm2 ou cable adapte environnement | A verifier avant commande | Section retenue en premiere intention pour moteur 12 VDC. |
| A mesurer | Cable departs 230 VAC terrain | 3G1,5 mm2 adapte local humide / exterieur selon pose | A verifier avant commande | Pompe rincage, filtration, UV, decoration, mise a niveau, prises. |
| 10 a 15 m | Cable eclairage exterieur | Section et type a confirmer selon pose | A verifier avant commande | Depart eclairage 6 spots LED 3 W avec detecteurs. |
| A mesurer | Rallonges capteurs | Cable 3 fils 0,34 a 0,75 mm2, ou connectique etanche adaptee | A verifier avant commande | Pour CR18-8DN et DS18B20 si cables d'origine insuffisants. |

## 9. Pieces mecaniques et hydrauliques a verifier

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Toile tambour | Inox 74 microns | A verifier stock | Reference de filtration retenue. |
| 1 | Joint a levre tambour | Dimension selon tambour | A verifier avant commande | Indispensable separation eau sale / eau propre. |
| 1 | Tube report niveau | PVC 32 mm avec event 1 mm | A verifier stock | Support des capteurs de niveau. |
| 1 | Rampe de rincage | Tuyau 32 mm + buses | Deja achete/fabrique selon doc | Verifier que tout le raccordement est complet. |
| 1 lot | Raccords pompe rincage | Raccords 1 pouce, clapet/vanne si schema hydraulique le demande | A verifier avant commande | Adapte a la VEVOR / Leo EKJ-802S. |
| 1 | Support FAT | Structure a fabriquer | A verifier avant commande | Conditionne les niveaux gravitaires. |
| 1 | Capot transparent | Matiere et isolation a definir | A verifier avant commande | Doit actionner le ME-8104 et permettre l'inspection visuelle. |

## 10. Materiel hors controleur a confirmer

Ces elements sont cites dans l'architecture mais peuvent deja exister sur l'installation. Ne les commander que si le stock/projet les confirme manquants.

| Qté | Article | Reference / caracteristiques | Statut | Notes |
| ---: | --- | --- | --- | --- |
| 1 | Pompe principale de filtration | AquaForte DM-Vario 20 000 ou reference existante | A verifier stock | Depart dedie 6 A. |
| 1 | UV | Reference existante, hors tambour apres pompe | A verifier stock | Asservi a la filtration et EP_CRITIQUE absent. |
| 1 | Pompe decoration | Reference existante | A verifier stock | Coupee sur EP_CRITIQUE. |
| 1 | Mise a niveau automatique | Dispositif final a confirmer | A verifier stock | Coupee sur EP_CRITIQUE. |
| 2 | Bulleurs | Bassin + filtre bio | A verifier stock | Alimentes directement, hors sorties controlees. |
| 4 | Prises local | 2 bulleurs + 2 maintenance | A verifier avant commande | Prises compatibles local technique humide. |

## 11. Minimum de rechange a avoir dans le coffret

| Qté | Piece de rechange | Notes |
| ---: | --- | --- |
| 1 | CR18-8DN | Niveau lavage ou critique. |
| 1 | DS18B20 | Eau ou local. |
| 1 | Bouton 22 mm 1NO1NC | Remplacement reset/test/manuel selon couleur disponible. |
| 1 | Voyant 16 mm 12 VDC | Rouge ou jaune prioritaire. |
| 1 | Contacteur TOMZN TOCT1-25Z 12 VDC | Si budget/place disponible. |
| 1 lot | Fusibles ATO 1 A / 3 A / 5 A | A garder dans un sachet repere. |

## 12. Points a confirmer avant validation panier

- Reference exacte du coffret plastique IP65 : cible recommandee 800 x 600 x 300 mm environ, quatre rails utiles de 450 a 500 mm, platine isolante de preference et zone radio libre autour de l'antenne KC868-A16. Le bilan detaille est dans [NC-0005](../calculs/NC-0005-dimensionnement-coffret-electrique.md).
- Reference finale de l'interrupteur differentiel 2P 30 mA 40 A type A et coherence de gamme avec les disjoncteurs.
- Etat reel du sectionneur TeSys VCDN20 trouve en atelier.
- Longueurs de cables terrain : moteur tambour, capteurs, sondes, pompe rincage, filtration, UV, decoration, mise a niveau, prises et eclairage.
- Diametre des cables pour choisir correctement les presse-etoupes.
- Validation de l'interface directe KC868-A16 / CR18-8DN avant cablage definitif.
- Compatibilite 3,3 V reelle du LCD 2004 I2C et du module DS3231.
- Brochage reel des DS18B20 et couleurs de fils du lot recu.
- Revue du schema 230 VAC avant achat final si les references tableau changent.
