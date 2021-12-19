# ChatRoomOS-2021-2022

## Code Ereur

- **0 - NORMAL_EXIT** : Code de retour de programme qui s'est terminé avec succès.
- **1 - EXIT_BAD_USAGE_ERROR** : Mauvaises utilisations d'arguments.
- **2 - EXIT_SOCK_ERROR** : Une erreur qui survient lors d'une connection ou communication avec le serveur.
- **3 - EXIT_OTHER_ERROR** : Tout autre type d'erreur qui surviendrait lors de l'exécution du programme.


## Lancer le salon de chat 

### Prérequis 
Étant donné deux exécutables, client et server, lancez le server.
Par la suite, lancez les clients avec les bons paramètres.


### Server 
Le serveur se lance via la commande suivante :
```shell 
source ./bin/server <port>
```


### Client 
Pour chacun des clients lancer la commande 
```shell 
source ./bin/client <pseudo> <ip_serveur> <port>
```

## Terminaison

### Client 
"Ctrl+D" pour se déconnecter du server.
Un cas de terminaison est la perte de connection avec le server.

### Server
"Ctrl+C" pour arrêter le server. Ce signal est capturé de manière à déconneceter les clients après l'arrêt du server.

## Compilation locale

Dans le terminal se déplacer jusque dans le dossier source du MakeFile.
```shell
cd ChatRoomOS-2021-2022; make
```

Par la suite, vous pourrez trouver les exécutables dans le répertoire ```./bin/```.
