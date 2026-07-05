# ADR-0004 - Architecture electrique V1

## Statut

Acceptee

## Contexte

Le controleur du FAT doit separer proprement la distribution secteur 230 VAC, les sorties de puissance et la basse tension 12 VDC. L'installation est dans un local humide protege de la pluie directe, avec des organes essentiels pour la survie du bassin et des organes secondaires qui ne doivent pas pouvoir arreter la filtration principale en cas de defaut.

Les choix ci-dessous documentent l'architecture retenue avant schema electrique detaille. Les calibres definitifs devront rester coherents avec les caracteristiques constructeur, les longueurs de cables, le coffret retenu et les regles applicables.

## Decision

Le tableau 230 VAC retient l'organisation suivante :

| Depart | Protection retenue | Equipements alimentes | Commentaire |
| --- | --- | --- | --- |
| Tete de tableau | Interrupteur differentiel 30 mA | Ensemble du tableau local | Type et calibre definitifs a arbitrer en backlog. |
| Alimentation 12 VDC | Disjoncteur 4 A courbe C | Alimentation Mean Well NDR-120-12 | Depart dedie basse tension controle. |
| Pompe de rincage | Disjoncteur 10 A courbe C | Pompe de rincage 230 VAC | Commandee par contacteur moteur Schneider TeSys LC1D12P7, bobine 230 VAC. |
| Prises local | Disjoncteur 16 A courbe C | 1 prise bulleur bassin, 1 prise bulleur filtre bio, 2 prises maintenance ponctuelle | Les bulleurs restent hors controleur et hors coupure niveau critique. Les prises maintenance servent par exemple aspirateur ou pompe de relevage temporaire. |
| Pompe filtration | Disjoncteur 6 A courbe C | Pompe principale de filtration | Depart separe car organe essentiel. |
| UV, pompe decoration, mise a niveau | Disjoncteur 6 A courbe C | UV, pompe decoration, mise a niveau automatique | Depart separe de la filtration pour qu'un defaut sur un organe non essentiel ne coupe pas la pompe de filtration. |

La basse tension V1 retient une alimentation Mean Well NDR-120-12, 12 VDC, 120 W, 10 A, montee sur rail DIN. Le 12 V est distribue par un porte-fusibles ATO 4 emplacements :

| Depart 12 VDC | Fusible retenu | Usage |
| --- | --- | --- |
| Moteur tambour | 7,5 A | Motorisation tambour 12 VDC. |
| Automate | 3 A | KC868-A32. |
| Capteurs et boutons | 1 A | Capteurs, contacts et commandes locales. |
| Ecran, voyants, accessoires | 1 A | IHM locale et signalisation. |

La plateforme de controle retenue est un KC868-A32.

La motorisation tambour retenue est un motorreducteur Fyearfly 12 VDC, 10 rpm. Elle est commandee par un relais HELLA 12 V, reference 4RD 933 332-551, avec charge inductive 15 A. Un support sera imprime en 3D pour monter proprement ce relais automobile sur rail DIN.

La pompe de rincage est commandee par un relais de l'automate qui pilote la bobine 230 VAC d'un contacteur moteur Schneider Electric TeSys LC1D12P7, 3P, 12 A, AC-3.

L'UV, la pompe de decoration et la pompe de filtration sont commandes par les relais de l'automate qui pilotent en 12 VDC des contacteurs modulaires TOMZN TOCT1-25Z, 25 A. Chaque organe conserve sa logique de securite fonctionnelle : la pompe de filtration est separee de la pompe de decoration et de l'UV afin de limiter les defauts communs.

## Consequences

- La tension de commande V1 est maintenant fixee a 12 VDC.
- Le KC868-A32 devient la base du schema de cablage et du futur firmware.
- Le moteur d'essuie-glace SWF 403.835 n'est plus la reference principale de motorisation tambour ; il est remplace par le motorreducteur Fyearfly 12 VDC 10 rpm.
- La separation des departs 230 VAC evite qu'un defaut sur l'UV, la pompe decoration ou la mise a niveau coupe directement le depart de pompe filtration.
- Les bulleurs sont alimentes par les prises classiques du local, mais restent hors sorties controlees afin de maintenir l'aeration independante.
- Le choix du type et du calibre de l'interrupteur differentiel 30 mA reste a documenter avant achat et cablage final.
- Les valeurs de fusibles et disjoncteurs restent a confirmer par revue de schema, fiches constructeur, sections de cable, conditions de pose et essais.

## Alternatives considerees

- Garder un seul depart 230 VAC pour filtration, UV et decoration : non retenu, car un defaut sur un organe secondaire pourrait couper la filtration principale.
- Conserver le moteur d'essuie-glace SWF 403.835 : non retenu pour la V1 documentee ici, car le motorreducteur Fyearfly 12 VDC 10 rpm simplifie la reduction mecanique et le dimensionnement de vitesse.
- Commander directement les charges secteur par les relais de l'automate : non retenu pour les charges moteur et les departs de puissance, qui passent par des contacteurs adaptes.
