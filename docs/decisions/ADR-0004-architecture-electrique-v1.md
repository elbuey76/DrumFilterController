# ADR-0004 - Architecture electrique V1

## Statut

Acceptee

## Contexte

Le controleur du FAT doit separer proprement la distribution secteur 230 VAC, les sorties de puissance et la basse tension 12 VDC. L'installation est dans un local humide protege de la pluie directe, avec des organes essentiels pour la survie du bassin et des organes secondaires qui ne doivent pas pouvoir arreter la filtration principale en cas de defaut.

Les choix ci-dessous documentent l'architecture retenue pour la commande definitive du MVP, avant schema electrique detaille. Les calibres definitifs devront rester coherents avec les caracteristiques constructeur, les longueurs de cables, le coffret retenu et les regles applicables.

## Decision

L'alimentation du coffret FAT doit venir d'un depart dedie cote maison, protege par un disjoncteur 16 A. Ce calibre remplace l'hypothese initiale de 20 A, car la liaison existante vers le local filtration fait environ 20 m et la section reellement tiree cote maison n'est pas confirmee a 2,5 mm2.

La sequence cible est : disjoncteur 16 A cote maison, interrupteur-sectionneur local TeSys VCDN20 20 A dans le coffret bassin, interrupteur differentiel 30 mA type A 40 A, puis disjoncteurs des departs.

Le tableau 230 VAC local retient l'organisation suivante :

| Depart | Protection retenue | Equipements alimentes | Commentaire |
| --- | --- | --- | --- |
| Coupure generale locale | Schneider Electric TeSys VCDN20, interrupteur-sectionneur 3P, 690 V, 20 A, poignee rouge cadenassable | Ensemble de la partie coffret placee en aval | Utilise comme sectionneur de consignation local, place avant l'interrupteur differentiel du coffret. En monophase, couper phase et neutre avec deux poles ; le troisieme pole reste inutilise ou reserve selon schema final. |
| Tete de tableau | Interrupteur differentiel 2P 30 mA, 40 A, type A | Ensemble du tableau local | Calibre coherent avec les charges reelles du MVP, la pompe AquaForte DM-Vario 20 000 limitee a 187 W, les prises maintenance ponctuelles et les disjoncteurs aval dimensionnes aussi pour les appels moteurs. |
| Alimentation 12 VDC | Disjoncteur 4 A courbe C | Alimentation Mean Well NDR-120-12 | Depart dedie basse tension controle. |
| Pompe de rincage | Disjoncteur 10 A courbe C | Pompe de rincage 230 VAC | Commandee par contacteur moteur Schneider TeSys LC1D12P7, bobine 230 VAC. |
| Prises local | Disjoncteur 16 A courbe C | 1 prise bulleur bassin, 1 prise bulleur filtre bio, 2 prises maintenance ponctuelle | Les bulleurs restent hors controleur et hors coupure niveau critique. Les prises maintenance servent par exemple aspirateur ou pompe de relevage temporaire. Le courant total disponible reste limite par le disjoncteur amont 16 A cote maison. |
| Pompe filtration | Disjoncteur 6 A courbe C | Pompe principale de filtration | Depart separe car organe essentiel. |
| UV, pompe decoration, mise a niveau | Disjoncteur 6 A courbe C | UV, pompe decoration, mise a niveau automatique | Depart separe de la filtration pour qu'un defaut sur un organe non essentiel ne coupe pas la pompe de filtration. |
| Eclairage exterieur | Disjoncteur 6 A courbe C | 6 spots LED exterieurs de 3 W, avec detecteurs, environ 10 a 15 m de cable | Depart supplementaire distinct, hors automatisme FAT et sans role de securite bassin. La charge nominale est de 18 W, soit environ 0,08 A sous 230 VAC ; le calibre 6 A est retenu comme depart dedie confortable, a confirmer avec la section et les conditions de pose du cable. |

Le TeSys VCDN20 trouve en atelier est donc retenu comme candidat de coupure locale cadenassable du coffret FAT. Il ne remplace ni l'interrupteur differentiel 30 mA ni les disjoncteurs aval. Son integration finale doit verifier l'etat du composant, le schema monophase phase/neutre, le calibre amont 16 A, le pouvoir de coupure adapte aux charges moteur et le maintien des bulleurs hors coupure automatique du controleur.

La basse tension V1 retient une alimentation Mean Well NDR-120-12, 12 VDC, 120 W, 10 A, montee sur rail DIN. Le 12 V est distribue par un porte-fusibles ATO 4 emplacements. Si le modele retenu n'est pas directement compatible rail DIN, un adaptateur imprime en 3D sera prevu pour le fixer proprement dans le coffret :

| Depart 12 VDC | Fusible retenu | Usage |
| --- | --- | --- |
| Moteur tambour | 5 A | Motorisation tambour 12 VDC. |
| Automate | 3 A | KC868-A32. |
| Capteurs et boutons | 1 A | Capteurs, contacts et commandes locales. |
| Ecran, voyants, accessoires | 1 A | IHM locale et signalisation. |

La plateforme de controle retenue est un KC868-A32.

Cette plateforme et cette architecture de puissance constituent la base matérielle MVP et doivent rester compatibles avec la V2 sans remplacement de la plateforme principale.

La motorisation tambour retenue est un motorreducteur Fyearfly 12 VDC, 10 rpm. Elle est commandee par un relais HELLA 12 V, reference 4RD 933 332-551, avec charge inductive 15 A. Un support sera imprime en 3D pour monter proprement ce relais automobile sur rail DIN.

La pompe de rincage est commandee par un relais de l'automate qui pilote directement la bobine 230 VAC d'un contacteur moteur Schneider Electric TeSys LC1D12P7, 3P, 12 A, AC-3. Cette solution est retenue pour eviter d'ajouter un relais intermediaire 12 V -> 230 V et parce que le contacteur equivalent en bobine 12 VDC avec categorie moteur AC-3 est difficile a sourcer ; le schema final devra donc traiter clairement le cheminement 230 VAC de cette commande, la separation BT/secteur et le reperage des borniers.

L'UV, la pompe de decoration et la pompe de filtration sont commandes par les relais de l'automate qui pilotent en 12 VDC des contacteurs modulaires TOMZN TOCT1-25Z, 25 A. Chaque organe conserve sa logique de securite fonctionnelle : la pompe de filtration est separee de la pompe de decoration et de l'UV afin de limiter les defauts communs.

## Consequences

- La tension de commande V1 est maintenant fixee a 12 VDC.
- Le KC868-A32 devient la base du schema de cablage et du futur firmware.
- La commande materielle MVP est definitive dans son principe : les validations restantes portent sur le schema, les interfaces, les sections, les protections et l'implantation, pas sur une plateforme temporaire.
- Le moteur d'essuie-glace SWF 403.835 n'est plus la reference principale de motorisation tambour ; il est remplace par le motorreducteur Fyearfly 12 VDC 10 rpm.
- La separation des departs 230 VAC evite qu'un defaut sur l'UV, la pompe decoration ou la mise a niveau coupe directement le depart de pompe filtration.
- Les bulleurs sont alimentes par les prises classiques du local, mais restent hors sorties controlees afin de maintenir l'aeration independante.
- Le depart cote maison doit etre protege par un disjoncteur 16 A dedie, cote maison, afin de rester coherent avec la liaison existante d'environ 20 m tant que sa section n'est pas confirmee en 2,5 mm2 ; ce calibre limite aussi le courant total disponible du coffret filtration.
- L'interrupteur differentiel de tete est retenu en 2P 30 mA, 40 A, type A. La notice AquaForte DM-Vario impose une protection differentielle 30 mA mais ne demande pas de type F ou B ; le type A est donc retenu comme choix rationnel pour les charges electroniques du MVP.
- Le TeSys VCDN20 trouve en atelier peut etre reutilise comme sectionneur de consignation local avant l'interrupteur differentiel du coffret si son etat mecanique/electrique est sain et si le schema final confirme son cablage.
- Un depart supplementaire d'eclairage exterieur est reserve dans le coffret, protege par disjoncteur 6 A courbe C ; ce circuit n'a pas de role fonctionnel dans le controleur du bassin.
- Les sections de cablage retenues pour le MVP sont : 2 x 2,5 mm2 pour le moteur tambour 12 VDC, 0,5 mm2 pour les voyants et boutons, et 1,5 mm2 pour les circuits 230 VAC cote filtration, y compris dans l'armoire, afin de rester coherent avec l'hypothese prudente d'une liaison amont non confirmee en 2,5 mm2 ; ces choix restent soumis a verification du schema final, des longueurs, des conditions de pose et des protections associees.
- La somme des fusibles 12 VDC est maintenant coherente avec l'alimentation 10 A, mais le courant reel reste a mesurer avec moteur, automate, bobines, IHM et voyants actifs.
- Les valeurs de fusibles et disjoncteurs restent a confirmer par revue de schema, fiches constructeur, sections de cable, conditions de pose et essais, notamment l'absence de declenchement intempestif du disjoncteur 4 A courbe C a la mise sous tension de l'alimentation Mean Well.
- Les adaptations imprimees en 3D pour le relais HELLA et le porte-fusibles ATO deviennent des pieces du coffret a concevoir et valider mecaniquement.

## Alternatives considerees

- Garder un seul depart 230 VAC pour filtration, UV et decoration : non retenu, car un defaut sur un organe secondaire pourrait couper la filtration principale.
- Conserver le moteur d'essuie-glace SWF 403.835 : non retenu pour la V1 documentee ici, car le motorreducteur Fyearfly 12 VDC 10 rpm simplifie la reduction mecanique et le dimensionnement de vitesse.
- Commander directement les charges secteur par les relais de l'automate : non retenu pour les charges moteur et les departs de puissance, qui passent par des contacteurs adaptes.
- Retenir un differentiel 63 A ou type F par defaut : non retenu, car les charges reelles du MVP ne justifient pas 63 A et la notice de la pompe de filtration variable ne demande pas de type F. Ces options restent possibles seulement si le schema amont ou un retour terrain les justifie.
