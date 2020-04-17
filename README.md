Adrian Cottais
# Contrôle d’une lampe avec The Things Network et Cayenne:

**Matériel requis:**

- Passerelle LORA connectée à TTN
- Carte TheThingUno
- Relais arduino et sorties 220V
- Une lampe 220V 



**Schéma de câblage:**



![](https://i.imgur.com/SELa5ZO.jpg)



Sur ce schéma, la carte utilisée est une UNO mais les branchements sont les mêmes sur la carte ThingUno.

Pour déclencher le relais, on utilise un port digital car on seulement besoin de transmettre 1 ou 0. 

Pour l'ampoule, on branche un câble sur le port COM et un sur le porc NO qui signifie Normalement Ouvert, c'est à dire que la lampe sera éteinte de base.


### Configuration ThingUno et The Things Network

Une fois l’ensemble des câbles connectés sur la carte ThingUno la configuration avec TTN peut commencer.

Il nous faut d’abord récupérer l’identifiant de l’appareil (DevEui) qui permet de l’identifier de manière unique.
Pour cela, on utilise le code “DeviceInfo” présent dans les exemples Arduino de la librairie TTN.
On remplace la fréquence d’émission avec la fréquence utilisée dans la pays où l’on se trouve:

![](https://i.imgur.com/erQfMSi.png)



Une fois le code téléversé, on obtient le résultat suivant dans la console:


![](https://i.imgur.com/GBGh8f5.png)





Nous pouvons maintenant créer une application sur TTN. Il faut cliquer sur créer une nouvelle application et remplir les champs comme le montre la capture d’écran ci-dessous.

![](https://i.imgur.com/VNegyBa.png)


Pour le choix du serveur, choisissez le serveur le plus proche de votre zone géographique pour minimiser les latences.

Une fois cette opération réalisée, on doit enregistrer notre carte ThingUno avec notre application.

Pour cela, on se rend sur notre application pour dans la partie "Devices".

![](https://i.imgur.com/7zb0OvP.png)


On clique ensuite sur “Register device” et on rempli les champs “Device ID” avec le nom que l’on souhaite et le champ “Device EUI” qui correspond à notre numéro de carte trouvé précédemment.
Les autres champs se remplissent automatiquement.

![](https://i.imgur.com/YiZG648.png)


Une fois notre carte enregistrée, on peut la visualiser toujours dans la partie “Device” de notre application. En cliquant sur le nom de celle-ci, on peut avoir les détails la concernant.


![](https://i.imgur.com/qGtSi9S.png)



### Contrôle via TTN

**Le code source utilisé se trouve en intégralité sur ce [lien](https://)**.

Comme notre carte est correctement enregistrée, nous allons maintenant la configurer pour allumer et éteindre la lampe depuis TTN dans un premier temps.

Pour cela nous allons utliser le programme exemple proposé par la librairie TTN sur Arduino nommé "Receive".

Nous avons dans un premier temps créer une constante qui identifie le port digital sur lequel notre relai est connecté comme suit:

![](https://i.imgur.com/swrXuMw.png)

Nous allons utiliser cette constante dans le reste du programme au lieu spécifier le numéro du port à chaque fois.

A noté que les ports digitaux 0 et 1 ne semblent pas fonctionner correctement avec le code de TTN.

Nous avons remplacé la ligne "appEUI" et "appKey" avec les clés que l'ont peut retrouver sur l'interface de notre application TTN.

On doit aussi remplacer la fréquence d'émission comme nous l'avons fait dans le programme "Device Info".


Nous allons maintenant nous intéresser à la partie void setup de
notre programme.

![](https://i.imgur.com/FnK20Xk.png)

### **Explication des principales lignes:**

- **loraSerial(57600):** Inialisation de la transmission LoRa à 57600 bits/s
- **debugSerial(9600):** Initialisation de la console Arduino
- **ttn.onMessage(message):** Lors de la réception d'un message, on va utiliser la fonction "message" pour traiter les données.
- **ttn.showStatus:** Permet de voir l'ensemble des informations sur nos canaux de transmission
- **ttn.join(appEui, appKey):** Etablissement de la connexion avec notre application sur TTN.

La partie void loop du programme est assez simple puisque composée d'un seul élement majeur.

![](https://i.imgur.com/QPK1xoF.png)

- **ttn.poll():** envoi un octet vers TTN d'une valeur de 0 qui va permettre de vérifier si un message en attente sur TTN souhaite aller vers notre carte. Il est impossible de recevoir des messages si on envoi pas cet octet.
- **delay(10000):** On attend 10 secondes entre chaque envoi, ce temps peut être modifié en fonction des besoins.

Nous allons maintenant nous intéresser à la fonction "message" qui est exécutée à chaque fois que nous recevons un message.

Le payload que nous allons voir représente ce que nous recevons.
Un payload est exprimé en octet. Si on utilise la notion payload[0] cela signifie que l'on utilise le premier octet reçu. De même pour payload[1] qui est le 2ème octet reçu et ainsi de suite.


![](https://i.imgur.com/Z8fs3Zt.png)

Dans notre cas, les 2 premières lignes permettent de donner des informations sur la taille du message reçu, le port de réception. Elles sont affichées en console.

Pour allumer ou éteindre la lampe, nous allons devoir choisir une valeur par état:

- Allumer: 1
- Éteindre: Toutes autres valeurs différentes de 1 

Pour coder ces valeurs, nous auront donc simplement d'envoyer un octet, ce qui est le minimum. 

Si on regarde le code, on retrouve la valeur des états dans notre structure conditionelle if else.

En effet, si le payload[0], soit le premier et unique octet est égale à 1, alors on allume la lampe avec la commande **digitalWrite(pinOutPut, HIGH)**. 
Si cet octet est égale à toutes autres valeurs, on éteind la lampe avec la commande **digitalWrite(pinOutPut, LOW)**.

>digitalWrite(numéro de port, état (LOW/HIGH))

On téléverse le programme et on se rend sur TTN, sur l'interface de notre device.
Dans la section "Downlink", on retrouve un champ où l'on peut saisir les valeurs à envoyer vers notre carte.
Ici 01 pour l'exemple.

![](https://i.imgur.com/PZQx5ob.png)

**Attention:** Sur cette interface, nous devons encoder les données en hexadécimal donc pour envoyer un octet de valeur 1, nous devons écrire 01.

Une fois l'octet envoyé depuis TTN, on attend quelques secondes et on peut voir la lampe s'allumer ou s'éteindre.

Pour plus de détails, vous pouvez retrouvez une démonstration sur ce [lien](https://www.youtube.com/watch?v=uzXIE61C4fk&feature=youtu.be).

### Contrôle via Cayenne 

Nous allons maintenant voir la partie concernant le contrôle de la lampe grâce à l'interface de Cayenne. Les données LoRa transiteront toujours via TTN mais encodées dans un format différent, CayenneLPP.

Cayenne LowPowerPayload(LPP) permet d'encoder les données de notre noeud et de les transmettre vers Cayenne par l'intermédiaire de TTN.

Dans un premier temps, il faut d'abord installer et ajouter la librairie à notre code arduino.

![](https://i.imgur.com/Pln4UPb.png)

Les constantes "appEUI" et "appKey" reste les mêmes étant donné que l'on va utiliser la même application sur TTN. De même pour le plan de fréquences.

Nous allons nous intéresser dans un premier au void setup() de notre programme:
![](https://i.imgur.com/hBkcCkO.png)

On déclare dans un premier temps une variable lpp de 3 octets de type Cayenne LPP qui nous servira pour stocker et envoyer nos données.
On peut définir maximum 51 octets pour la variable lpp mais dans notre cas, seulement 3 sont utiles pour envoyer les données vers Cayenne.

En effet, d'après la documentation, une structure de payload se présente comme suit:

![](https://i.imgur.com/5yN7tse.png)

- 1 octet pour identifier le canal, c'est à dire le numéro de widget sur Cayenne
- 1 octet pour le type de données
- N octet pour les données à transmettre

Dans notre cas, 1 octet pour les données à transmettre sera largement suffisant car nous pouvons coder des nombres entre 0 et 255 et que les valeurs à transmettre vers cayenne seront toujours 0 ou 1.

Le début du void setup est le même que précédement étant donné que l'on utilise toujours TTN pour transmettre nos données.

Seulement 3 lignes du programme vont changer:

- **lpp.reset()** permet de remettre notre variable à 0 
- **lpp.addDigitalOutput(1,0)** indique à cayenne d'ajouter une sortie digitale sur l'interface en ligne. Le 1 correspond à notre numéro de canal et 0 correspond à notre état de base, c'est à dire éteint.
- **ttn.sendBytes(lpp.getBuffer, lpp.getSize())** permet d'envoyer les données vues précédement grâce au protocole de TTN.


Le void loop lui reste exactement le même que celui vu auparavent.

En revanche la fonction message a été modifiée pour pouvoir contrôler notre lampe:

![](https://i.imgur.com/rGXGXp5.png)

Lors de différents essais de reception de données, nous avons vu que nous recevions 4 octets de la part de Cayenne. Les 2 premiers sont le canal et le type des données et le 3ème est la valeur pour contrôler la lampe.
Le dernier est FF.

Nous allons donc utiliser la payload[2] qui correspond au 3ème octet.

Comme le montre le code, lorsque nous recevons une consigne pour allumer la lampe, nous recevons un octet de valeur 100 de la part de Cayenne et 0 quand on souhaite l'éteindre.

Comme la montre la structure if else, quand nous recevons une de ces 2 valeurs, nous envoyons soit un signal HIGH à notre relais ou LOW.

La partie suivante n'est pas obligatoire mais est plus pratique. En effet, nous renvoyons l'état de la lampe sur Cayenne ce qui est permet au switch de savoir si la lampe est allumée ou non. 
Cela est très pratique quand on utilse plusieurs appareils (pc ou téléphone par exemple ) en même temps, car on connaît l'état de la lampe sur tout ces supports.

La partie arduino est finie, il reste maintenant à configurer TTN et Cayenne.

Sur TTN:

Dans la section "Payload Format", on doit choisir "Cayenne LPP".

![](https://i.imgur.com/jj9KNWl.png)

Ensuite on doit créer une nouvelle intégration:

![](https://i.imgur.com/066ro7H.png)

On chosit ensuite la plateforme "mydevices":


![](https://i.imgur.com/1SfVGqC.png)

Et enfin on indique le nom de l'intégration et on choisit "Default-Key":

![](https://i.imgur.com/WHURHsK.png)

**Sur Cayenne:**

On clique d'abord sur "Add New" puis "Device/Widget". Dans la section "LoRa" on chosit "TTN".
On remplit ensuite le nom de notre device, puis le "DevEUI", on peut choisir aussi l'adresse à laquelle se trouve notre objet si celui-ci ne bouge pas.

![](https://i.imgur.com/kbKgtLF.png)

Une fois cela fait, on accède à notre interface ou l'on va retrouver notre bouton avec une carte et quelques informations concernant la transmission.
Il suffit de cliquer sur le bouton pour allumer ou éteindre la lampe.

![](https://i.imgur.com/ZKxhxGc.png)

**Fin de la configuration**
