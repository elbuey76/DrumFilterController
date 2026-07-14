# ADR-0011 - Strategie de validation materielle V1

## Statut

Acceptee

## Contexte

L'architecture materielle V1 du controleur de filtre a tambour est maintenant largement fixee : plateforme KC868-A16 ESP32 classique selon ADR-0012, alimentation 12 VDC, capteurs de niveau CR18-8DN, motorisation tambour 12 VDC, pompe de rincage 230 VAC deja achetee, relais d'interface de rinçage a choisir, contacteurs de puissance, IHM locale, RTC et sondes de temperature.

Ces choix ne sont pas des choix de maquette jetable : ils constituent la base cible du MVP et doivent rester compatibles avec la V2. Cependant, plusieurs points critiques restent conditionnes a des validations banc, schema ou terrain : sens logique des entrees, rupture de fils, etats au boot, perte du 12 VDC, separation basse tension / secteur, comportement des contacteurs, courant moteur, debit de rincage, condensation, reglage des seuils hydrauliques et reprise apres defaut.

La validation materielle doit donc empecher deux erreurs opposees :

- rouvrir inutilement l'architecture a chaque doute mineur ;
- transformer des hypotheses non mesurees en cablage definitif.

## Decision

La V1 retient une validation progressive et bloquante avant exploitation autonome.

Les choix materiels acceptes restent figes par defaut, mais une mesure ou un essai reel peut rouvrir une ADR si le resultat montre une incompatibilite electrique, une limite de securite, un echauffement anormal, un declenchement intempestif, un comportement instable ou une inefficacite hydraulique majeure.

Le banc de test materiel est obligatoire avant cablage definitif du coffret. Il doit couvrir au minimum :

- KC868-A16, avec identification de revision, scan I2C et sorties MOSFET desarmees ;
- capteurs CR18-8DN `EP_LAVAGE` et `EP_CRITIQUE` ;
- simulation du contact capot ;
- boutons et selecteur AUTO / MAINTENANCE ;
- commandes de relais et contacteurs sans charge dangereuse ;
- LCD 2004 I2C ;
- RTC DS3231 ;
- sondes DS18B20.

Les essais suivants sont bloquants avant mise en eau reelle :

- toutes les sorties controlees sont OFF au boot avant lecture des securites ;
- perte du 12 VDC de commande = contacteurs et sorties controlees OFF par conception ;
- `EP_CRITIQUE` confirme coupe ou inhibe filtration, UV, decoration, mise a niveau, tambour et rincage selon l'architecture retenue ;
- capot ouvert coupe ou interdit tambour et rincage ;
- incoherence critique des capteurs niveau met le systeme en securite ;
- retour apres `EP_CRITIQUE` demande retour niveau stable puis acquittement local ;
- duree maximale de lavage respectee ;
- reset refuse si la cause active persiste ;
- IHM locale affiche la cause principale de refus ou de defaut ;
- rupture ou deconnexion du contact capot est interpretee comme capot ouvert.

La mise en service terrain est progressive :

1. simulation sur table ;
2. coffret alimente sans charges de puissance ou charges deconnectees ;
3. commande des contacteurs ;
4. essai tambour et rincage en manuel ;
5. essai automatique sous presence humaine ;
6. exploitation normale seulement apres plusieurs cycles observes.

Le schema 230 VAC final doit etre relu par une personne competente avant mise sous tension reelle, notamment pour le differentiel, les disjoncteurs, la terre, les sections, les borniers, la separation basse tension / secteur et les cheminements.

La V1 documente explicitement ses angles morts. En particulier, sans capteur dedie, elle ne pretend pas detecter directement un contacteur colle, un relais colle, une pompe de rincage faible mais non arretee, un tambour qui tourne mal, un capteur niveau mal positionne ou un blocage tambour par mesure de courant. Ces cas sont couverts par protections materielles, temporisations, alarmes indirectes, inspection et validation terrain.

Une checklist go/no-go materielle est creee dans `docs/validation/` et doit etre passee avant cablage final puis avant exploitation autonome.

## Consequences

- Les ADR materiels restent utiles et stables, mais le reel garde un droit de veto.
- Les validations banc et terrain deviennent des prerequis, pas de simples recommandations.
- Le dossier doit conserver les mesures de courant moteur, debit de rincage, comportement des capteurs, tests de boot, tests de perte alimentation et revue 230 VAC.
- La pompe de rincage deja achetee reste la reference materielle V1 ; si son essai est insuffisant, les adaptations portent d'abord sur durees, buses, rampe, pression ou conditions de fonctionnement.
- Le troisieme capteur niveau n'est pas ajoute en V1, mais une reserve mecanique et de cablage doit etre prevue.
- Les pieces critiques et references de rechange doivent rester documentees pour faciliter le depannage.
- Une fiche papier plastifiee dans le coffret est retenue comme support d'exploitation local simple.

## Alternatives considerees

- Cablage direct sans banc complet : non retenu, car trop de choix restent conditionnes par des comportements electriques reels.
- Remettre tous les choix materiels en concurrence avant prototype : non retenu, car l'architecture est deja suffisamment convergente et plusieurs composants sont achetes.
- Validation uniquement logicielle : non retenue, car les risques principaux incluent puissance, humidite, hydraulique, capteurs et contacteurs.
- Exploitation autonome apres un seul cycle reussi : non retenue, car les reglages de niveau, la mecanique et le rincage demandent observation terrain.
